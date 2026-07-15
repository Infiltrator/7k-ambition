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

#include <cassert>
#include <stdexcept>
#include <vector>

#define _AMBITION_IMPLEMENTATION
#include "pragma_silence_7kaa_warnings.hh"
#include "ConfigAdv.h"
#include "gettext.h"
#include "KEY.h"
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
#include "Ambition_user_interface.hh"
#include "Ambition_vga.hh"


namespace Ambition {

std::vector<int> _7kaaConfigErrorLineNumbers;

void runModeSelectionScreen();
int runSelectionScreen(
  const std::string heading,
  const std::vector<std::pair<std::string, std::string>> options,
  const int initialSelection
);


/** The global Ambition Config. */
Config config;

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
  const int initialSelection
) {
  constexpr auto SLOT_HEIGHT = 44;

  const auto slotCount = 4;

  mouse_cursor.set_icon(CURSOR_NORMAL);

  auto selected = initialSelection;

  image_interface.put_front(0, 0, "SCENARIO");
  image_interface.put_back(0, 0, "TUTORIAL");
  copyBackBufferToFront(UserInterface::SelectionListScreen::Buttons::SECTION);
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

  const auto listScrollbarArea = UserInterface::SelectionListScreen::List::SCROLLBAR;
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

  const auto slotsArea = UserInterface::SelectionListScreen::List::SLOTS;

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

      descriptionScrollUpButton.paint();
      descriptionScrollDownButton.paint();

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

    _7kaaAmbitionInterface::Input::detectScenarioScroll(
      0,
      options.size() - 1,
      selected,
      listScrollbar,
      descriptionScrollbar,
      refreshFlag
    );
    constexpr auto DESCRIPTION_UPDATED = 0x00200000;
    resetDescriptionScroll |= refreshFlag & DESCRIPTION_UPDATED;

    _7kaaAmbitionInterface::Input::detectScenarioScroll(
      0,
      options.size() - 1,
      selected,
      listScrollbar,
      descriptionScrollbar,
      refreshFlag
    );

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
