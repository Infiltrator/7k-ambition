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
 * Implementation file for _7kaaAmbitionInterface::Unit.
 */

#define _AMBITION_IMPLEMENTATION
#include "7kaaInterface/unit.hh"

#include "OFIRM.h"
#include "OUNIT.h"
#include "OSPY.h"

#include "Ambition_config.hh"
#include "Ambition_coordinates.hh"
#include "Ambition_time.hh"
#include "Ambition_unit.hh"


namespace _7kaaAmbitionInterface::Unit {

void clearWaypoints(
  const std::vector<short> _7kaaUnitRecordNumbers
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  for (const auto _7kaaUnitRecordNumber : _7kaaUnitRecordNumbers) {
    auto unit
      = Ambition::Unit::getBy7kaaSpriteRecordNumber(_7kaaUnitRecordNumber);
    unit->clearWaypoints();
  }
}

void died(
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->died(Ambition::Time::now());
  }
}
void died(
  const Spy* _7kaaSpy
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit
    = Ambition::Unit::findBy7kaaSpyRecordNumber(_7kaaSpy->spy_recno);
  if (unit) {
    unit->died(Ambition::Time::now());
  }
}

void dropSpyIdentity(
  const ::Unit* _7kaaUnit
) {
  auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  unit->dropSpyIdentity();
}
void dropSpyIdentity(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
) {
  auto unit = Ambition::Unit::findBy7kaaWorker(_7kaaFirm, _7kaaWorker);
  unit->dropSpyIdentity();
}

void enteredBuilding(
  const ::Unit* _7kaaUnit,
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->enteredBuilding(_7kaaFirm, _7kaaWorker);
  }
}
void enteredBuilding(
  const ::Unit* _7kaaUnit,
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->enteredBuilding(_7kaaTown, Ambition::Time::now());
  }
}

void exitedBuilding(
  const Worker* _7kaaWorker,
  const Firm* _7kaaFirm,
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit = Ambition::Unit::findBy7kaaWorker(_7kaaFirm, _7kaaWorker);
  if (unit) {
    unit->exitedBuilding(_7kaaUnit);
  }
}
void exitedBuilding(
  const Spy* _7kaaSpy,
  const Town* _7kaaTown,
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit = Ambition::Unit::findBy7kaaSpyRecordNumber(_7kaaSpy->spy_recno);
  if (unit) {
    unit->exitedBuilding(_7kaaUnit);
  }
}

void goToNextWaypoint(
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->goToNextWaypoint();
  }
}

void migrated(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker,
  const Town* destination
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit = Ambition::Unit::findBy7kaaWorker(_7kaaFirm, _7kaaWorker);
  if (unit) {
    unit->migrated(destination);
  }
}
void migrated(
  const Spy* _7kaaSpy,
  const Town* destination
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit = Ambition::Unit::findBy7kaaSpyRecordNumber(_7kaaSpy->spy_recno);
  if (unit) {
    unit->migrated(destination);
  }
}

void retired(
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->retired(Ambition::Time::now());
  }
}
void retired(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit = Ambition::Unit::findBy7kaaWorker(_7kaaFirm, _7kaaWorker);
  if (unit) {
    unit->retired(Ambition::Time::now());
  }
}

bool sendAvailableBuilderToFirm(
  const Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::sendAvailableBuilderToFirm(firm);
}

bool toggleWaypoint(
  const std::vector<short> _7kaaUnitRecordNumbers,
  const short _7kaaX,
  const short _7kaaY,
  const bool allowAction
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  const auto location = world.get_loc(_7kaaX, _7kaaY);
  for (const auto _7kaaUnitRecordNumber : _7kaaUnitRecordNumbers) {
    auto unit
      = Ambition::Unit::getBy7kaaSpriteRecordNumber(_7kaaUnitRecordNumber);
    unit->toggleWaypoint(
      {
        .action = (allowAction && location->explored())
          ? Ambition::Unit::Waypoint::Action::InteractWithBuilding
          : Ambition::Unit::Waypoint::Action::MoveOnly,
        .point = Ambition::Coordinates::Point::from7kaaCoordinates(
          {
            .x = _7kaaX,
            .y = _7kaaY,
          }
        ),
      }
    );
  }

  return true;
}

} // namespace _7kaaAmbitionInterface::Unit
