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
 * Header file for _7kaaAmbitionInterface::Draw.
 */

#pragma once

#include "OINFO.h"

class Button;
class Button3D;
class ButtonCustom;
class Firm;
class FirmHarbor;
class FirmWar;
class Font;
class InnUnit;
class MapMatrix;
class SnowLayer;
class Spy;
class Town;
class Unit;
class UnitCaravan;
class VBrowseIF;
class Worker;
struct FirmBitmap;


namespace _7kaaAmbitionInterface::Draw {

constexpr auto FIRM_ID_TOWN = 0;


void buildingAnimationFrame(
  Firm* firm,
  const int displayLayer
);

/**
 * Draw a builder icon for the firm if it has a builder.
 *
 * Also animate it if repair is currently in progress.
 *
 * @param firm A pointer to the Firm for which to draw the icon.
 */
void buildingBuilderIcon(
  Firm* firm
);

/**
 * Draw the hitbar for a Firm.
 *
 * @param firm The Firm for which to draw a hitbar.
 */
void buildingHitBar(
  Firm* firm
);

void buildingLinkLine(
  const int sourceFirmId,
  const int destinationFirmId,
  int sourceLeft,
  int sourceTop,
  int destinationLeft,
  int destinationTop,
  const bool animated = true,
  const bool thick = false
);

bool buildingMenu(
  const int refreshFlag,
  const ::Spy* _7kaaSpy,
  const Firm* _7kaaFirm
);

/**
 * Draw a hitbar in the info pane for an occupant of a building.
 *
 * @return true when the Ambition code has taken effect and 7kaa should skip the
 * rest of its.
 */
bool buildingOccupantHitbar(
  const int left,
  const int top,
  const int maximumWidth,
  const int currentHitpoints,
  const int maximumHitpoints
);

void buildingProgressBar(
  Firm* firm
);

void buildingProgressBar(
  const Town* town
);

void buildingRallyButton(
  Firm* _7kaaFirm
);
void buildingRallyButton(
  const Town* _7kaaTown
);

void buildingRallyPoint(
  Firm* _7kaaFirm
);
void buildingRallyPoint(
  Town* _7kaaTown
);

void buildMarkerGridLines(
  const int left,
  const int top,
  const int right,
  const int bottom
);

void buildModeHighlighting(
  const short _7kaaX,
  const short _7kaaY,
  const int left,
  const int top,
  const short maximum7kaaX,
  const short maximum7kaaY
);

void buttonCost(
  Font& font,
  const int cost,
  const int left,
  const int top,
  const int right,
  const int bottom,
  const int verticalAlignment = 1
);

void buttonKeybind(
  const unsigned int keyCode,
  const int left,
  const int top,
  const int right,
  const int bottom,
  const int refreshFlag = INFO_REPAINT
);
void buttonKeybind(
  const unsigned int keyCode,
  const Button& button
);
void buttonKeybind(
  const unsigned int keyCode,
  const Button3D& button,
  const int refreshFlag = INFO_REPAINT
);
void buttonKeybind(
  const unsigned int keyCode,
  const ButtonCustom& button
);

short calculateAnimatedLinePhase(
  const short _7kaaCalculation,
  const int animatedFlag,
  const int lineProgress
);

int calculateBuildMarkerColour(
  const int _7kaaCalculation
);

int calculateDoneButtonWidth(
  const int _7kaaCalculation,
  const Firm* firm
);

FirmBitmap* calculateFirmBitmap(
  FirmBitmap* _7kaaCalculation,
  Firm* firm
);

void calculateFirmDrawLocation(
  const int _7kaaFirmBuildId,
  int& left,
  int& top,
  int& right,
  int& bottom
);

int calculateHitbarBaseColour(
  const int _7kaaCalculation,
  const double maximumHitpoints
);

int calculateHitbarWidth(
  const int availableWidth,
  const double maximumHitpoints
);

short calculateRainSpeed(
  const short _7kaaCalculation
);

short calculateRockBitmapRecordNumber(
  const short _7kaaCaclulation
);

char calculateRockRemainingDelay(
  const char _7kaaCalculation
);

char* calculateTerrainBitmap(
  char* _7kaaCalculation,
  const short terrainId,
  const int x,
  const int y
);

char calculateTownFlagNumber(
  const char _7kaaCalculation,
  const int townRecordNumber
);

int calculateUnitHitbarWidth(
  const int _7kaaCalculation
);

int calculateUnitIconY(
  const int _7kaaCalculation
);

int calculateWorkerPortraitLeft(
  const int _7kaaCalculation,
  const int workerIndex
);

void caravanCloneButton(
  UnitCaravan* _7kaaCaravan,
  const int refreshFlag
);

int centreHitbar(
  const int left,
  const int maximumWidth,
  const int currentWidth
);

void discordButton(
);

void doneButtonText(
  const ButtonCustom& button
);

void feedbackLink(
);

/**
 * Draw a hitbar outline based on the owneship.
 *
 * @param isOwn Whether the hitbar's object is owned by the Nation to draw for.
 * @param x The pixel x-coordinate of the top-left of the hitbar.
 * @param y The pixel y-coordinate of the top-left of the hitbar.
 * @param width The pixel width of the hitbar.
 * @param height The pixel height of the hitbar.
 */
void hitbarOutline(
  const bool isOwn,
  const int x,
  const int y,
  const int width,
  const int height
);

bool initialiseSnowLayer(
  SnowLayer& layer,
  const int level,
  const char animationSpeed,
  double slideSpeed
);

void innGuestCount(
  const short count,
  const int refreshFlag
);

void innGuestDetailsLeavingInformation(
  const InnUnit* _7kaaInnGuest,
  const int refreshFlag
);

void innGuestLeavingSoonMark(
  const InnUnit* _7kaaInnGuest,
  const int top
);

void loadMenuDeleteButton(
  Button3D& button,
  const int left,
  const int top
);

void militaryReportCostHeadings(
);

void militaryReportCostLine(
  const int top,
  const int rowHeight,
  const int unitID,
  const int unitCount
);

void militaryReportTotalCost(
  const int top,
  const int rowHeight
);

bool minimapViewBoundsBox(
  const MapMatrix* _7kaaMapMatrix
);

/**
 * Draw the current running mode onto the top-right of the screen.
 */
void modeInformation(
);

void outsideLeadershipIcon(
  Unit* _7kaaUnit
);

void printAssasinationEstimate(
  const ::Spy* _7kaaSpy,
  const Firm* _7kaaFirm,
  ::Unit* target
);

void printBribeEstimate(
  const ::Spy* _7kaaSpy,
  Firm* _7kaaFirm,
  const char selectedWorkerId
);

/**
 * Print the given game speed to the screen.
 *
 * @param speed The speed to display.  Special value 0 means paused.  Special
 * value 99 means unlimited.
 */
void printGameSpeed(
  const int speed
);

bool printLeadershipStatus(
  Unit* _7kaaUnit,
  const int top,
  const int refreshFlag
);

bool printSaveFileDate(
  const std::string string,
  const int left,
  const int top
);

bool printSaveFilename(
  const std::string string,
  const int left,
  const int top
);

bool villagesReportTownPopulations(
  const int top,
  const int villagers,
  const int peasants
);

bool villagesReportUpperTotals(
  const int totalVillagers,
  const int totalPeasants
);

void printSyncError(
);

void printTownQualityOfLife(
  Town* town,
  const int refreshFlag,
  const int displayTop
);

void printUnitContribution(
  Unit* unit,
  const int left,
  const int top
);

void printWarMachineInBuildingInformation(
  const Worker* _7kaaWorker
);

int printWarMachineInformation(
  const ::Unit* _7kaaUnit,
  const int refreshFlag
);

void queueCount(
  const FirmHarbor* _7kaaHarbour
);
void queueCount(
  const FirmWar* _7kaaWarFactory
);
void queueCount(
  const Town* _7kaaTown
);

bool shouldDrawUnitIcon(
  const Unit* _7kaaUnit
);

void tradeReportcaravanCloneButton(
  const char browserMode,
  VBrowseIF& caravanBrowser
);

void trainingSkillIcon(
  const char _7kaaSkillId
);

void unitWaypointsOnWorld(
  const Unit* _7kaaUnit
);

void unitWaypointsOnMinimap(
  const Unit* _7kaaUnit
);

void warMachineTechnologyLevel(
  int* techLevel,
  const int left,
  const int top,
  const char* portraitBitmap
);

void whatsNewButton(
);

void workerSpyIcon(
  const char** spyIconName,
  const int left,
  const int top,
  const char* portraitBitmap
);

} // namespace _7kaaAmbitionInterface::Draw

#ifndef _AMBITION_IMPLEMENTATION
/** Allow 7kaa to call using Ambition::*. */
namespace Ambition = _7kaaAmbitionInterface;
#endif
