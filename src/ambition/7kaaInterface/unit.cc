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

#include "pragma_silence_7kaa_warnings.hh"
#include "OFIRM.h"
#include "OMOUSE.h"
#include "OPOWER.h"
#include "OUNIT.h"
#include "OSPY.h"
#pragma GCC diagnostic pop

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

int canSendBuilderToFirm(
  const Firm* _7kaaFirm,
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  if (!(mouse.skey_state & CONTROL_KEY_MASK)) {
    return _7kaaCalculation;
  }

  if (_7kaaFirm->builder_recno) {
    return 0;
  }

  return Ambition::builderInRegion(_7kaaFirm->nation_recno, _7kaaFirm->region_id);
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
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->dropSpyIdentity();
  }
}
void dropSpyIdentity(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto unit = Ambition::Unit::findBy7kaaWorker(_7kaaFirm, _7kaaWorker);
  if (unit) {
    unit->dropSpyIdentity();
  }
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

  return Ambition::sendAvailableBuilderToFirm(
    firm,
    mouse.skey_state & CONTROL_KEY_MASK
  );
}

void selectUnits(
  const std::vector<short> _7kaaUnitRecordNumbers
) {
  power.reset_selection();

  for (const auto _7kaaUnitRecordNumber : _7kaaUnitRecordNumbers) {
    if (unit_array.is_deleted(_7kaaUnitRecordNumber)) {
      continue;
    }

    auto _7kaaUnit = unit_array[_7kaaUnitRecordNumber];
    _7kaaUnit->selected_flag = true;
    unit_array.selected_count++;
    if (!unit_array.selected_recno) {
      unit_array.selected_recno = _7kaaUnitRecordNumber;
    }
  }

  info.disp();
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

  if (location->has_any_unit()) {
    const auto targetUnit = unit_array[
      location->unit_recno(location->has_any_unit())
    ];
    if (targetUnit->is_own() && targetUnit->unit_id == UNIT_EXPLOSIVE_CART) {
      return false;
    }
  }

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
