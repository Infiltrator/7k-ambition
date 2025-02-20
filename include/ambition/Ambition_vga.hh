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

class Firm;
class SnowLayer;
class Unit;
class VgaBuf;
struct FirmBitmap;


namespace Ambition {

FirmBitmap* calculateFirmBitmap(
  FirmBitmap* _7kaaCalculation,
  Firm* firm
);

short calculateRainSpeed(
  const short _7kaaCalculation
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

bool initialiseSnowLayer(
  SnowLayer& layer,
  const int level,
  const char animationSpeed,
  double slideSpeed
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
