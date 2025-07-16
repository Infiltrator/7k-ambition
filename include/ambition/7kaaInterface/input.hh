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
 * Header file for _7kaaAmbitionInterface::Input.
 */

#pragma once

class GetA;
class SlideVBar;
class UnitCaravan;
class VBrowseIF;


namespace _7kaaAmbitionInterface::Input {

enum class Action {
  Common_Cancel,
  Common_Confirm,
  Common_Reward,

  Spy_Assassinate,
  Spy_Bribe,
  Spy_Capture,
  Spy_ChangeMission,
  Spy_Mobilise,
  Spy_Menu,
  Spy_StealReports,

  Town_Train_Builder,
  Town_Train_Soldier,
  Town_Train_Miner,
  Town_Train_Artisan,
  Town_Train_Researcher,
  Town_Train_Spy,
};


enum class ReportType {
  KingdomKingdoms,
  KingdomDiplomaticLogs,
  VillagesTowns,
  VillagesFirms,
  EconomyIncomes,
  EconomyExpenses,
  TradeTraders,
  TradeFirms,
  MilitaryTroops,
  MilitaryUnits,
  TechnologyTechnologies,
  TechnologyGods,
  Espionage,
  Ranking,
  News,
};


unsigned int cancelKeyEvent(
);

unsigned int confirmKeyEvent(
);

void detectCaravanCloneButton(
  const UnitCaravan* _7kaaCaravan
);

bool detectInnScroll(
  VBrowseIF& guestBrowser
);

bool detectTradeReportCaravanCloneButton(
  VBrowseIF& caravanBrowser
);

bool detectClipboardKeys(
  GetA* textField
);

void detectDiscordClick(
);

void detectFeedbackLinkClick(
);

/**
 * Detect whether the mode selection button was clicked and then run the mode
 * selection screen.
 *
 * @return Whether anything was drawn to the screen and it needs to be redrawn.
 */
bool detectModeSelectClick(
);

bool detectRallyButtonClick(
);

void detectReportScroll(
  const ReportType reportType,
  VBrowseIF& browser,
  short& selectedRecordNumber
);

void detectSaveGameScroll(
  const int minimumRecordNumber,
  const int size,
  short& browseRecordNumber,
  short& browseTopRecordNumber,
  int& refreshFlag,
  SlideVBar& scrollBar
);

bool detectSpyScroll(
  VBrowseIF& spyBrowser
);

bool detectTownScroll(
  VBrowseIF& raceBrowser
);

/**
 * Detect whether the What's New button was clicked and then run the news
 * screen.
 *
 * @return Whether anything was drawn to the screen and it needs to be redrawn.
 */
bool detectWhatsNewClick(
);

unsigned int getKeyEvent(
  const Action action
);

unsigned int getTrainingKeyEvent(
  const int _7kaaSkillIndex
);

void setOrClearRallyPoint(
  const short _7kaaX,
  const short _7kaaY,
  const bool allowAction
);

} // namespace _7kaaAmbitionInterface::Input

#ifndef _AMBITION_IMPLEMENTATION
/** Allow 7kaa to call using Ambition::*. */
namespace Ambition = _7kaaAmbitionInterface;
#endif
