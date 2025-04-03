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
 * Implementation file for _7kaaAmbitionInterface::Draw.
 */

#define _AMBITION_IMPLEMENTATION
#include "draw.hh"

#include "OFIRM.h"
#include "ONATIONA.h"
#include "OTOWN.h"
#include "OUNIT.h"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_minimap.hh"
#include "Ambition_remote.hh"
#include "Ambition_unit.hh"
#include "Ambition_vga.hh"


namespace _7kaaAmbitionInterface::Draw {

void buildingAnimationFrame(
  Firm* firm,
  const int displayLayer
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawFirmFrame(firm, displayLayer);
}

void buildingBuilderIcon(
  Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawFirmBuilderIcon(firm);
}

void buildingHitBar(
  Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawFirmHitBar(firm);
}

void buildingLinkLine(
  const int sourceFirmId,
  const int destinationFirmId,
  int sourceLeft,
  int sourceTop,
  int destinationLeft,
  int destinationTop,
  const bool animated,
  const bool thick
) {
  Ambition::drawBuildingLinkLine(
    sourceFirmId,
    destinationFirmId,
    sourceLeft,
    sourceTop,
    destinationLeft,
    destinationTop,
    animated,
    thick
  );
}

bool buildingOccupantHitbar(
  const int left,
  const int top,
  const int maximumWidth,
  const int currentHitpoints,
  const int maximumHitpoints
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::drawBuildingOccupantHitbar(
    left,
    top,
    maximumWidth,
    currentHitpoints,
    maximumHitpoints
  );
}

void buildingProgressBar(
  Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawBuildingProgressBar(firm);
}

void buildingProgressBar(
  const Town* town
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawTownTrainingProgressBar(town);
}

void buildingRallyButton(
  Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (_7kaaFirm->own_firm()) {
    const auto building
      = Ambition::Building::findBy7kaaFirmRecordNumber(_7kaaFirm->firm_recno);
    if (building) {
      building->drawRallyButton();
    }
  }
}
void buildingRallyButton(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (_7kaaTown->nation_recno == nation_array.player_recno) {
    const auto building
      = Ambition::Building::findBy7kaaTownRecordNumber(_7kaaTown->town_recno);
    if (building) {
      building->drawRallyButton();
    }
  }
}

void buildingRallyPoint(
  Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawBuildingRallyPoint(_7kaaFirm);
}
void buildingRallyPoint(
  Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawBuildingRallyPoint(_7kaaTown);
}

short calculateAnimatedLinePhase(
  const short _7kaaCalculation,
  const int animatedFlag,
  const int lineProgress
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateAnimatedLinePhase(
    animatedFlag,
    lineProgress
  );
}

FirmBitmap* calculateFirmBitmap(
  FirmBitmap* _7kaaCalculation,
  Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateFirmBitmap(_7kaaCalculation, firm);
}

int calculateHitbarBaseColour(
  const int _7kaaCalculation,
  const double maximumHitpoints
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateHitbarBaseColour(maximumHitpoints);
}

int calculateHitbarWidth(
  const int availableWidth,
  const double maximumHitpoints
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return availableWidth;
  }

  return Ambition::calculateHitbarWidth(availableWidth, maximumHitpoints);
}

short calculateRainSpeed(
  const short _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateRainSpeed(_7kaaCalculation);
}

char calculateRockRemainingDelay(
  const char _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateRockRemainingDelay(_7kaaCalculation);
}

char* calculateTerrainBitmap(
  char* _7kaaCalculation,
  const short terrainId,
  const int x,
  const int y
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateTerrainBitmap(terrainId, x, y);
}

char calculateTownFlagNumber(
  const char _7kaaCalculation,
  const int townRecordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateTownFlagNumber(townRecordNumber);
}

int calculateUnitHitbarWidth(
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateUnitHitbarWidth();
}

int calculateUnitIconY(
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateUnitIconY(_7kaaCalculation);
}

int calculateWorkerPortraitLeft(
  const int _7kaaCalculation,
  const int workerIndex
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateWorkerPortraitX(workerIndex);
}

int centreHitbar(
  const int left,
  const int maximumWidth,
  const int currentWidth
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return left;
  }

  return Ambition::centreHitbar(left, maximumWidth, currentWidth);
}

void hitbarOutline(
  const bool isOwn,
  const int x,
  const int y,
  const int width,
  const int height
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawHitbarOutline(isOwn, x, y, width, height);
}

bool initialiseSnowLayer(
  SnowLayer& layer,
  const int level,
  const char animationSpeed,
  double slideSpeed
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::initialiseSnowLayer(layer, level, animationSpeed, slideSpeed);
}

void innGuestCount(
  const short count,
  const int refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawInnGuestCount(count, refreshFlag);
}

void loadMenuDeleteButton(
  Button3D& button,
  const int left,
  const int top
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawLoadMenuDeleteButton(button, left, top);
}

bool minimapViewBoundsBox(
  const MapMatrix* _7kaaMapMatrix
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::Minimap::drawViewBoundsBox(_7kaaMapMatrix);
}

void modeInformation(
) {
  Ambition::drawModeInformation();
}

void printGameSpeed(
  const int speed
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::displayGameSpeed(speed);
}

void printSyncError(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::printSyncError();
}

void printTownQualityOfLife(
  Town* town,
  const int refreshFlag,
  const int displayTop
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::displayTownQualityOfLife(town, refreshFlag, displayTop);
}

void printUnitContribution(
  ::Unit* unit,
  const int left,
  const int top
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::displayUnitContribution(unit, left, top);
}

void unitWaypointsOnWorld(
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->drawWaypointsOnWorld();
  }
}

void unitWaypointsOnMinimap(
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->drawWaypointsOnMinimap();
  }
}

} // namespace _7kaaAmbitionInterface::Draw
