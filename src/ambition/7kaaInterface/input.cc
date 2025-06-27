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
 * Implementation file for _7kaaAmbitionInterface::Input.
 */

#define _AMBITION_IMPLEMENTATION
#include "input.hh"

#include <algorithm>
#include <SDL.h>

#include "KEY.h"
#include "OFIRMA.h"
#include "OGETA.h"
#include "OINFO.h"
#include "OMOUSE.h"
#include "OMOUSE2.h"
#include "OSLIDCUS.h"
#include "OTOWN.h"
#include "OU_CARA.h"
#include "OVBROWIF.h"
#include "OWORLD.h"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_input.hh"
#include "Ambition_news.hh"
#include "Ambition_trade.hh"
#include "Ambition_user_interface.hh"


namespace _7kaaAmbitionInterface::Input {

const std::map<ReportType, Ambition::UserInterface::Rectangle> detectionAreas = {
  { ReportType::News, Ambition::UserInterface::VIEWPORT },
};


void calculateScroll(
  int& x,
  int& y
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::calculateScroll(x, y);
}

unsigned int cancelKeyEvent(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return SDLK_UNKNOWN;
  }

  return GETKEY(KEYEVENT_CANCEL);
}

unsigned int confirmKeyEvent(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return SDLK_UNKNOWN;
  }

  return GETKEY(KEYEVENT_CONFIRM);
}

void detectCaravanCloneButton(
  const UnitCaravan* _7kaaCaravan
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::Trade::detectCaravanCloneButton(_7kaaCaravan);
}

bool detectInnScroll(
  VBrowseIF& guestBrowser
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  constexpr auto SLOT_COUNT = 4;

  return Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::INFO_PANE_CONTENTS,
    Ambition::Input::BASE_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        [&guestBrowser](
          int amount
        ) {
          guestBrowser.rec_no = std::clamp(
            guestBrowser.rec_no + amount,
            1,
            guestBrowser.total_rec()
          );
          guestBrowser.refresh();
        }
      },
    },
    []() { return SLOT_COUNT; }
  );
}

bool detectTradeReportCaravanCloneButton(
  VBrowseIF& caravanBrowser
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  const auto caravanBrowserRecordNumber = caravanBrowser.mouse_over();
  if (caravanBrowserRecordNumber < 1) {
    return false;
  }

  const auto _7kaaCaravan = dynamic_cast<UnitCaravan*>(
    unit_array[info.get_report_data(caravanBrowserRecordNumber)]
  );
  return Ambition::Trade::detectReportCaravanCloneButton(_7kaaCaravan);
}

bool detectClipboardKeys(
  GetA* textField
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  constexpr auto OBFUSCATION_CHARACTER = '*';

  const auto key = mouse.is_key(
    mouse.scan_code,
    mouse.event_skey_state,
    static_cast<unsigned short>(0),
    K_CHAR_KEY | K_IS_CTRL
  );

  if (key == 'c' || key == 'x') {
    const auto selectedText = std::string(
      textField->input_field,
      textField->mark_begin(),
      textField->mark_end() - textField->mark_begin()
    );
    SDL_SetClipboardText(selectedText.c_str());
  }

  if (key == 'x' || key == 'v') {
    const auto remainingLength
      = textField->field_len - textField->mark_end();
    memmove(
      textField->input_field + textField->mark_begin(),
      textField->input_field + textField->mark_end(),
      remainingLength
    );
    textField->cursor_pos = textField->mark_begin();
    textField->clear_select();

    if (textField->hide_flag) {
      textField->hide_field[strlen(textField->input_field)] = '\0';
    }
  }

  if (key == 'v') {
    const auto buffer = SDL_GetClipboardText();
    const auto fieldLength = strlen(textField->input_field);
    const auto remainingLength = textField->field_len - fieldLength;
    const auto charactersToInsert = std::min(remainingLength, strlen(buffer));

    memmove(
      textField->input_field + textField->mark_begin() + charactersToInsert,
      textField->input_field + textField->mark_begin(),
      fieldLength - textField->mark_begin() + 1
    );

    strncpy(
      &textField->input_field[textField->mark_begin()],
      buffer,
      charactersToInsert
    );
    SDL_free(buffer);

    if (textField->hide_flag) {
      const auto oldLength = strlen(textField->hide_field);
      for (auto i = oldLength; i < oldLength + charactersToInsert; i++) {
        textField->hide_field[i] = OBFUSCATION_CHARACTER;
      }
      textField->hide_field[strlen(textField->input_field)] = '\0';
    }

    textField->cursor_pos = textField->mark_begin() + charactersToInsert;
    textField->clear_select();
  }

  if (key == 'c' || key == 'v' || key == 'x') {
    return true;
  }

  return false;
}

bool detectModeSelectClick(
) {
  return Ambition::detectModeSelectClick();
}

bool detectRallyButtonClick(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  std::shared_ptr<Ambition::Building> building;
  if (firm_array.selected_recno) {
    building = Ambition::Building::findBy7kaaFirmRecordNumber(
      firm_array.selected_recno
    );
  }
  if (town_array.selected_recno) {
    building = Ambition::Building::findBy7kaaTownRecordNumber(
      town_array.selected_recno
    );
  }

  if (!building) {
    return false;
  }

  const auto buttonClickArea
    = Ambition::UserInterface::RALLY_POINT_BUTTON.outer(2);

  if (mouse.single_click(
      buttonClickArea.start.left,
      buttonClickArea.start.top,
      buttonClickArea.end.left,
      buttonClickArea.end.top,
      0
    )
  ) {
    world.go_loc(
      building->getRallyLocation().to7kaaCoordinates().x,
      building->getRallyLocation().to7kaaCoordinates().y
    );
    return true;
  }

  if (mouse.single_click(
      buttonClickArea.start.left,
      buttonClickArea.start.top,
      buttonClickArea.end.left,
      buttonClickArea.end.top,
      1
    )
    // TODO: make clearing the rally point its own event, defaulting to the T
    // key.
    || ISKEY(KEYEVENT_UNIT_SETTLE)
  ) {
    building->clearRallyPoint();
    return true;
  }

  return false;
}

void detectReportScroll(
  const ReportType reportType,
  VBrowseIF& browser,
  short& selectedRecordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::Input::detectReportScroll(
    browser,
    selectedRecordNumber,
    detectionAreas.find(reportType)->second
  );
}

inline int LSOPTION_SLOT(int n) { return 1 << (n); }

void detectSaveGameScroll(
  const int minimumRecordNumber,
  const int size,
  short& browseRecordNumber,
  short& browseTopRecordNumber,
  int& refreshFlag,
  SlideVBar& scrollBar
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  constexpr auto LSOPTION_ALL_SLOTS = 0x0000ffff;
  constexpr auto LSOPTION_SCROLL = 0x00020000;
  constexpr auto SLOT_COUNT = 5;

  Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::BOUNDS,
    { },
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        [&browseTopRecordNumber, &refreshFlag, &scrollBar](
          int amount
        ) {
          const auto oldValue = scrollBar.view_recno;
          if (oldValue != scrollBar.set_view_recno(oldValue + amount)) {
            refreshFlag |= LSOPTION_SCROLL | LSOPTION_ALL_SLOTS;
          }
          browseTopRecordNumber = scrollBar.view_recno;
        }
      },
    },
    []() { return SLOT_COUNT; }
  );

  Ambition::Input::detectScroll(
    false,
    Ambition::UserInterface::BOUNDS,
    Ambition::Input::EXTENDED_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
          [&browseRecordNumber,
            &browseTopRecordNumber,
            &minimumRecordNumber,
            &refreshFlag,
            &scrollBar,
            size
          ](
            int amount
          ) {
          refreshFlag |= LSOPTION_SLOT(
            browseRecordNumber - scrollBar.view_recno
          );

          browseRecordNumber += amount;
          if (browseRecordNumber < minimumRecordNumber) {
            browseRecordNumber = minimumRecordNumber;
          }
          if (browseRecordNumber > size) {
            browseRecordNumber = size;
          }

          refreshFlag |= LSOPTION_SLOT(
            browseRecordNumber - scrollBar.view_recno
          );

          if (browseRecordNumber - scrollBar.view_recno < 0) {
            scrollBar.set_view_recno(browseRecordNumber);
            refreshFlag |= LSOPTION_SCROLL | LSOPTION_ALL_SLOTS;
            browseTopRecordNumber = scrollBar.view_recno;
          }
          if (browseRecordNumber - scrollBar.view_recno >= SLOT_COUNT) {
            scrollBar.set_view_recno(browseRecordNumber - (SLOT_COUNT - 1));
            refreshFlag |= LSOPTION_SCROLL | LSOPTION_ALL_SLOTS;
            browseTopRecordNumber = scrollBar.view_recno;
          }
        }
      },
    },
    []() { return SLOT_COUNT; }
  );
}

bool detectWhatsNewClick(
) {
  constexpr Ambition::UserInterface::Rectangle WhatsNewButton = {
    .start = {
      .left = 770,
      .top = 550,
    },
    .end = {
      .left = 789,
      .top = 570,
    },
  };
  const auto clickArea = WhatsNewButton.outer(8);

  if (
    mouse.single_click(
      clickArea.start.left,
      clickArea.start.top,
      clickArea.end.left,
      clickArea.end.top
    )
  ) {
    Ambition::News::display();
    return true;
  }

  return false;
}

void setOrClearRallyPoint(
  const short _7kaaX,
  const short _7kaaY,
  const bool allowAction
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto location = world.get_loc(_7kaaX, _7kaaY);
  Ambition::setOrClearRallyPoint(
    { .x =_7kaaX, .y = _7kaaY },
    allowAction && location->explored()
  );
}

} // namespace _7kaaAmbitionInterface::Input
