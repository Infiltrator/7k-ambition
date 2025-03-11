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
 * Implementation file for _7kaaAmbitionInterface::Building.
 */

#define _AMBITION_IMPLEMENTATION
#include "7kaaInterface/building.hh"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_coordinates.hh"
#include "Ambition_unit.hh"


namespace _7kaaAmbitionInterface::Building {

void clearRallyPoint(
  const Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::clearRallyPoint(_7kaaFirm);
}
void clearRallyPoint(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::clearRallyPoint(_7kaaTown);
}

void destroy(
  const Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::destroy(_7kaaFirm);
}
void destroy(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::destroy(_7kaaTown);
}

void sendUnitsToRallyPoint(
  const Firm* _7kaaFirm,
  std::vector<short> _7kaaUnitRecordNumbers
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::Unit::sendToBuildingRallyPoint(_7kaaUnitRecordNumbers, _7kaaFirm);
}
void sendUnitsToRallyPoint(
  const Town* _7kaaTown,
  std::vector<short> _7kaaUnitRecordNumbers
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::Unit::sendToBuildingRallyPoint(_7kaaUnitRecordNumbers, _7kaaTown);
}

} // namespace _7kaaAmbitionInterface::Building
