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

#include "Ambition_config.hh"
#include "Ambition_control.hh"
#include "Ambition_input.hh"
#include "Ambition_user_interface.hh"
#include "Ambition_version.hh"
#include "Ambition_vga.hh"


namespace Ambition::News {

constexpr auto LAST_DISPLAYED_NEWS_VERSION_FILENAME = "last-displayed-news-version";


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
  const auto SCROLL_PROMPTS_AREA
    = TEXT_BOX
    .outer(0, 30, 0, 26)
    .inner(470, 0, 50);
  const auto SCROLL_HELP_AREA
    = TEXT_BOX
    .outer(0, 60)
    .inner(50, 0)
    .internal(
      { .width = 225, .height = 50 },
      UserInterface::HorizontalAlignment::Left,
      UserInterface::VerticalAlignment::Bottom
    );

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

  Control::startMusic();

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

  printText(
    font_bard,
    _("WHAT'S NEW"),
    HEADING,
    UserInterface::Clear::EntireArea,
    UserInterface::HorizontalAlignment::Centre,
    UserInterface::VerticalAlignment::Centre
  );

  UserInterface::printParagraph(
    font_std,
    _(
      "Scroll using the mouse wheel, up/down keys, pg-up/pg-dwn keys, or"
      " left/right keys."
    ),
    SCROLL_HELP_AREA
  );

  vga_util.d3_panel_up(
    TEXT_BOX.start.left,
    TEXT_BOX.start.top,
    TEXT_BOX.end.left,
    TEXT_BOX.end.top
  );

  auto scrollActivations = Ambition::Input::EXTENDED_ACTIVATIONS;
  scrollActivations.push_back(
    {
      .orientation = Ambition::Input::ScrollOrientation::Vertical,
      .direction = Ambition::Input::ScrollDirection::Forward,
      .distance = Ambition::Input::ScrollStep::Single,
      ._7kaaButtonDetects = {
        [&scrollDown]() { return scrollDown.detect(); },
      },
    }
  );
  scrollActivations.push_back(
    {
      .orientation = Ambition::Input::ScrollOrientation::Vertical,
      .direction = Ambition::Input::ScrollDirection::Backward,
      .distance = Ambition::Input::ScrollStep::Single,
      .keyCodes = { KEY_UP },
      ._7kaaButtonDetects = {
        [&scrollUp]() { return scrollUp.detect(); },
      },
    }
  );
  scrollActivations.push_back(
    {
      .orientation = Ambition::Input::ScrollOrientation::Vertical,
      .direction = Ambition::Input::ScrollDirection::Forward,
      .distance = Ambition::Input::ScrollStep::Page,
      .keyCodes = { SDLK_SPACE },
    }
  );
  scrollActivations.push_back(
    {
      .orientation = Ambition::Input::ScrollOrientation::Vertical,
      .direction = Ambition::Input::ScrollDirection::Backward,
      .distance = Ambition::Input::ScrollStep::Page,
      .keyCodes = { KEY_BACK_SPACE },
    }
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

      vga_util.blt_buf(
        SCROLL_PROMPTS_AREA.start.left,
        SCROLL_PROMPTS_AREA.start.top,
        SCROLL_PROMPTS_AREA.end.left,
        SCROLL_PROMPTS_AREA.end.top,
        0
      );

      startButton.paint();
      scrollUp.paint();
      scrollDown.paint();

      slideBar.paint(itemsToSkip);

      mouse.show();

      auto top = TEXT_AREA.start.top;

      if (itemsToSkip > 0) {
        printText(
          font_bible,
          _("scroll up for more"),
          SCROLL_PROMPTS_AREA,
          UserInterface::Clear::None,
          UserInterface::HorizontalAlignment::Right,
          UserInterface::VerticalAlignment::Top
        );
      }

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
        if (top >= font_bard.next_text_y - font_bard.font_height) {
          goto deep_break;
        }
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
          if (top >= font_bard.next_text_y - font_bard.font_height) {
            goto deep_break;
          }
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
      if (currentItem < totalItemCount) {
        printText(
          font_bible,
          _("scroll down for more"),
          SCROLL_PROMPTS_AREA,
          UserInterface::Clear::None,
          UserInterface::HorizontalAlignment::Right,
          UserInterface::VerticalAlignment::Bottom
        );
      }

      refreshFlag = 0;
    }

    sys.blt_virtual_buf();
    sys.yield();
    vga.flip();

    mouse.get_event();

    Input::detectScroll(
      true,
      UserInterface::BOUNDS,
      scrollActivations,
      {
        {
          Input::ScrollOrientation::Vertical,
            [&itemsToSkip, &refreshFlag, totalItemCount](
            int amount
          ) {
            itemsToSkip += amount;
            refreshFlag = 1;
            itemsToSkip = std::max(0, std::min(totalItemCount - 1, itemsToSkip));
          },
        },
      },
      [currentItem, itemsToSkip, totalItemCount]() {
        if (totalItemCount - currentItem < 6) {
          return 6;
        }
        return currentItem - 1 - itemsToSkip;
      },
      [totalItemCount]() { return totalItemCount; }
    );

    if (startButton.detect(KEY_ESC, KEY_RETURN)
      || sys.signal_exit_flag == 1
    ) { // Clicking on the Start button, pressing Esc, pressing Enter, or
        // otherwise exiting.
      break;
    } else if (slideBar.detect()) {
      itemsToSkip = slideBar.view_recno;
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

  vga_util.finish_disp_image_file();
}

std::string lastDisplayedNewsVersion(
) {
  std::ifstream file(DirectoryPath::config() / LAST_DISPLAYED_NEWS_VERSION_FILENAME);
  if (!file.good()) {
    return "Error roading last displayed news version file";
  }

  std::string buffer;
  file >> buffer;

  return buffer;
}

void saveDisplayedNewsVersion(
) {
  std::ofstream file(DirectoryPath::config() / LAST_DISPLAYED_NEWS_VERSION_FILENAME);
  if (file.good()) {
    file << versionString();
  }
}

} // namespace Ambition::News
