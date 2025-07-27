#!/bin/bash

# This is a basic build script for building on linux. Note that the
# prefix is overridden to make this a relocatable package. There
# will need to be some kind of shortcut installed which can be done
# when this is turned into a more formal installer

# The build system will need the following shared libs, plus enet as
# a static lib. The end users will need the libraries listed provided
# by their distribution.

# libSDL2-2.0.so.0
# libopenal.so.1
# libcurl.so.4
# libstdc++.so.6
# libm.so.6
# libgcc_s.so.1
# libc.so.6

#HOST=i686-w64-mingw32.shared
#HOST_OPT=--host=$HOST
#STRIP=${HOST}-strip
STRIP=strip

ENET_LIBS="-l:libenet.a" ./configure --disable-fhs $HOST_OPT
#make -j6 pkgdatadir="" localedir=locale
echo "#define PACKAGE_DATA_WITH_EXE 1" >> include/config.h
make -j6
make install DESTDIR=`pwd`/dest bindir=/ docdir=/ pkgdatadir=/ localedir=/locale
$STRIP dest/7kaa

cd doc
latexmk
cd ..
cp doc/main.pdf dest/7kaa-manual.pdf

# Still need to add music
