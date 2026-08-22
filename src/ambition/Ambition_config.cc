/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2025–26 Tim Sviridov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 *
 * Implementation file for Ambition::Config.
 */

#include "Ambition_config.hh"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#define _AMBITION_IMPLEMENTATION
#include "pragma_silence_7kaa_warnings.hh"
#include "ConfigAdv.h"
#include "gettext.h"
#include "KEY.h"
#include "OBOX.h"
#include "OBUTT3D.h"
#include "OCONFIG.h"
#include "OFIRM.h"
#include "OFONT.h"
#include "OHELP.h"
#include "OIMGRES.h"
#include "OMOUSE.h"
#include "OMOUSECR.h"
#include "OSLIDCUS.h"
#include "OSYS.h"
#include "vga_util.h"
#pragma GCC diagnostic pop

#include "7kaaInterface/input.hh"
#include "Ambition_input.hh"
#include "Ambition_user_interface.hh"
#include "Ambition_vga.hh"
#include "format.hh"
#include "utility.hh"


using namespace std::literals::string_literals;


namespace Ambition {

const std::vector <std::string> localeCodes = {
  "system",
  "ca",
  "de",
  "en",
  "eo",
  "es",
  "fr",
  "la",
  "nl",
  "pl",
  "pt_BR",
  "ru",
};

std::vector<int> _7kaaConfigErrorLineNumbers;
std::map<std::string, std::optional<std::string>> pendingSettings;

/** The global Ambition Config. */
Config config;


std::string localeDescription(
  const std::string localeCode
);

void runModeSelectionScreen();
int runSelectionScreen(
  const std::string heading,
  const std::vector<std::pair<std::string, std::string>> options,
  const int initialSelection,
  const bool showDescription = true
);

bool setLocaleForNextBoot(
  const std::string locale
);


std::string
Config::modeString(
) const {
  switch (_mode) {
  case Mode::Classic:
    // TRANSLATORS: The name of the running mode.
   return _("classic");
  case Mode::Enhanced:
    // TRANSLATORS: The name of the running mode.
   return _("enhanced");
  case Mode::Advanced:
    // TRANSLATORS: The name of the running mode.
    return _("advanced");
  case Mode::Ambition:
    // TRANSLATORS: The name of the running mode.
    return _("ambition");
  }

  return _("ERROR:UNKNOWN");
}

bool
Config::compatibleWith7kaa(
) const {
  return _mode <= Mode::Enhanced;
};

bool
Config::enhancementsAvailable(
) const {
  return _mode >= Mode::Enhanced;
}

void Config::setMode(
  const Mode mode
) {
  // if(!currentState.allowsModeTransitions()) {
  //   throw std::domain_error("The current state does not allow mode transitions.");
  // }

  _mode = mode;

  if (!compatibleWith7kaa()) {
    config_adv.flags |= ConfigAdv::FLAG_NOT_7KAA_COMPATIBLE;
  } else {
    config_adv.flags &= ~ConfigAdv::FLAG_NOT_7KAA_COMPATIBLE;
  }
}

Config::Mode Config::currentMode(
) const {
  return _mode;
}


constexpr auto MODE_INFORMATION_TOP = 6;
constexpr auto MODE_INFORMATION_RIGHT = 794;
constexpr auto CLICK_DETECTION_AREA_TOP = MODE_INFORMATION_TOP + 26;
constexpr auto MODE_INFORMATION_WIDTH = 44;

bool detectModeSelectClick(
) {
  constexpr auto HEIGHT = 42;

  if (mouse.single_click(
        MODE_INFORMATION_RIGHT - MODE_INFORMATION_WIDTH,
        CLICK_DETECTION_AREA_TOP,
        MODE_INFORMATION_RIGHT,
        CLICK_DETECTION_AREA_TOP + HEIGHT
      )
  ) {
    runModeSelectionScreen();
    return true;
  }

  return false;
}

void drawModeInformation(
) {
  constexpr auto HEIGHT = 38;

  font_news.right_put(
    MODE_INFORMATION_RIGHT,
    MODE_INFORMATION_TOP,
    _("Current mode:")
  );
  font_news.right_put(
    MODE_INFORMATION_RIGHT,
    MODE_INFORMATION_TOP + 12,
    (char*)(config.modeString()).c_str()
  );

  const auto buttonBitmap = image_button.get_ptr("U_MANU");
  mouse.hide_area(
    MODE_INFORMATION_RIGHT - MODE_INFORMATION_WIDTH,
    CLICK_DETECTION_AREA_TOP,
    MODE_INFORMATION_RIGHT,
    CLICK_DETECTION_AREA_TOP + HEIGHT
  );
  vga_front.put_bitmap_trans_decompress(
    MODE_INFORMATION_RIGHT - MODE_INFORMATION_WIDTH,
    MODE_INFORMATION_TOP + 28,
    buttonBitmap
  );
  mouse.show_area();
}

void report7kaaConfigLoadingErrors(
  const char* filename
) {
  if (_7kaaConfigErrorLineNumbers.empty()) {
    return;
  }

  FilePath fullPath(sys.dir_config);
  fullPath += filename;

  String report;
  report.catf(_("Error in %s at lines:\n"), fullPath.str_buf);
  for (const auto lineNumber : _7kaaConfigErrorLineNumbers) {
    // TRANSLATORS: The separator for config.txt file error line numbers.
    report.catf(_("%d, "), lineNumber);
  }
  report += "\n";
  report += _(
    "Seven Kingdoms: Ambition will continue loading using the valid lines only."
  );

  sys.show_error_dialog(report);

  _7kaaConfigErrorLineNumbers.clear();
}

void runLocaleSelectionScreen(
) {
  const std::string currentLocale = config_adv.locale;
  auto currentLocaleIndex = 0;
  for(auto i = 0u; i < localeCodes.size(); i++) {
    if(currentLocale.find(localeCodes[i]) == 0) {
      currentLocaleIndex = i;
      break;
    }
  }

  const auto selection = runSelectionScreen(
    _("Select Locale"),
    Utility::map_to<std::vector>(
      localeCodes,
      [](const auto& localeCode) {
        return std::pair(localeDescription(localeCode), ""s);
      }
    ),
    currentLocaleIndex,
    false
  );

  if (selection < 0) {
    return;
  }

  if (setLocaleForNextBoot(localeCodes[selection])
    && box.ask(
      _("The locale setting has been saved to your config.txt file and will"
        " take effect the next time that Seven Kingdoms: Ambition starts."
        "\nWould you like to exit now?"
      ),
      _("Yes"),
      _("No")
    )
  ) {
    sys.signal_exit_flag = 1;
  }
}

void set7kaaConfigOption(
  char* key,
  char* value,
  const int lineNumber
) {
  const auto success = config_adv.set(key, value);

  if (!success) {
    _7kaaConfigErrorLineNumbers.push_back(lineNumber);
  }
}

void setSetting(
  const std::string setting,
  const std::string value
) {
  pendingSettings[setting] = value;
}

bool shouldDrawFirmHitBar(
  const Firm* _7kaaFirm
) {
  switch (::config.help_mode) {
  case NO_HELP:
    return false;

  case BRIEF_HELP: {
    const auto buildingArea = UserInterface::Rectangle::fromWorldRectangle(
      Coordinates::Rectangle::from7kaaRectangle(
        { .x = _7kaaFirm->loc_x1, .y = _7kaaFirm->loc_y1 },
        { .x = _7kaaFirm->loc_x2, .y = _7kaaFirm->loc_y2 }
      )
    );
    return (
      firm_array.selected_recno == _7kaaFirm->firm_recno
      || UserInterface::mouseCursorInArea(buildingArea)
    );
  }

  case DETAIL_HELP:
    return _7kaaFirm->hit_points < _7kaaFirm->max_hit_points;

  default:
    assert(false);
  }
}

void unsetSetting(
  const std::string setting
) {
  pendingSettings[setting] = {};
}

bool updateSettingsFile(
) {
  if (pendingSettings.empty()) {
    return false;
  }

  constexpr auto AUTO_COMMENT = "# auto";

  std::string buffer;

  const auto filename = DirectoryPath::config() / "config.txt";
  if(std::filesystem::exists(filename)) {
    std::fstream configFile(filename);
    if (!configFile.good()) {
      box.msg(
        format(
          "%s %s",
          _("Error opening config.txt file for reading:"),
          std::strerror(errno)
        ).c_str()
      );
      return false;
    }

    while (!configFile.eof()) {
      const auto line = Utility::getline(configFile);

      if (line == AUTO_COMMENT) {
        const auto nextLine = Utility::getline(configFile);
        const auto key = Utility::trim(Utility::split(nextLine, '=')[0]);
        if (pendingSettings.contains(key)) {
          continue;
        }

        buffer += AUTO_COMMENT + "\n"s;
      }

      const auto lookat = (line.size() >= 9 && line.substr(0, 9) == "#ambition")
        ? line.substr(9)
        : line;
      if (lookat.size() >= 1 && Utility::trim(lookat).substr(0, 1) != "#") {
        const auto key = Utility::trim(Utility::split(lookat, '=')[0]);
        if (pendingSettings.contains(key) && !pendingSettings[key].has_value()) {
          buffer += "# " + line + "\n";
          continue;
        }
      }

      if (!configFile.eof() || line != "") {
        buffer += line + "\n";
      }
    }

    buffer = buffer.substr(0, buffer.find_last_not_of("\n") + 1);

    if (!buffer.empty()) {
      buffer += "\n";
      if (std::any_of(
          pendingSettings.cbegin(),
          pendingSettings.cend(),
          [](const auto& pair) { return pair.second.has_value(); }
        )
      ) {
        buffer += "\n";
      }
    }
  }

  for (const auto& pendingSetting : pendingSettings) {
    if (!pendingSetting.second.has_value()) {
      continue;
    }

    buffer += AUTO_COMMENT + "\n"s
      + pendingSetting.first + " = " + pendingSetting.second.value() + "\n";
  }

  std::fstream configFile(filename, std::ios_base::out | std::ios_base::trunc);
  if (!configFile.good()) {
    box.msg(
      format(
        "%s %s",
        _("Error opening config.txt file for writing:"),
        std::strerror(errno)
      ).c_str()
    );
    return false;
  }
  configFile << buffer;
  if (!configFile.good()) {
    box.msg(
      format(
        "%s %s",
        _("Error writing to config.txt file:"),
        std::strerror(errno)
      ).c_str()
    );
    return false;
  }

  return true;
}


namespace DirectoryPath {

std::filesystem::path config(
) {
  constexpr auto _7KAA_DIRECTORY = "7kaa";

  /* When we read 7kaa's config.txt, we need to read from the base directory
     instead. */
  const auto path = std::filesystem::path(sys.dir_config);
  if (path.parent_path().filename() == _7KAA_DIRECTORY
    || path.filename() == _7KAA_DIRECTORY
  ) {
    return std::filesystem::path(sys.dir_config);
  }

  return path / "config/";
}

std::filesystem::path multiplayerSave(
) {
  return std::filesystem::path(sys.dir_config) / "save" / "multiplayer/";
}

std::filesystem::path save(
  const std::string filename
) {
  const auto extension = std::filesystem::path(filename).extension();

  if (extension == ".SAV") {
    return singleplayerSave();
  } else {
    return multiplayerSave();
  }
}

std::filesystem::path screenshot(
) {
  return std::filesystem::path(sys.dir_config) / "screenshot/";
}

std::filesystem::path singleplayerSave(
) {
  return std::filesystem::path(sys.dir_config) / "save" / "singleplayer/";
}

} // namespace Ambition::DirectoryPath


/* Private functions. */

std::string localeDescription(
  const std::string localeCode
) {
  static const std::map<
    std::string,
    std::pair<std::string, std::string>
  > localeNames = {
    { "system", { "detect", _("Detect System Locale") } },
    { "ca", { "Català", _("Catalan") } },
    { "de", { "Deutsch", _("German") } },
    { "en", { "English", _("English") } },
    { "eo", { "Esperanto", _("Esperanto") } },
    { "es", { "Español", _("Spanish") } },
    { "fr", { "Français", _("French") } },
    { "la", { "Latinum", _("Latin") } },
    { "nl", { "Nederlands", _("Dutch") } },
    { "pl", { "Polski", _("Polish") } },
    { "pt_BR", { "Português (Brasil)", _("Portuguese (Brazil)") } },
    { "ru", { "Русский [Pycckuû]", _("Russian") } },
  };

  if (!localeNames.contains(localeCode)) {
    return localeCode;
  }

  return (
    localeCode
    + " - " + localeNames.at(localeCode).first
    + " - " + localeNames.at(localeCode).second
  );
}

bool setLocaleForNextBoot(
  const std::string locale
) {
  if (locale == "system") {
    return deleteSetting("locale");
  } else {
    return saveSetting("locale", locale);
  }
}

void detectScroll(
  const int minimumRecordNumber,
  const int size,
  int& browseRecordNumber,
  SlideVBar& scrollBar,
  int& refreshFlag
) {
  constexpr auto SLOT_COUNT = 9;

  Input::detectScroll(
    false,
    UserInterface::BOUNDS,
    Input::STANDARD_ACTIVATIONS,
    {
      {
        Input::ScrollOrientation::Vertical,
        [
          &browseRecordNumber,
          &refreshFlag,
          &scrollBar,
          minimumRecordNumber,
          size
        ] (
          const int amount
        ) {
          browseRecordNumber = std::clamp(
            browseRecordNumber + amount,
            minimumRecordNumber,
            size - 1
          );

          if (browseRecordNumber - scrollBar.view_recno < 0) {
            scrollBar.set_view_recno(browseRecordNumber);
          }
          if (browseRecordNumber - scrollBar.view_recno >= SLOT_COUNT) {
            scrollBar.set_view_recno(browseRecordNumber - (SLOT_COUNT - 1));
          }

          refreshFlag = 1;
        }
      }
    },
    []() { return SLOT_COUNT - 1; }
  );

  Ambition::Input::detectScroll(
    true,
    UserInterface::BOUNDS,
    { },
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        [ &scrollBar, &refreshFlag ] (
          const int amount
        ) {
          scrollBar.set_view_recno(scrollBar.view_recno + amount);
          refreshFlag = 1;
        },
      },
    },
    []() { return SLOT_COUNT - 1; }
  );
}

void runModeSelectionScreen(
) {
  const auto selection = runSelectionScreen(
    _("Select Mode"),
    {
      {
        // TRANSLATORS: The name of the running mode.
        _("I. Classic"),
        _("Run everything the exact same as 7Kfans' 7kaa.  Allows for"
          " multiplayer with 7K:AA."
        ),
      },
      {
        // TRANSLATORS: The name of the running mode.
        _("II. Enhanced"),
        _("Add enhancements that do not affect gameplay.  Allows for"
          " multiplayer with 7K:AA."
        ),
      },
    },
    static_cast<int>(config.currentMode())
  );

  if (selection < 0) {
    return;
  }

  config.setMode(Config::Mode(selection));
}

int runSelectionScreen(
  const std::string heading,
  const std::vector<std::pair<std::string, std::string>> options,
  const int initialSelection,
  const bool showDescriptions
) {
  constexpr auto SLOT_HEIGHT = 44;

  const auto slotCount = showDescriptions ? 4 : 9;

  mouse_cursor.set_icon(CURSOR_NORMAL);

  auto selected = initialSelection;

  if (showDescriptions) {
    image_interface.put_front(0, 0, "SCENARIO");
    image_interface.put_back(0, 0, "TUTORIAL");
    copyBackBufferToFront(UserInterface::SelectionListScreen::Buttons::SECTION);
  } else {
    image_interface.put_front(0, 0, "LONGLIST");
  }
  copyFrontBufferToBack(UserInterface::BOUNDS);

  Button3D descriptionScrollUpButton;
  Button3D descriptionScrollDownButton;
  Button3D listScrollUpButton;
  Button3D listScrollDownButton;

  const auto descriptionScrollUpButtonLocation
    = UserInterface::SelectionListScreen::Description::SCROLLBAR.internal(
      UserInterface::SCROLL_BUTTON_SIZE,
      UserInterface::HorizontalAlignment::Centre,
      UserInterface::VerticalAlignment::Top
    );
  const auto descriptionScrollDownButtonLocation
    = UserInterface::SelectionListScreen::Description::SCROLLBAR.internal(
      UserInterface::SCROLL_BUTTON_SIZE,
      UserInterface::HorizontalAlignment::Centre,
      UserInterface::VerticalAlignment::Bottom
    );
  UserInterface::initScrollButton(
    descriptionScrollUpButton,
    descriptionScrollUpButtonLocation,
    UserInterface::ScrollButtonDirection::Up
  );
  UserInterface::initScrollButton(
    descriptionScrollDownButton,
    descriptionScrollDownButtonLocation,
    UserInterface::ScrollButtonDirection::Down
  );

  const auto listScrollbarArea = showDescriptions
    ? UserInterface::SelectionListScreen::List::SCROLLBAR
    : UserInterface::SelectionListScreen::LongList::SCROLLBAR;
  const auto listScrollUpButtonLocation = listScrollbarArea.internal(
    UserInterface::SCROLL_BUTTON_SIZE,
    UserInterface::HorizontalAlignment::Centre,
    UserInterface::VerticalAlignment::Top
  );
  const auto listScrollDownButtonLocation = listScrollbarArea.internal(
    UserInterface::SCROLL_BUTTON_SIZE,
    UserInterface::HorizontalAlignment::Centre,
    UserInterface::VerticalAlignment::Bottom
  );
  UserInterface::initScrollButton(
    listScrollUpButton,
    listScrollUpButtonLocation,
    UserInterface::ScrollButtonDirection::Up
  );
  UserInterface::initScrollButton(
    listScrollDownButton,
    listScrollDownButtonLocation,
    UserInterface::ScrollButtonDirection::Down
  );

  constexpr auto LINE_SPACING = 4;
  const auto LINES_IN_DESCRIPTION_TEXT_AREA
    = (UserInterface::SelectionListScreen::Description::TEXT.height()
      + LINE_SPACING)
    / (font_std.font_height + LINE_SPACING);

  SlideVBar descriptionScrollbar;
  const auto descriptionScrollbarTrough
    = UserInterface::SelectionListScreen::Description::SCROLLBAR.inner(
      {
        .top
          = static_cast<unsigned int>(UserInterface::SCROLL_BUTTON_SIZE.height),
        .bottom
          = static_cast<unsigned int>(UserInterface::SCROLL_BUTTON_SIZE.height),
      }
    );
  descriptionScrollbar.init_scroll(
    descriptionScrollbarTrough.start.left,
    descriptionScrollbarTrough.start.top,
    descriptionScrollbarTrough.end.left,
    descriptionScrollbarTrough.end.top,
    LINES_IN_DESCRIPTION_TEXT_AREA,
    UserInterface::draw7kaaScrollbar
  );

  SlideVBar listScrollbar;
  const auto listScrollbarTrough = listScrollbarArea.inner(
    {
      .top = static_cast<unsigned int>(UserInterface::SCROLL_BUTTON_SIZE.height),
      .bottom
        = static_cast<unsigned int>(UserInterface::SCROLL_BUTTON_SIZE.height),
    }
  );
  listScrollbar.init_scroll(
    listScrollbarTrough.start.left,
    listScrollbarTrough.start.top,
    listScrollbarTrough.end.left,
    listScrollbarTrough.end.top,
    slotCount,
    UserInterface::draw7kaaScrollbar
  );
  listScrollbar.set(0, options.size() - 1, 0);
  listScrollbar.set_view_recno(selected - slotCount / 2);

  constexpr auto ELASTIC = 1;
  constexpr auto IS_NOT_PUSHED = 0;

  Button3D startButton;
  Button3D backButton;

  startButton.create(
    UserInterface::SelectionListScreen::Buttons::START_BUTTON.start.left,
    UserInterface::SelectionListScreen::Buttons::START_BUTTON.start.top,
    "START-U",
    "START-D",
    ELASTIC,
    IS_NOT_PUSHED
  );
  backButton.create(
    UserInterface::SelectionListScreen::Buttons::BACK_BUTTON.start.left,
    UserInterface::SelectionListScreen::Buttons::BACK_BUTTON.start.top,
    "RETURN-U",
    "RETURN-D",
    ELASTIC,
    IS_NOT_PUSHED
  );

  const auto slotsArea = showDescriptions
    ? UserInterface::SelectionListScreen::List::SLOT_AREA
    : UserInterface::SelectionListScreen::LongList::SLOT_AREA;

  auto refreshFlag = 1;
  auto resetDescriptionScroll = true;

  while (true) {
    if (refreshFlag) {
      mouse.hide();

      printText(
        font_bard,
        heading,
        UserInterface::SelectionListScreen::Heading::TEXT,
        UserInterface::Clear::EntireArea,
        UserInterface::HorizontalAlignment::Centre,
        UserInterface::VerticalAlignment::Centre
      );

      if (showDescriptions) {
        printText(
          font_std,
          options[selected].second,
          UserInterface::SelectionListScreen::Description::TEXT,
          UserInterface::Clear::EntireArea
        );

        if (resetDescriptionScroll) {
          int linesThatFitCount;
          int totalLineCount;
          font_std.count_line(
            UserInterface::SelectionListScreen::Description::TEXT.start.left,
            UserInterface::SelectionListScreen::Description::TEXT.start.top,
            UserInterface::SelectionListScreen::Description::TEXT.end.left,
            UserInterface::SelectionListScreen::Description::TEXT.end.top,
            options[selected].second.c_str(),
            LINE_SPACING,
            linesThatFitCount,
            totalLineCount
          );

          descriptionScrollbar.set(0, totalLineCount - 1, 0);
          resetDescriptionScroll = false;
        }
        descriptionScrollbar.paint();

        descriptionScrollUpButton.paint();
        descriptionScrollDownButton.paint();

        UserInterface::printParagraph(
          font_std,
          options[selected].second,
          UserInterface::SelectionListScreen::Description::TEXT,
          LINE_SPACING,
          UserInterface::Clear::EntireArea,
          UserInterface::HorizontalAlignment::Left,
          UserInterface::VerticalAlignment::Top,
          descriptionScrollbar.view_recno
        );
      }

      for (auto slot = 0; slot < slotCount; slot++) {
        const auto slotArea
          = slotsArea
          .inner({ top: static_cast<unsigned int>(slot * SLOT_HEIGHT) })
          .internal({ height: SLOT_HEIGHT });

        if (static_cast<unsigned int>(listScrollbar.view_recno + slot)
          < options.size()
        ) {
          printText(
            font_bible,
            options[listScrollbar.view_recno + slot].first,
            slotArea,
            UserInterface::Clear::EntireArea,
            UserInterface::HorizontalAlignment::Centre,
            UserInterface::VerticalAlignment::Centre
          );

          if (listScrollbar.view_recno + slot == selected) {
            vga_front.adjust_brightness(
              slotArea.start.left,
              slotArea.start.top,
              slotArea.end.left,
              slotArea.end.top,
              -2
            );
          }
        }
      }

      mouse.show();

      listScrollbar.paint();

      listScrollUpButton.paint();
      listScrollDownButton.paint();

      startButton.paint();
      backButton.paint();

      refreshFlag = 0;
    }

    sys.yield();

    if (showDescriptions) {
      _7kaaAmbitionInterface::Input::detectScenarioScroll(
        0,
        options.size() - 1,
        selected,
        listScrollbar,
        descriptionScrollbar,
        refreshFlag
      );
    } else {
      detectScroll(0, options.size(), selected, listScrollbar, refreshFlag);
    }

    mouse.get_event();

    if (descriptionScrollbar.detect() == 1) {
      refreshFlag = 1;
    } else if (descriptionScrollUpButton.detect()) {
      descriptionScrollbar.set_view_recno(descriptionScrollbar.view_recno - 1);
      refreshFlag = 1;
    } else if (descriptionScrollDownButton.detect()) {
      descriptionScrollbar.set_view_recno(descriptionScrollbar.view_recno + 1);
      refreshFlag = 1;
    } else if (listScrollbar.detect() == 1) {
      refreshFlag = 1;
    } else if (listScrollUpButton.detect()) {
      listScrollbar.set_view_recno(listScrollbar.view_recno - 1);
      refreshFlag = 1;
    } else if (listScrollDownButton.detect()) {
      listScrollbar.set_view_recno(listScrollbar.view_recno + 1);
      refreshFlag = 1;
    } else if (UserInterface::detectMouseClick(slotsArea)) {
      const auto clickedSlot
        = (mouse.click_y(LEFT_BUTTON) - slotsArea.start.top)
        / SLOT_HEIGHT;
      const auto clickedOption = listScrollbar.view_recno + clickedSlot;
      if (static_cast<unsigned int>(clickedOption) < options.size()) {
        if (selected != clickedOption) {
          refreshFlag = 1;
          selected = clickedOption;
          resetDescriptionScroll = true;
        }
      }
    } else if (backButton.detect(GETKEY(KEYEVENT_CANCEL))
      || mouse.any_click(RIGHT_BUTTON)
      || sys.signal_exit_flag == 1
    ) { // Clicking on the back button, pressing Esc, or otherwise exiting.
      return -1;
    } else if (startButton.detect(GETKEY(KEYEVENT_CONFIRM))) {
      return selected;
    }

    sys.blt_virtual_buf();
    vga.flip();

    if (!refreshFlag) {
      delayFrame();
    }
  }
}

} // namespace Ambition
