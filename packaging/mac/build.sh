#!/bin/zsh
set -e

# This is a build script for building on macOS and creating a self-contained
# .app bundle for Seven Kingdoms: Ancient Adversaries. The prefix is embedded
# inside the bundle so the package is fully relocatable.

# The build system will need the following Homebrew packages installed:
#   brew install pkg-config sdl2 openal-soft gettext
#
#   Package       Purpose
#   -----------   -------------------------------------------------
#   pkg-config    used by configure to locate libraries
#   sdl2          graphics and input
#   openal-soft   audio (macOS system OpenAL is a non-linking stub)
#   gettext       NLS locale support (provides libintl)

# Usage: sh packaging/mac/build.sh [--with-multiplayer] [--with-dmg] [OUTPUT_DIR]
#   OUTPUT_DIR defaults to packaging/mac/
#   --with-multiplayer  build with ENet LAN multiplayer support; online lobby
#                       disabled (see workaround note below in this script)
#                       (requires: brew install enet)
#   --with-dmg          also create a drag-to-install DMG after the bundle
#
# Note: music is not included. Download the music archive from
#   https://www.7kfans.com/download and copy the MUSIC/ directory into data/.

STRIP=strip

REPO="$(cd "$(dirname "$0")/../.." && pwd)"
BIN="$REPO/build/src/7kaa"
DATA="$REPO/data"
ICON_SRC="$REPO/data/IMAGE/7K_ICON.BMP"
WITH_MULTIPLAYER=no
WITH_DMG=no
for _arg in "$@"; do
  case "$_arg" in
    --with-multiplayer) WITH_MULTIPLAYER=yes ;;
    --with-dmg) WITH_DMG=yes ;;
    *) OUT="$_arg" ;;
  esac
done
OUT="${OUT:-"$REPO/packaging/mac"}"

NAME="Seven Kingdoms AA"
VERSION=$(sed -n 's/^AC_INIT(\[[^]]*\], \[\([^]]*\)\].*/\1/p' "$REPO/configure.ac")
[ -n "$VERSION" ] || { echo "Could not extract version from configure.ac" >&2; exit 1; }
BUNDLE_ID="com.7kfans.7kaa"

BDIR="$OUT/$NAME.app"
CDIR="$BDIR/Contents"
XDIR="$CDIR/MacOS"
FDIR="$CDIR/Frameworks"
RDIR="$CDIR/Resources"
EXEC="$XDIR/7kaa"
ICNS="$RDIR/7kaa.icns"

# Abort early if a required tool is missing
require() {
  command -v "$1" > /dev/null 2>&1 || { echo "Required tool not found: $1" >&2; exit 1; }
}

# Xcode Command Line Tools
require make
require strip
require otool
require install_name_tool
require codesign

# macOS built-ins
require sips
require iconutil

# Homebrew tools
require pkg-config

# Homebrew libraries
pkg-config --exists sdl2 \
  || { echo "sdl2 not found: brew install sdl2" >&2; exit 1; }
PKG_CONFIG_PATH="/opt/homebrew/opt/openal-soft/lib/pkgconfig:$PKG_CONFIG_PATH" \
  pkg-config --exists openal \
  || { echo "openal-soft not found: brew install openal-soft" >&2; exit 1; }
[ -f /opt/homebrew/lib/libintl.dylib ] \
  || { echo "gettext not found: brew install gettext" >&2; exit 1; }

if [ ! -d "$DATA" ]; then
  echo "Data directory not found: $DATA" >&2
  exit 1
fi

mkdir -p "$REPO/build"
cd "$REPO/build" || exit 1

if [ "$WITH_MULTIPLAYER" = yes ]; then

  # -------------------------------------------------------------------------
  # MULTIPLAYER WORKAROUND — LAN only, online lobby intentionally disabled
  #
  # configure.ac requires fpu_type=387 (x86 only) to enable multiplayer.
  # arm64 lacks that mode, so this build could desync against x86 clients on
  # 7kfans.com.  libcurl is left out to prevent that; online connect attempts
  # show: "Unable to connect to the 7kfans service. (No libcurl)"
  # -------------------------------------------------------------------------

  pkg-config --exists libenet \
    || { echo "enet not found: brew install enet" >&2; exit 1; }

  # Configure without disabling multiplayer/enet (curl stays disabled)
  PKG_CONFIG_PATH="/opt/homebrew/opt/openal-soft/lib/pkgconfig:$PKG_CONFIG_PATH" \
      LDFLAGS="-L/opt/homebrew/lib" \
      CPPFLAGS="-I/opt/homebrew/include" \
      ../configure

  # Patch include/config.h: remove DISABLE_MULTI_PLAYER, enable USE_ENET
  sed -i '' \
      '/\/\* Define to 1 if the multiplayer menu option is disabled \*\//{N;d;}' \
      include/config.h
  sed -i '' \
      's|/\* #undef USE_ENET \*/|#define USE_ENET 1|' \
      include/config.h

  # Patch src/Makefile: add -lenet to LIBS
  sed -i '' \
      '/^LIBS = / s/$/ -lenet/' \
      src/Makefile

else
  PKG_CONFIG_PATH="/opt/homebrew/opt/openal-soft/lib/pkgconfig:$PKG_CONFIG_PATH" \
      LDFLAGS="-L/opt/homebrew/lib" \
      CPPFLAGS="-I/opt/homebrew/include" \
      ../configure --disable-enet --disable-multiplayer
fi

make -j"$(sysctl -n hw.ncpu)"
"$STRIP" src/7kaa
make install DESTDIR="$REPO/build/dest" localedir=/locale

cd "$REPO" || exit 1

# Recursively bundle all non-system dylibs referenced by FILE into Frameworks/
install_dyn_libs() {
  local FILE="$1"
  otool -L "$FILE" \
    | grep -v '^\S' \
    | grep -v '^\s*/usr/lib/' \
    | grep -v '^\s*/System/' \
    | grep -v '^\s*@' \
    | sed 's/^[^/]*\([^ ]*\) (.*$/\1/' \
    | while IFS= read -r LIB; do
        install_dyn_lib "$FILE" "$LIB" || return 1
      done
}

# Copy a single dylib into Frameworks/, rewrite its install name, and re-sign it
install_dyn_lib() {
  local PARENT="$1"
  local LIB="$2"
  local BN
  BN="$(basename "$LIB")"
  local DN="$FDIR/$BN"
  local RN="@executable_path/../Frameworks/$BN"

  install_name_tool -change "$LIB" "$RN" "$PARENT"

  if [ -f "$DN" ]; then
    return 0
  fi

  install -m 0644 "$LIB" "$DN"
  install_name_tool -id "$RN" "$DN"
  install_dyn_libs "$DN"
}

echo "Removing old bundle..."
rm -rf "$BDIR"

echo "Creating bundle structure..."
mkdir -p "$XDIR" "$FDIR" "$RDIR"

echo "Writing Info.plist..."
sed -e "s|@VERSION@|$VERSION|g" -e "s|@BUNDLE_ID@|$BUNDLE_ID|g" \
  "$REPO/packaging/mac/Info.plist.in" > "$CDIR/Info.plist"

# Convert ICON_SRC to a multi-resolution .icns file using an iconset
echo "Building icon..."
TMPICONSET=$(mktemp -d)
ICONSET="$TMPICONSET/7kaa.iconset"
mkdir -p "$ICONSET"
for SIZE in 16 32 64 128 256 512 1024; do
  sips -z "$SIZE" "$SIZE" -s format png "$ICON_SRC" \
    --out "$ICONSET/icon_${SIZE}x${SIZE}.png" \
    > /dev/null 2>&1
done
# Rename to the expected iconset naming convention
mv "$ICONSET/icon_32x32.png"     "$ICONSET/icon_16x16@2x.png"
cp "$ICONSET/icon_16x16@2x.png"  "$ICONSET/icon_32x32.png"
mv "$ICONSET/icon_64x64.png"     "$ICONSET/icon_32x32@2x.png"
cp "$ICONSET/icon_256x256.png"   "$ICONSET/icon_128x128@2x.png"
cp "$ICONSET/icon_512x512.png"   "$ICONSET/icon_256x256@2x.png"
mv "$ICONSET/icon_1024x1024.png" "$ICONSET/icon_512x512@2x.png"
iconutil --convert icns "$ICONSET" --output "$ICNS"
rm -rf "$TMPICONSET"

echo "Copying game data..."
for DIR in "$DATA"/*/; do
  cp -R "${DIR%/}" "$RDIR/"
done
install -m 0644 "$REPO/COPYING" "$RDIR/COPYING.txt"
install -m 0644 "$REPO/README" "$RDIR/README.txt"
if [ -d "$REPO/build/dest/locale" ]; then
  cp -R "$REPO/build/dest/locale" "$RDIR/"
fi
find "$RDIR" -type d -exec chmod 755 {} \;
find "$RDIR" -type f -exec chmod 644 {} \;

echo "Installing binary..."
install -m 0755 "$BIN" "$EXEC"

echo "Bundling dynamic libraries..."
install_dyn_libs "$EXEC"

echo "Ad-hoc signing bundle..."
for f in "$FDIR"/*.dylib; do
  codesign --sign - --force "$f"
done
codesign --sign - --force "$BDIR"

echo ""
echo "Bundle created: $BDIR"

# ---------------------------------------------------------------------------
# Create DMG installer (drag-to-Applications layout)
# ---------------------------------------------------------------------------
if [ "$WITH_DMG" = no ]; then
  exit 0
fi

echo "Creating DMG installer..."
DMG_PATH="$OUT/$NAME-$VERSION.dmg"
TMPDMG="$(mktemp -u).dmg"

# Size the writable DMG to fit the bundle plus a small margin
APP_MB=$(du -sm "$BDIR" | awk '{print $1}')
DMG_MB=$((APP_MB + 20))
hdiutil create -size "${DMG_MB}m" -volname "$NAME" -fs HFS+ "$TMPDMG"

# Mount the writable image
DEV=$(hdiutil attach -readwrite -noverify -noautoopen "$TMPDMG" \
      | grep -E '^/dev/' | awk 'NR==1{print $1}')
VOLDIR="/Volumes/$NAME"

cp -R "$BDIR" "$VOLDIR/"
ln -s /Applications "$VOLDIR/Applications"

# Lay out the window: app icon on the left, Applications arrow on the right
osascript << APPLESCRIPT
tell application "Finder"
  tell disk "$NAME"
    open
    set current view of container window to icon view
    set toolbar visible of container window to false
    set statusbar visible of container window to false
    set bounds of container window to {400, 100, 870, 410}
    set theViewOptions to icon view options of container window
    set arrangement of theViewOptions to not arranged
    set icon size of theViewOptions to 128
    set position of item "$NAME.app" of container window to {135, 155}
    set position of item "Applications" of container window to {345, 155}
    close
    open
    update without registering applications
    delay 2
    close
  end tell
end tell
APPLESCRIPT

sync
hdiutil detach "$DEV"

# Convert to a compressed, read-only DMG
rm -f "$DMG_PATH"
hdiutil convert "$TMPDMG" -format UDZO -imagekey zlib-level=9 -o "$DMG_PATH"
rm -f "$TMPDMG"

echo "DMG created:    $DMG_PATH"
