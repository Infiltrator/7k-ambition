/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2025 Tim Sviridov
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

#include <stdexcept>

#include "ConfigAdv.h"
#include "gettext.h"
#include "KEY.h"
#include "OBUTT3D.h"
#include "OFONT.h"
#include "OIMGRES.h"
#include "OMOUSE.h"
#include "OMOUSECR.h"
#include "OSYS.h"
#include "vga_util.h"

#include "ambition/Ambition_vga.hh"

namespace Ambition {

void runModeSelectionScreen();

/** The global Ambition Config. */
Config config;

std::string
Config::modeString(
) const {
  switch (_mode) {
  case Mode::Classic:
	 return "classic";
  case Mode::Enhanced:
	 return "enhanced";
  case Mode::Advanced:
	 return "advanced";
  case Mode::Ambition:
	 return "ambition";
  }

  return "ERROR:UNKNOWN";
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
  // 	 throw std::domain_error("The current state does not allow mode transitions.");
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
    (char*)"Current mode:"
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


/* Private functions. */

void runModeSelectionScreen(
) {
  constexpr auto BROWSE_X1 = 30;
  constexpr auto BROWSE_Y1 = 336;
  constexpr auto SLOT_WIDTH  = 725;
  constexpr auto SLOT_HEIGHT = 44;
  constexpr auto BROWSE_X2 = BROWSE_X1 + SLOT_WIDTH - 1;

  constexpr auto TEXT_AREA_X1 = 40;
  constexpr auto TEXT_AREA_Y1 = 198;
  constexpr auto TEXT_AREA_X2 = TEXT_AREA_X1 + 728;
  constexpr auto TEXT_AREA_Y2 = TEXT_AREA_Y1 + 62;

  constexpr auto TEXT_X = BROWSE_X1 + 11;
  constexpr auto TEXT_OFFSET_Y = 9;

  constexpr auto SLOT_COUNT = 4;
  constexpr auto MODE_SELECTION_COUNT = 2;

  constexpr auto BUTTON_TOP = 529;
  constexpr auto START_BUTTON_X = 170;
  constexpr auto BACK_BUTTON_X = 465;

  struct ModeText {
    const char* name;
    const char* description;
  };

  constexpr ModeText modeText[MODE_SELECTION_COUNT] = {
    {
      .name = "1. Classic",
      .description = "Run everything the exact same as 7Kfans' 7kaa.  Allows"
      " for multiplayer with 7K:AA.",
    },
    {
      .name = "2. Enhanced",
      .description = "Add enhancements that do not affect gameplay.  Allows for"
      " multiplayer with 7K:AA.",
    },
  };

  mouse_cursor.set_icon(CURSOR_NORMAL);

  auto selectedMode = (size_t)config.currentMode();

  Button3D startButton;
  Button3D backButton;

  startButton.create(START_BUTTON_X, BUTTON_TOP, "START-U", "START-D", 1, 0);
  backButton.create(BACK_BUTTON_X, BUTTON_TOP, "RETURN-U", "RETURN-D", 1, 0);

  auto refreshFlag = 1;

  while (true) {
    if (refreshFlag) {
      mouse.hide();

      image_interface.put_front(0, 0, "SCENARIO");
      image_interface.put_back(0, 0, "TUTORIAL");
      vga_util.blt_buf(0, 499, 799, 599);

      startButton.paint();
      backButton.paint();

      mouse.show();

      constexpr auto LINE_SPACING = 4;

      font_std.put_paragraph(
        TEXT_AREA_X1,
        TEXT_AREA_Y1,
        TEXT_AREA_X2,
        TEXT_AREA_Y2,
        _(modeText[selectedMode].description),
        LINE_SPACING
      );

      for (auto slot = 0; slot < SLOT_COUNT; slot++) {
        const auto y1 = BROWSE_Y1 + slot * SLOT_HEIGHT;
        const auto y2 = y1 + SLOT_HEIGHT - 1;

        if (slot < MODE_SELECTION_COUNT) {
          font_bible.put(
            TEXT_X,
            y1 + TEXT_OFFSET_Y,
            _(modeText[slot].name),
            0,
            BROWSE_X2
          );

          if (slot == selectedMode) {
            vga_front.adjust_brightness(BROWSE_X1, y1, BROWSE_X2, y2, -2);
          }
        }
      }

      refreshFlag = 0;
    }

    sys.blt_virtual_buf();

    sys.yield();
    vga.flip();

    mouse.get_event();

    if (mouse.single_click(
          BROWSE_X1,
          BROWSE_Y1,
          BROWSE_X1 + SLOT_WIDTH - 1,
          BROWSE_Y1 + SLOT_HEIGHT * SLOT_COUNT - 1
        )
    ) { // Clicking on a mode.
      const auto clickedSlot = (mouse.click_y(LEFT_BUTTON) - BROWSE_Y1) / SLOT_HEIGHT;
      if (clickedSlot < MODE_SELECTION_COUNT) {
        if (selectedMode != clickedSlot) {
          refreshFlag = 1;
          selectedMode = clickedSlot;
        }
      }
    } else if (backButton.detect(KEY_ESC)
      || mouse.any_click(RIGHT_BUTTON)
      || sys.signal_exit_flag == 1
    ) { // Clicking on the back button, pressing Esc, or otherwise exiting.
      break;
    } else if (startButton.detect()) {
      config.setMode(Config::Mode(selectedMode));
      break;
    }

    if (!refreshFlag) {
      delayFrame();
    }
  }
}

} // namespace Ambition
