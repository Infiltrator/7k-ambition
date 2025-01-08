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
  const short terrainId
);

char calculateTownFlagNumber(
  const char _7kaaCalculation,
  const int townRecordNumber
);

void delayFrame(
  const unsigned long long int deadlineSdlTicks64 = 0
);

bool initialiseSnowLayer(
  SnowLayer& layer,
  const int level,
  const char animationSpeed,
  double slideSpeed
);

void drawFirmFrame(
  Firm* firm,
  const int displayLayer
);

} // namespace Ambition
