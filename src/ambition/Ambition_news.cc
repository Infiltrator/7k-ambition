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
 * Implementation file for Ambition::News.
 */

#include "Ambition_news.hh"

#include <fstream>

#define _AMBITION_IMPLEMENTATION
#include "FilePath.h"
#include "gettext.h"
#include "KEY.h"
#include "OBUTT3D.h"
#include "OFONT.h"
#include "OIMGRES.h"
#include "OMOUSE.h"
#include "OMOUSECR.h"
#include "OSLIDCUS.h"
#include "OSYS.h"
#include "OVGA.h"
#include "vga_util.h"

#include "Ambition_user_interface.hh"
#include "Ambition_version.hh"
#include "Ambition_vga.hh"


namespace Ambition::News {

static void displaySlideBall(
  SlideVBar *slideBar,
  const int repaintBody
) {
  vga_util.blt_buf(
    slideBar->scrn_x1,
    slideBar->scrn_y1,
    slideBar->scrn_x2,
    slideBar->scrn_y2,
    0
  );

  image_interface.put_front(
    slideBar->scrn_x1,
    slideBar->rect_top(),
    "SLIDBALL"
  );
}

void display(
) {
  constexpr Ambition::UserInterface::Point START_BUTTON = {
    .left = 317,
    .top = 535,
  };
  constexpr Ambition::UserInterface::Rectangle HEADING = {
    .start = {
      .left = 300,
      .top = 25,
    },
    .end = {
      .left = 500,
      .top = 45,
    },
  };
  constexpr Ambition::UserInterface::Rectangle TEXT_BOX = {
    .start = {
      .left = 35,
      .top = 70,
    },
    .end = {
      .left = 765,
      .top = 525,
    },
  };

  constexpr auto LINE_SPACING = 4;

  constexpr auto SCROLL_BUTTON_WIDTH = 13;
  constexpr auto SCROLL_BUTTON_HEIGHT = 16;

  constexpr auto SLIDE_BALL_WIDTH = 23;

  const auto TEXT_AREA = TEXT_BOX.inner(8, 8, 8 + SCROLL_BUTTON_WIDTH);

  const auto SCROLL_BUTTON_LOCATIONS = TEXT_BOX.inner(
    0,
    0,
    SCROLL_BUTTON_WIDTH,
    SCROLL_BUTTON_HEIGHT
  );

  Button3D startButton;
  Button3D scrollUp;
  Button3D scrollDown;

  SlideVBar slideBar;

  startButton.create(
    START_BUTTON.left,
    START_BUTTON.top,
    "START-U",
    "START-D",
    1,
    0
  );
  scrollUp.create(
    SCROLL_BUTTON_LOCATIONS.end.left,
    SCROLL_BUTTON_LOCATIONS.start.top,
    "SV-UP-U",
    "SV-UP-D",
    1,
    0
  );
  scrollDown.create(
    SCROLL_BUTTON_LOCATIONS.end.left,
    SCROLL_BUTTON_LOCATIONS.end.top,
    "SV-DW-U",
    "SV-DW-D",
    1,
    0
  );

  slideBar.init_slide(
    SCROLL_BUTTON_LOCATIONS.end.left - (SLIDE_BALL_WIDTH - SCROLL_BUTTON_WIDTH) / 2,
    SCROLL_BUTTON_LOCATIONS.start.top + SCROLL_BUTTON_HEIGHT,
    SCROLL_BUTTON_LOCATIONS.end.left + SLIDE_BALL_WIDTH,
    SCROLL_BUTTON_LOCATIONS.end.top,
    SLIDE_BALL_WIDTH,
    displaySlideBall
  );

  mouse_cursor.set_icon(CURSOR_NORMAL);

  const auto totalItemCount = std::accumulate(
    versionDetails.begin(),
    versionDetails.end(),
    0,
    [](const int accumulator, const VersionUpdates& version) {
      return accumulator + version.totalItemCount();
    }
  );

  slideBar.set(0, totalItemCount - 1, 0);

  vga_util.disp_image_file("RESULTS");

  Ambition::printTextCentred(font_bard, _("WHAT'S NEW"), HEADING);

  vga_util.d3_panel_up(
    TEXT_BOX.start.left,
    TEXT_BOX.start.top,
    TEXT_BOX.end.left,
    TEXT_BOX.end.top
  );

  auto currentItem = 0;
  auto itemsToSkip = 0;

  auto refreshFlag = 1;

  while (true) {
    if (refreshFlag) {
      mouse.hide();

      vga_util.blt_buf(
        TEXT_BOX.start.left,
        TEXT_BOX.start.top,
        TEXT_BOX.end.left,
        TEXT_BOX.end.top,
        0
      );

      startButton.paint();
      scrollUp.paint();
      scrollDown.paint();

      slideBar.paint(itemsToSkip);

      mouse.show();

      auto top = TEXT_AREA.start.top;
      currentItem = 0;

      for (const auto& version : versionDetails) {
        const auto versionItemCount = version.totalItemCount();
        if (currentItem + versionItemCount - 1 < itemsToSkip) {
          currentItem += versionItemCount;
          continue;
        }

        font_bard.put_paragraph(
          TEXT_AREA.start.left,
          top,
          TEXT_AREA.end.left,
          TEXT_AREA.end.top,
          ("_" + version.identifier + "_").c_str(),
          LINE_SPACING,
          1,
          1,
          Font::CENTER_JUSTIFY
        );
        top = std::max(top, font_bard.next_text_y - font_bard.font_height);

        for (const auto& section : version.sections) {
          if (currentItem + section.items.size() - 1 < itemsToSkip) {
            currentItem += section.items.size();
            continue;
          }

          font_bard.put_paragraph(
            TEXT_AREA.start.left + 40,
            top,
            TEXT_AREA.end.left,
            TEXT_AREA.end.top,
            section.heading.c_str(),
            LINE_SPACING
          );
          top = std::max(top, font_bard.next_text_y - font_bard.font_height);

          for (const auto& item : section.items) {
            currentItem++;
            if (currentItem - 1 < itemsToSkip) {
              continue;
            }

            font_std.put_paragraph(
              TEXT_AREA.start.left,
              top,
              TEXT_AREA.end.left,
              TEXT_AREA.end.top,
              ("   - " + item).c_str(),
              LINE_SPACING
            );
            if (top >= font_std.next_text_y - font_std.font_height) {
              currentItem--;
              goto deep_break;
            }
            top = std::max(top, font_std.next_text_y - font_std.font_height);
          }
        }
      }
    deep_break:

      refreshFlag = 0;
    }

    sys.blt_virtual_buf();
    sys.yield();
    vga.flip();

    int _discard;
    int scroll;

    mouse.get_event();

    if (mouse.get_scroll(&_discard, &scroll)) {
    } else if (mouse.unique_key_code == KEY_HOME) {
      scroll = -itemsToSkip;
    } else if (mouse.unique_key_code == KEY_END) {
      scroll = totalItemCount - 1 - itemsToSkip;
    } else if (mouse.unique_key_code == KEY_DOWN) {
      scroll = 1;
    } else if (mouse.unique_key_code == KEY_UP) {
      scroll = -1;
    } else if (mouse.unique_key_code == KEY_BACK_SPACE
      || mouse.unique_key_code == KEY_PGUP
      || mouse.unique_key_code == KEY_LEFT
    ) {
      scroll = -8;
    } else if (mouse.unique_key_code == SDLK_SPACE
      || mouse.unique_key_code == KEY_PGDN
      || mouse.unique_key_code == KEY_RIGHT
    ) {
      scroll = currentItem - 1 - itemsToSkip;
    } else if (scrollDown.detect()) {
      scroll = 1;
    } else if (scrollUp.detect()) {
      scroll = -1;
    }

    if (startButton.detect(KEY_ESC, KEY_RETURN)
      || sys.signal_exit_flag == 1
    ) { // Clicking on the Start button, pressing Esc, pressing Enter, or
        // otherwise exiting.
      break;
    } else if (slideBar.detect()) {
      itemsToSkip = slideBar.view_recno;
      refreshFlag = 1;
    } else if (scroll > 0) {
      itemsToSkip += scroll;
      itemsToSkip = std::min(totalItemCount - 1, itemsToSkip);
      refreshFlag = 1;
    } else if (scroll < 0) {
      itemsToSkip += scroll;
      itemsToSkip = std::max(0, itemsToSkip);
      refreshFlag = 1;
    } else if (
      mouse.any_click(
        TEXT_AREA.start.left,
        TEXT_AREA.start.top,
        TEXT_AREA.end.left,
        TEXT_AREA.end.top,
        LEFT_BUTTON
      )
    ) {
      itemsToSkip++;
      if (itemsToSkip == totalItemCount) {
        break;
      }
      refreshFlag = 1;
    } else if (
      mouse.any_click(
        TEXT_AREA.start.left,
        TEXT_AREA.start.top,
        TEXT_AREA.end.left,
        TEXT_AREA.end.top,
        RIGHT_BUTTON
      )
    ) {
      if (itemsToSkip == 0) {
        break;
      }
      itemsToSkip--;
      refreshFlag = 1;
    }

    if (!refreshFlag) {
      delayFrame();
    }
  }
}

std::string lastDisplayedNewsVersion(
) {
  FilePath versionSaveFilePath(sys.dir_config);
  versionSaveFilePath += "last-displayed-news-version";

  std::ifstream file(versionSaveFilePath);
  if (!file.good()) {
    return "Error roading last displayed news version file";
  }

  std::string buffer;
  file >> buffer;

  return buffer;
}

void saveDisplayedNewsVersion(
) {
  FilePath versionSaveFilePath(sys.dir_config);
  versionSaveFilePath += "last-displayed-news-version";

  std::ofstream file(versionSaveFilePath);
  if (file.good()) {
    file << versionString();
  }
}

} // namespace Ambition::News
