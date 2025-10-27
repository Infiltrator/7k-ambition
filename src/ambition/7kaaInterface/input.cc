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
#include "OBUTT3D.h"
#include "OFIRMA.h"
#include "OGETA.h"
#include "OINFO.h"
#include "OMOUSE.h"
#include "OMOUSE2.h"
#include "OSLIDCUS.h"
#include "OSPY.h"
#include "OSYS.h"
#include "OTOWN.h"
#include "OU_CARA.h"
#include "OVBROWIF.h"
#include "OWORLD.h"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_control.hh"
#include "Ambition_input.hh"
#include "Ambition_news.hh"
#include "Ambition_spy.hh"
#include "Ambition_trade.hh"
#include "Ambition_user_interface.hh"


namespace _7kaaAmbitionInterface::Input {

const std::map<ReportType, Ambition::UserInterface::Rectangle> detectionAreas = {
  {
    ReportType::KingdomKingdoms,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 241,
      }
    ),
  },
  {
    ReportType::KingdomDiplomaticLogs,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 303,
      },
      Ambition::UserInterface::HorizontalAlignment::Left,
      Ambition::UserInterface::VerticalAlignment::Bottom
    ),
  },
  { ReportType::News, Ambition::UserInterface::VIEWPORT },
  {
    ReportType::VillagesTowns,
    Ambition::UserInterface::Report::Villages::Towns::AREA.outer(6, 6, 6, 2),
  },
  {
    ReportType::VillagesFirms,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 235,
      },
      Ambition::UserInterface::HorizontalAlignment::Left,
      Ambition::UserInterface::VerticalAlignment::Bottom
    ),
  },
  {
    ReportType::EconomyIncomes,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 195,
      }
    ),
  },
  {
    ReportType::EconomyExpenses,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 349,
      },
      Ambition::UserInterface::HorizontalAlignment::Left,
      Ambition::UserInterface::VerticalAlignment::Bottom
    ),
  },
  {
    ReportType::TradeTraders,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 228,
      }
    ),
  },
  {
    ReportType::TradeFirms,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 316,
      },
      Ambition::UserInterface::HorizontalAlignment::Left,
      Ambition::UserInterface::VerticalAlignment::Bottom
    ),
  },
  {
    ReportType::MilitaryTroops,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 249,
      }
    ),
  },
  {
    ReportType::MilitaryUnits,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 295,
      },
      Ambition::UserInterface::HorizontalAlignment::Left,
      Ambition::UserInterface::VerticalAlignment::Bottom
    ),
  },
  {
    ReportType::TechnologyTechnologies,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 230,
      }
    ),
  },
  {
    ReportType::TechnologyGods,
    Ambition::UserInterface::VIEWPORT.internal(
      {
        .width = Ambition::UserInterface::VIEWPORT.width(),
        .height = 229,
      },
      Ambition::UserInterface::HorizontalAlignment::Left,
      Ambition::UserInterface::VerticalAlignment::Bottom
    ),
  },
  { ReportType::Espionage, Ambition::UserInterface::VIEWPORT },
  { ReportType::Ranking, Ambition::UserInterface::VIEWPORT },
};

const std::map<Action, KeyEventType> KEY_EVENT_MAP = {
  { Action::Common_Cancel, KEYEVENT_CANCEL },
  { Action::Common_Confirm, KEYEVENT_CONFIRM },
  { Action::Common_Reward, KEYEVENT_UNIT_REWARD },

  { Action::Spy_Assassinate, KEYEVENT_SPY_ASSASSINATE },
  { Action::Spy_Bribe, KEYEVENT_SPY_BRIBE },
  { Action::Spy_Capture, KEYEVENT_SPY_CAPTURE },
  { Action::Spy_ChangeMission, KEYEVENT_SPY_CHANGE_MISSION },
  { Action::Spy_Mobilise, KEYEVENT_SPY_MOBILISE },
  { Action::Spy_Menu, KEYEVENT_SPY_MENU },
  { Action::Spy_StealReports, KEYEVENT_SPY_STEAL_REPORTS },

  { Action::Town_Train_Builder, KEYEVENT_TRAIN_BUILDER },
  { Action::Town_Train_Soldier, KEYEVENT_TRAIN_SOLDIER },
  { Action::Town_Train_Miner, KEYEVENT_TRAIN_MINER },
  { Action::Town_Train_Artisan, KEYEVENT_TRAIN_ARTISAN },
  { Action::Town_Train_Researcher, KEYEVENT_TRAIN_RESEARCHER },
  { Action::Town_Train_Spy, KEYEVENT_TRAIN_SPY },

  { Action::Building_Mobilise_All, KEYEVENT_FIRM_PATROL },
};

constexpr Action TRAINING_KEY_ACTIONS[MAX_TRAINABLE_SKILL] = {
  Action::Town_Train_Builder,
  Action::Town_Train_Soldier,
  Action::Town_Train_Miner,
  Action::Town_Train_Artisan,
  Action::Town_Train_Researcher,
  Action::Town_Train_Spy,
};


void detectStealReportConfirmationMenu(
  char& menu,
  const char _7kaaMenuMain,
  const ::Spy* _7kaaSpy,
  const short _7kaaNationRecordNumber
);

char* stripUnprintable7kaaCharacters(
  char* input
);


bool detectBuildingMenu(
  char& menu,
  ::Spy* _7kaaSpy,
  const Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  if (firm_array.selected_recno
    != Ambition::UserInterface::selected7kaaFirmOrTownRecordNumber
  ) {
    return false;
  }

  switch (Ambition::UserInterface::buildingMenu) {
  case Ambition::UserInterface::BuildingMenu::_7kaa:
    break;

  case Ambition::UserInterface::BuildingMenu::AssassinationConfirmation:
    if (Ambition::Spy::assassinationButton.detect(
        Input::getKeyEvent(Input::Action::Common_Confirm)
      )
    ) {
      Ambition::UserInterface::buildingMenu
        = Ambition::UserInterface::BuildingMenu::_7kaa;
      _7kaaSpy->assassinate(_7kaaFirm->overseer_recno, COMMAND_PLAYER);
    }
    if (Ambition::Spy::cancelButton.detect(
        Input::getKeyEvent(Input::Action::Common_Cancel)
      )
    ) {
      menu = FIRM_MENU_MAIN;
      Ambition::UserInterface::buildingMenu
        = Ambition::UserInterface::BuildingMenu::_7kaa;
      info.disp();
    }
    return true;
    break;

  case Ambition::UserInterface::BuildingMenu::StealReportConfirmation:
    detectStealReportConfirmationMenu(
      menu,
      FIRM_MENU_MAIN,
      _7kaaSpy,
      _7kaaFirm->nation_recno
    );
    return true;
    break;
  }

  return false;
}
bool detectBuildingMenu(
  char& menu,
  const ::Spy* _7kaaSpy,
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  if (town_array.selected_recno
    != Ambition::UserInterface::selected7kaaFirmOrTownRecordNumber
  ) {
    return false;
  }

  switch (Ambition::UserInterface::buildingMenu) {
  case Ambition::UserInterface::BuildingMenu::_7kaa:
    break;

  case Ambition::UserInterface::BuildingMenu::AssassinationConfirmation:
    assert(false);
    break;

  case Ambition::UserInterface::BuildingMenu::StealReportConfirmation:
    constexpr auto TOWN_MENU_MAIN = 0;

    detectStealReportConfirmationMenu(
      menu,
      TOWN_MENU_MAIN,
      _7kaaSpy,
      _7kaaTown->nation_recno
    );
    return true;
    break;
  }

  return false;
}

void detectCaravanCloneButton(
  UnitCaravan* _7kaaCaravan
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::Trade::detectCaravanCloneButton(_7kaaCaravan);
}

bool detectUnitListScroll(
  VBrowseIF& guestBrowser
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

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
          guestBrowser.top_rec_no = std::clamp(
            guestBrowser.top_rec_no + amount,
            1,
            std::max(1, guestBrowser.total_rec() - guestBrowser.y_max_rec + 1)
          );
          guestBrowser.rec_no = std::clamp(
            guestBrowser.rec_no + amount,
            1,
            guestBrowser.total_rec()
          );
          guestBrowser.refresh();
        }
      },
    },
    [&guestBrowser]() { return guestBrowser.y_max_rec - 1; }
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
    const auto buffer = stripUnprintable7kaaCharacters(SDL_GetClipboardText());
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

void detectDiscordClick(
) {
  if (Ambition::UserInterface::detectMouseClick(
      Ambition::UserInterface::DISCORD_BUTTON_CLICK_AREA
    )
  ) {
    Ambition::Control::openDiscord();
  }
}

void detectFeedbackLinkClick(
) {
  if (Ambition::UserInterface::detectMouseClick(
      Ambition::UserInterface::FEEDBACK_LINK_CLICK_AREA
    )
  ) {
    Ambition::Control::openFeedback();
  }
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
    []() { return SLOT_COUNT - 1; }
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
    []() { return SLOT_COUNT - 1; }
  );
}

void detectScenarioScroll(
  const int minimumRecordNumber,
  const int size,
  int& browseRecordNumber,
  SlideVBar& scrollBar,
  SlideVBar& descriptionScrollBar,
  int& refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  constexpr auto TUOPTION_ALL_BROWSE = 0x0000ffff;
  constexpr auto TUOPTION_PIC_AREA = 0x00040000;
  constexpr auto TUOPTION_SCROLL = 0x00080000;
  constexpr auto TUOPTION_TEXT_AREA = 0x00020000;
  constexpr auto TUOPTION_TEXT_BUFFER = 0x00200000;
  constexpr auto TUOPTION_TEXT_SCROLL = 0x00100000;
  constexpr auto SLOT_COUNT = 4;

  const auto scrollScenarioSelection = [
    &browseRecordNumber,
    &refreshFlag,
    &scrollBar,
    minimumRecordNumber,
    size
  ] (
    const int amount
  ) {
    refreshFlag |= LSOPTION_SLOT(
      browseRecordNumber - scrollBar.view_recno
    );

    browseRecordNumber = std::clamp(
      browseRecordNumber + amount,
      minimumRecordNumber,
      size
    );

    refreshFlag |= LSOPTION_SLOT(
      browseRecordNumber - scrollBar.view_recno
    );

    if (browseRecordNumber - scrollBar.view_recno < 0) {
      scrollBar.set_view_recno(browseRecordNumber);
      refreshFlag |= TUOPTION_SCROLL | TUOPTION_ALL_BROWSE;
    }
    if (browseRecordNumber - scrollBar.view_recno >= SLOT_COUNT) {
      scrollBar.set_view_recno(browseRecordNumber - (SLOT_COUNT - 1));
      refreshFlag |= TUOPTION_SCROLL | TUOPTION_ALL_BROWSE;
    }

    refreshFlag
      |= TUOPTION_TEXT_BUFFER
      | TUOPTION_TEXT_SCROLL
      | TUOPTION_TEXT_AREA
      | TUOPTION_PIC_AREA;
  };

  Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::ScenarioList::HEADING_AREA,
    Ambition::Input::STANDARD_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        scrollScenarioSelection,
      },
    },
    []() { return SLOT_COUNT - 1; }
  );

  Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::ScenarioList::DESCRIPTION_AREA,
    Ambition::Input::STANDARD_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        [ &descriptionScrollBar, &refreshFlag ] (
          const int amount
        ) {
          descriptionScrollBar.set_view_recno(
            descriptionScrollBar.view_recno + amount
          );
          refreshFlag |= TUOPTION_TEXT_SCROLL | TUOPTION_TEXT_AREA;
        },
      },
    },
    []() { return SLOT_COUNT - 1; }
  );

  Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::ScenarioList::LIST_AREA,
    { },
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        [ &scrollBar, &refreshFlag ] (
          const int amount
        ) {
          scrollBar.set_view_recno(scrollBar.view_recno + amount);
          refreshFlag |= TUOPTION_SCROLL | TUOPTION_ALL_BROWSE;
        },
      },
    },
    []() { return SLOT_COUNT - 1; }
  );

  Ambition::Input::detectScroll(
    false,
    Ambition::UserInterface::ScenarioList::LIST_AREA,
    Ambition::Input::STANDARD_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        scrollScenarioSelection,
      },
    },
    []() { return SLOT_COUNT - 1; }
  );
}

bool detectSpyScroll(
  VBrowseIF& spyBrowser
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::INFO_PANE_CONTENTS,
    Ambition::Input::EXTENDED_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        [&spyBrowser] (
          const int amount
        ) {
          spyBrowser.top_rec_no = std::clamp(
            spyBrowser.top_rec_no + amount,
            1,
            std::max(1, spyBrowser.total_rec() - spyBrowser.y_max_rec + 1)
          );
          spyBrowser.rec_no = std::clamp(
            spyBrowser.rec_no + amount,
            1,
            spyBrowser.total_rec()
          );
        }
      },
    },
    [spyBrowser]() { return spyBrowser.y_max_rec - 1; },
    [spyBrowser]() { return spyBrowser.total_rec_num; }
  );
}

bool detectTownScroll(
  VBrowseIF& raceBrowser
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::INFO_PANE_CONTENTS,
    Ambition::Input::EXTENDED_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        [&raceBrowser] (
          const int amount
        ) {
          raceBrowser.top_rec_no = std::clamp(
            raceBrowser.top_rec_no + amount,
            1,
            std::max(1, raceBrowser.total_rec() - raceBrowser.y_max_rec + 1)
          );
          raceBrowser.rec_no = std::clamp(
            raceBrowser.rec_no + amount,
            1,
            raceBrowser.total_rec()
          );
        }
      },
    },
    [raceBrowser]() { return raceBrowser.y_max_rec - 1; },
    [raceBrowser]() { return raceBrowser.total_rec_num; }
  );
}

void detectTutorialScroll(
  const int minimumRecordNumber,
  const int size,
  int& browseRecordNumber,
  SlideVBar& scrollBar,
  int& refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  constexpr auto REFRESH_ALL_SLOTS = (1 << 16) - 1;
  constexpr auto REFRESH_DESCRIPTION = 1 << 17;
  constexpr auto REFRESH_PICTURE = 1 << 18;
  constexpr auto REFRESH_LIST_SCROLL_BAR = 1 << 19;
  constexpr auto SLOT_COUNT = 5;

  const auto scrollTutorialSelection = [
    &browseRecordNumber,
    &refreshFlag,
    &scrollBar,
    minimumRecordNumber,
    size
  ] (
    const int amount
  ) {
    refreshFlag |= LSOPTION_SLOT(
      browseRecordNumber - scrollBar.view_recno
    );

    browseRecordNumber = std::clamp(
      browseRecordNumber + amount,
      minimumRecordNumber,
      size
    );

    refreshFlag |= LSOPTION_SLOT(
      browseRecordNumber - scrollBar.view_recno
    );

    if (browseRecordNumber - scrollBar.view_recno < 0) {
      scrollBar.set_view_recno(browseRecordNumber);
      refreshFlag |= REFRESH_LIST_SCROLL_BAR | REFRESH_ALL_SLOTS;
    }
    if (browseRecordNumber - scrollBar.view_recno >= SLOT_COUNT) {
      scrollBar.set_view_recno(browseRecordNumber - (SLOT_COUNT - 1));
      refreshFlag |= REFRESH_LIST_SCROLL_BAR | REFRESH_ALL_SLOTS;
    }

    refreshFlag
      |= REFRESH_DESCRIPTION
      | REFRESH_PICTURE;
  };

  Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::TutorialList::HEADING_AREA,
    Ambition::Input::STANDARD_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        scrollTutorialSelection,
      },
    },
    []() { return SLOT_COUNT - 1; }
  );

  Ambition::Input::detectScroll(
    true,
    Ambition::UserInterface::TutorialList::LIST_AREA,
    { },
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        [ &scrollBar, &refreshFlag ] (
          const int amount
        ) {
          scrollBar.set_view_recno(scrollBar.view_recno + amount);
          refreshFlag |= REFRESH_LIST_SCROLL_BAR | REFRESH_ALL_SLOTS;
        },
      },
    },
    []() { return SLOT_COUNT - 1; }
  );

  Ambition::Input::detectScroll(
    false,
    Ambition::UserInterface::TutorialList::LIST_AREA,
    Ambition::Input::STANDARD_ACTIVATIONS,
    {
      {
        Ambition::Input::ScrollOrientation::Vertical,
        scrollTutorialSelection,
      },
    },
    []() { return SLOT_COUNT - 1; }
  );
}

bool detectWhatsNewClick(
) {
  constexpr Ambition::UserInterface::Rectangle WHATS_NEW_BUTTON = {
    .start = {
      .left = 770,
      .top = 550,
    },
    .end = {
      .left = 789,
      .top = 570,
    },
  };
  const auto clickArea = WHATS_NEW_BUTTON.outer(8);

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

bool enterAssassinationConfirmationMenu(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  Ambition::UserInterface::buildingMenu
    = Ambition::UserInterface::BuildingMenu::AssassinationConfirmation;
  Ambition::UserInterface::selected7kaaFirmOrTownRecordNumber
    = firm_array.selected_recno;
  return true;
}

bool enterStealReportConfirmationMenu(
  const int reportType
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  Ambition::UserInterface::buildingMenu
    = Ambition::UserInterface::BuildingMenu::StealReportConfirmation;
  Ambition::UserInterface::reportType = reportType;
  Ambition::UserInterface::selected7kaaFirmOrTownRecordNumber
    = firm_array.selected_recno ?: town_array.selected_recno;
  return true;
}

unsigned int getKeyEvent(
  const Action action
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return SDLK_UNKNOWN;
  }

  return GETKEY(KEY_EVENT_MAP.find(action)->second);
}

unsigned int getTrainingKeyEvent(
  const int _7kaaSkillIndex
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return SDLK_UNKNOWN;
  }

  assert(_7kaaSkillIndex >= 1 && _7kaaSkillIndex <= MAX_TRAINABLE_SKILL);

  return getKeyEvent(TRAINING_KEY_ACTIONS[_7kaaSkillIndex - 1]);
}

void resetBuildingMenu(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::UserInterface::buildingMenu
    = Ambition::UserInterface::BuildingMenu::_7kaa;

  return;
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


/* Private functions. */

void detectStealReportConfirmationMenu(
  char& menu,
  const char _7kaaMenuMain,
  const ::Spy* _7kaaSpy,
  const short _7kaaNationRecordNumber
) {
  if (Ambition::Spy::stealReportsButton.detect(
      Input::getKeyEvent(Input::Action::Common_Confirm)
    )
  ) {
    sys.set_view_mode(
      Ambition::UserInterface::reportType + 1,
      _7kaaNationRecordNumber,
      _7kaaSpy->spy_recno
    );
    menu = _7kaaMenuMain;
    Ambition::UserInterface::buildingMenu
      = Ambition::UserInterface::BuildingMenu::_7kaa;
    info.disp();
  }
  if (Ambition::Spy::cancelButton.detect(
      Input::getKeyEvent(Input::Action::Common_Cancel)
    )
  ) {
    menu = _7kaaMenuMain;
    Ambition::UserInterface::buildingMenu
      = Ambition::UserInterface::BuildingMenu::_7kaa;
    info.disp();
  }
}

char* stripUnprintable7kaaCharacters(
  char* input
) {
  const auto length = strlen(input);
  for(auto readIndex = 0, writeIndex = readIndex;
      readIndex < length;
      readIndex++
  ) {
    const auto character = input[readIndex];
    if (character > 0 && character < ' ') {
      continue;
    }
    input[writeIndex] = input[readIndex];
    writeIndex++;
  }
  return input;
}

} // namespace _7kaaAmbitionInterface::Input
