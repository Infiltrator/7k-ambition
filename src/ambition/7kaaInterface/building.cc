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

#include "OF_RESE.h"
#include "OFIRM.h"
#include "OTOWN.h"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_coordinates.hh"
#include "Ambition_polity.hh"
#include "Ambition_unit.hh"


namespace _7kaaAmbitionInterface::Building {

void clearRallyPoint(
  const Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto _7kaaFirmRecordNumber = _7kaaFirm->firm_recno;
  auto building
    = Ambition::Building::findBy7kaaFirmRecordNumber(_7kaaFirmRecordNumber);
  if (building) {
    building->clearRallyPoint();
  }
}
void clearRallyPoint(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto _7kaaTownRecordNumber = _7kaaTown->town_recno;
  auto building
    = Ambition::Building::findBy7kaaTownRecordNumber(_7kaaTownRecordNumber);
  if (building) {
    building->clearRallyPoint();
  }
}

void destroy(
  const Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto _7kaaFirmRecordNumber = _7kaaFirm->firm_recno;
  auto building
    = Ambition::Building::findBy7kaaFirmRecordNumber(_7kaaFirmRecordNumber);
  if (building) {
    building->destroy(Ambition::Time::now());
  }
}
void destroy(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto _7kaaTownRecordNumber = _7kaaTown->town_recno;
  auto building
    = Ambition::Building::findBy7kaaTownRecordNumber(_7kaaTownRecordNumber);
  if (building) {
    building->destroy(Ambition::Time::now());
  }
}

void processIdleTowerOfScience(
  FirmResearch* _7kaaFirmResearch
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (_7kaaFirmResearch->tech_id) {
    return;
  }

  const auto _7kaaNationRecordNumber = _7kaaFirmResearch->nation_recno;
  const auto polity
    = Ambition::Polity::findBy7kaaRecordNumber(_7kaaNationRecordNumber);
  if (polity) {
    const auto researchTarget = polity->researchTarget();
    if (researchTarget) {
      _7kaaFirmResearch->start_research(researchTarget, COMMAND_PLAYER);
    }
  }
}

void sendUnitsToRallyPoint(
  const Firm* _7kaaFirm,
  std::vector<short> _7kaaUnitRecordNumbers
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto building
    = Ambition::Building::findBy7kaaFirmRecordNumber(_7kaaFirm->firm_recno);
  if (building) {
    building->sendUnitsToRallyPoint(_7kaaUnitRecordNumbers);
  }
}
void sendUnitsToRallyPoint(
  const Town* _7kaaTown,
  std::vector<short> _7kaaUnitRecordNumbers
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto building
    = Ambition::Building::findBy7kaaTownRecordNumber(_7kaaTown->town_recno);
  if (building) {
    building->sendUnitsToRallyPoint(_7kaaUnitRecordNumbers);
  }
}

} // namespace _7kaaAmbitionInterface::Building
