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
 * Header file for Ambition::Vga.
 */

#pragma once

#include <string>

class Button3D;
class Firm;
class Font;
class SnowLayer;
class Town;
class Unit;
class VgaBuf;
struct FirmBitmap;


namespace Ambition {

constexpr auto FIRM_ID_TOWN = 0;

namespace Coordinates {
struct Point;
}

namespace UserInterface {
struct Rectangle;
}


short calculateAnimatedLinePhase(
  const int animatedFlag,
  const int lineProgress
);

FirmBitmap* calculateFirmBitmap(
  FirmBitmap* _7kaaCalculation,
  Firm* firm
);

int calculateHitbarBaseColour(
  const double maximumHitpoints
);

int calculateHitbarWidth(
  const int availableWidth,
  const double maximumHitpoints
);

short calculateRainSpeed(
  const short _7kaaCalculation
);

char calculateRockRemainingDelay(
  const char _7kaaCalculation
);

char* calculateTerrainBitmap(
  const short terrainId,
  const int x,
  const int y
);

char calculateTownFlagNumber(
  const int townRecordNumber
);

int calculateUnitHitbarWidth(
);

int calculateUnitIconY(
  const int _7kaaCalculation
);

int calculateWorkerPortraitX(
  const int workerIndex
);

int centreHitbar(
  const int left,
  const int maximumWidth,
  const int currentWidth
);

void delayFrame(
  const unsigned long long int deadlineSdlTicks64 = 0
);

/**
 * Display the given game speed on the screen.
 *
 * @param speed The speed to display.  Special value 0 means paused.  Special
 * value 99 means unlimited.
 */
void displayGameSpeed(
  int speed
);

void displayTownQualityOfLife(
  Town* town,
  const int refreshFlag,
  const int displayTop
);

void displayUnitContribution(
  ::Unit* unit,
  const int x,
  const int y
);

void drawBuildingLinkLine(
  const int sourceFirmId,
  const int destinationFirmId,
  int sourceLeft,
  int sourceTop,
  int destinationLeft,
  int destinationTop,
  const bool animated = true,
  const bool thick = false
);

/**
 * Draw a hitbar in the info pane for an occupant of a building.
 *
 * @return true when the Ambition code has taken effect and 7kaa should skip the
 * rest of its.
 */
bool drawBuildingOccupantHitbar(
  const int leftX,
  const int topY,
  const int maximumWidth,
  const int currentHitpoints,
  const int maximumHitpoints
);

void drawBuildingProgressBar(
  Firm* firm
);

void drawBuildingRallyPoint(
  Firm* _7kaaFirm
);
void drawBuildingRallyPoint(
  Town* _7kaaTown
);

void drawButtonOverlay(
  UserInterface::Rectangle buttonCoordinates,
  std::string text
);

/**
 * Draw a builder icon for the firm if it has a builder.
 *
 * Also animate it if repair is currently in progress.
 *
 * @param firm A pointer to the Firm for which to draw the icon.
 */
void drawFirmBuilderIcon(
  Firm* firm
);

void drawFirmFrame(
  Firm* firm,
  const int displayLayer
);

/**
 * Draw the hitbar for a Firm.
 *
 * @param firm The Firm for which to draw a hitbar.
 */
void drawFirmHitBar(
  Firm* firm
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
void drawHitbarOutline(
  const bool isOwn,
  const int x,
  const int y,
  const int width,
  const int height
);

void drawInnGuestCount(
  const short count,
  const int refreshFlag
);

void drawLoadMenuDeleteButton(
  Button3D& button,
  const int left,
  const int top
);

void drawMinimapLine(
  Coordinates::Point from,
  Coordinates::Point to,
  int effectFlag
);

void drawOutsideLeadershipIcon(
  const ::Unit* _7kaaUnit
);

void drawPanel(
  UserInterface::Rectangle rectangle
);

void drawTownTrainingProgressBar(
  const Town* town
);

void drawWorldLine(
  Coordinates::Point from,
  Coordinates::Point to,
  int animatedFlag,
  int effectFlag
);

bool initialiseSnowLayer(
  SnowLayer& layer,
  const int level,
  const char animationSpeed,
  double slideSpeed
);

void printLeadershipStatus(
  ::Unit *_7kaaUnit,
  const int top,
  const int refreshFlag
);

/**
 * Unlock a VgaBuf, overriding the usual buffer unlock steps as necessary.
 *
 * This is needed because sometimes unlocking a VgaBuf also causes a Vga flip
 * and we want to control when the flips occur.
 *
 * @param buffer The VgaBuf to unlock.
 */
void unlockBuffer(
  VgaBuf& buffer
);

} // namespace Ambition
