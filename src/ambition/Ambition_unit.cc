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
 * Implementation file for Ambition::Unit.
 */

#include "Ambition_unit.hh"

#include <algorithm>
#include <stdexcept>
#include <vector>

#define _AMBITION_IMPLEMENTATION
#include "OANLINE.h"
#include "OFIRM.h"
#include "OIMGRES.h"
#include "ONATIONA.h"
#include "OREMOTE.h"
#include "OTOWN.h"
#include "OUNIT.h"

#include "Ambition_coordinates.hh"
#include "Ambition_building.hh"
#include "Ambition_repository.hh"
#include "Ambition_user_interface.hh"
#include "Ambition_vga.hh"
#include "format.hh"


namespace Ambition {

std::vector<short> getAvailableBuildersRecordNumbers(
  char nationRecordNumber,
  uint8_t regionId
);


bool sendAvailableBuilderToFirm(
  const Firm* firm
) {
  err_when(firm->builder_recno);

  const auto availableBuildersRecordNumbers = getAvailableBuildersRecordNumbers(
    firm->nation_recno,
    firm->region_id
  );

  if (availableBuildersRecordNumbers.empty()) {
    return true;
  }

  std::vector<std::pair<short, double>> builderDistances;

  for (auto const builderRecordNumber : availableBuildersRecordNumbers) {
    const auto unit = unit_array[builderRecordNumber];

    if (unit->action_mode == ACTION_ASSIGN_TO_FIRM) {
      if (unit->action_para2 == firm->firm_recno) {
        // There is already a builder heading here, so we do not need to do
        // anything now.
        return true;
      } else {
        // This builder is going to another firm, so we cannot use him.
        continue;
      }
    }

    auto distance = misc.points_distance(
      unit->next_x_loc(),
      unit->next_y_loc(),
      firm->loc_x1,
      firm->loc_y1
    );

    if (unit->unit_mode == UNIT_MODE_CONSTRUCT) {
      const auto firm = firm_array[unit->unit_mode_para];
      if (firm->hit_points < firm->max_hit_points) {
        constexpr auto REPAIRING_DISTANCE_PENALTY = 32;
        distance += REPAIRING_DISTANCE_PENALTY;
      }
    }

    builderDistances.push_back(std::make_pair(builderRecordNumber, distance));
  }

  if (builderDistances.empty()) {
    return true;
  }

  std::sort(
    builderDistances.begin(),
    builderDistances.end(),
    [](const auto a, const auto b) {
      return a.second < b.second;
    }
  );

  const auto closestBuilderRecordNumber = builderDistances.front().first;
  const auto closestAvailableBuilder = unit_array[closestBuilderRecordNumber];
  if (closestAvailableBuilder->unit_mode == UNIT_MODE_CONSTRUCT) {
    // Tell the builder to leave the current firm.
    const auto builderFirmRecordNumber = closestAvailableBuilder->unit_mode_para;
    const auto builderFirm = firm_array[builderFirmRecordNumber];

    if (remote.is_enable()) {
      auto shortPtr = (short*)remote.new_send_queue_msg(
        MSG_FIRM_MOBL_BUILDER,
        sizeof(short)
      );
      *shortPtr = builderFirmRecordNumber;
    } else {
      builderFirm->set_builder(0);
    }
  }

  // Tell the builder to go to the new firm.
  short builderRecordNumberArray[1] = { closestBuilderRecordNumber };
  unit_array.assign(
    firm->loc_x1,
    firm->loc_y1,
    0,
    COMMAND_PLAYER,
    builderRecordNumberArray,
    1
  );

  return true;
}


Unit::Unit(
  const unsigned long long recordNumber,
  const short _7kaaSpriteRecordNumber
) :
  Entity(recordNumber),
  _7kaaSpriteRecordNumber(_7kaaSpriteRecordNumber),
  _7kaaSpyRecordNumber(-1),
  diedAt(Time::START),
  insideBuildingRecordNumber(0),
  retiredAt(Time::START),
  status(Status::Active),
  waypoints()
{
  workerIdentifier.clear();
  lastWaypointOrderIssuedAt = Time::START;
}

std::shared_ptr<Unit> Unit::create(
  const short _7kaaSpriteRecordNumber
) {
  return entityRepository.insert(
    std::make_shared<Unit>(
      entityRepository.takeRecordNumber(),
      _7kaaSpriteRecordNumber
    )
  );
}

std::shared_ptr<Unit> Unit::findBy7kaaSpriteRecordNumber(
  const short _7kaaSpriteRecordNumber
) {
  return entityRepository.findEntityBy<Unit>(
    [&_7kaaSpriteRecordNumber](std::shared_ptr<Unit> unit) {
      return unit->status == Status::Active
        && unit->_7kaaSpriteRecordNumber == _7kaaSpriteRecordNumber;
    }
  );
}
std::shared_ptr<Unit> Unit::findBy7kaaSpyRecordNumber(
  const short _7kaaSpyRecordNumber
) {
  return entityRepository.findEntityBy<Unit>(
    [&_7kaaSpyRecordNumber](std::shared_ptr<Unit> unit) {
      return unit->active()
        && unit->_7kaaSpyRecordNumber == _7kaaSpyRecordNumber;
    }
  );
}
std::shared_ptr<Unit> Unit::findBy7kaaWorker(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
) {
  const auto building = Building::findBy7kaaFirm(_7kaaFirm);
  if (!building) {
    return nullptr;
  }

  const WorkerIdentifier workerIdentifier = {
    .extra_para = _7kaaWorker->extra_para,
    .name_id = _7kaaWorker->name_id,
    .race_id = _7kaaWorker->race_id,
    .rank_id = _7kaaWorker->rank_id,
    .skill_id = _7kaaWorker->skill_id,
    .skill_potential = _7kaaWorker->skill_potential,
    .town_recno = _7kaaWorker->town_recno,
    .unit_id = _7kaaWorker->unit_id,
  };
  const auto _7kaaSpyRecordNumber = _7kaaWorker->spy_recno;

  return entityRepository.findEntityBy<Unit>(
    [&_7kaaSpyRecordNumber, &building, &workerIdentifier]
    (std::shared_ptr<Unit> unit) {
      return unit->status == Status::InsideBuilding
        && unit->insideBuildingRecordNumber == building->recordNumber
        && unit->workerIdentifier.extra_para == workerIdentifier.extra_para
        && unit->workerIdentifier.name_id == workerIdentifier.name_id
        && unit->workerIdentifier.race_id == workerIdentifier.race_id
        && unit->workerIdentifier.rank_id == workerIdentifier.rank_id
        && unit->workerIdentifier.skill_id == workerIdentifier.skill_id
        && unit->workerIdentifier.skill_potential == workerIdentifier.skill_potential
        && unit->workerIdentifier.town_recno == workerIdentifier.town_recno
        && unit->workerIdentifier.unit_id == workerIdentifier.unit_id
        && unit->_7kaaSpyRecordNumber == _7kaaSpyRecordNumber;
    }
  );
}

std::shared_ptr<Unit> Unit::getBy7kaaSpriteRecordNumber(
  const short _7kaaSpriteRecordNumber
) {
  auto unit = Unit::findBy7kaaSpriteRecordNumber(_7kaaSpriteRecordNumber);
  if (!unit) {
    unit = Unit::create(_7kaaSpriteRecordNumber);
  }

  return unit;
}


uint8_t Unit::_7kaaRegionId(
  ::Unit* _7kaaUnit
) {
  const auto _7kaaCalculation = _7kaaUnit->region_id();
  if (_7kaaCalculation != 0) {
    return _7kaaCalculation;
  }

  if (_7kaaUnit->unit_mode == UNIT_MODE_CONSTRUCT) {
    return firm_array[_7kaaUnit->unit_mode_para]->region_id;
  }

  if (_7kaaUnit->unit_mode == UNIT_MODE_ON_SHIP) {
    return _7kaaRegionId(unit_array[_7kaaUnit->unit_mode_para]);
  }

  throw std::domain_error(
    format(
      "Unable to determine 7kaa Unit [%d]'s 7kaa region ID with unit mode %d"
      " and parameter %d.",
      _7kaaUnit->sprite_recno,
      _7kaaUnit->unit_mode,
      _7kaaUnit->unit_mode_para
    )
  );
}

bool Unit::canReceiveLeadershipBonus(
  ::Unit* _7kaaUnit
) {
  return (
    (_7kaaUnit->skill.skill_id == SKILL_LEADING
     && _7kaaUnit->rank_id == RANK_SOLDIER)
    || isWarMachine(_7kaaUnit)
  );
}

bool Unit::isReceivingLeadershipBonus(
  ::Unit* _7kaaUnit
) {
  return _7kaaUnit->is_leader_in_range();
}

bool Unit::isWarMachine(
  ::Unit* _7kaaUnit
) {
  return (
    _7kaaUnit->unit_id == UNIT_F_BALLISTA
    || (_7kaaUnit->unit_id >= UNIT_CATAPULT
        && _7kaaUnit->unit_id <= UNIT_EXPLOSIVE_CART)
  );
}

void Unit::sendToDestination(
  std::vector<short> _7kaaUnitRecordNumbers,
  const Waypoint& destination
) {
  const auto location = world.get_loc(
    destination.point.to7kaaCoordinates().x,
    destination.point.to7kaaCoordinates().y
  );

  if (destination.action > Unit::Waypoint::Action::MoveOnly
    && location->is_firm()
  ) {
    const auto target = firm_array[location->firm_recno()];
    if (target->hit_points > 0) {
      const auto nation = nation_array[
        unit_array[_7kaaUnitRecordNumbers[0]]->nation_recno
      ];
      if (nation
        && nation->get_relation_should_attack(target->nation_recno)
      ) {
        unit_array.attack(
          destination.point.to7kaaCoordinates().x,
          destination.point.to7kaaCoordinates().y,
          0,
          &_7kaaUnitRecordNumbers[0],
          _7kaaUnitRecordNumbers.size(),
          COMMAND_PLAYER,
          0
        );
        return;
      }

      const auto unit = unit_array[_7kaaUnitRecordNumbers[0]];
      if (unit->nation_recno == target->nation_recno) {
        if (target->firm_id==FIRM_CAMP) {
          unit_array.assign_to_camp(
            destination.point.to7kaaCoordinates().x,
            destination.point.to7kaaCoordinates().y,
            COMMAND_PLAYER,
            &_7kaaUnitRecordNumbers[0],
            _7kaaUnitRecordNumbers.size()
          );
          return;
        } else if (unit_res[unit->unit_id]->race_id > 0) {
          unit_array.assign(
            destination.point.to7kaaCoordinates().x,
            destination.point.to7kaaCoordinates().y,
            0,
            COMMAND_PLAYER,
            &_7kaaUnitRecordNumbers[0],
            _7kaaUnitRecordNumbers.size()
          );
          return;
        }
      }
    }
  }

  unit_array.move_to(
    destination.point.to7kaaCoordinates().x,
    destination.point.to7kaaCoordinates().y,
    0,
    &_7kaaUnitRecordNumbers[0],
    _7kaaUnitRecordNumbers.size(),
    COMMAND_PLAYER
  );
}


bool Unit::active (
) {
  return status < Status::Retired;
}

void Unit::clearWaypoints(
) {
  waypoints.clear();
}

void Unit::died(
  const Time::Stamp stamp
) {
  status = Status::Dead;
  diedAt = stamp;

  clearWaypoints();
}

void Unit::drawWaypointsOnWorld(
) const {
  const auto waypointIcon = image_icon.get_ptr("WAYPOINT");
  const auto iconOffsetLeft = -(*(short*)waypointIcon / 2);
  const auto iconOffsetTop = -(*(1 + (short*)waypointIcon) / 2);
  const auto noActionIcon = image_icon.get_ptr("NOPICK");

  auto from = currentDestination();
  Coordinates::Point to;

  for (const auto& waypoint : waypoints) {
    to = waypoint.point;

    drawWorldLine(from, to, 0, 1);

    const auto screenTo = UserInterface::fromWorldPoint(to);
    world.zoom_matrix->put_bitmap_clip(
      screenTo.left + iconOffsetLeft,
      screenTo.top + iconOffsetTop,
      waypointIcon
    );
    if (waypoint.action == Waypoint::Action::MoveOnly) {
      world.zoom_matrix->put_bitmap_clip(
        screenTo.left + 2,
        screenTo.top,
        noActionIcon
      );
    }

    from = to;
  }
}

void Unit::drawWaypointsOnMinimap(
) const {
  auto from = currentDestination();
  Coordinates::Point to;

  for (const auto& waypoint : waypoints) {
    to = waypoint.point;
    drawMinimapLine(from, to, 2);
    from = to;
  }
}

void Unit::dropSpyIdentity(
) {
  _7kaaSpyRecordNumber = -1;
}

void Unit::enteredBuilding(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
) {
  status = Status::InsideBuilding;
  _7kaaSpriteRecordNumber = -1;
  _7kaaSpyRecordNumber = _7kaaWorker->spy_recno;
  insideBuildingRecordNumber = Building::getBy7kaaFirm(_7kaaFirm)->recordNumber;
  workerIdentifier = {
    .extra_para = _7kaaWorker->extra_para,
    .name_id = _7kaaWorker->name_id,
    .race_id = _7kaaWorker->race_id,
    .rank_id = _7kaaWorker->rank_id,
    .skill_id = _7kaaWorker->skill_id,
    .skill_potential = _7kaaWorker->skill_potential,
    .town_recno = _7kaaWorker->town_recno,
    .unit_id = _7kaaWorker->unit_id,
  };
}
void Unit::enteredBuilding(
  const Town* _7kaaTown,
  const Time::Stamp stamp
) {
  if (!_7kaaSpyRecordNumber) {
    retired(stamp);
  }

  status = Status::InsideBuilding;
  _7kaaSpriteRecordNumber = -1;
  insideBuildingRecordNumber = Building::getBy7kaaTown(_7kaaTown)->recordNumber;
  workerIdentifier.clear();
}

void Unit::exitedBuilding(
  const ::Unit* _7kaaUnit
) {
  status = Status::Active;
  _7kaaSpriteRecordNumber = _7kaaUnit->sprite_recno;
  insideBuildingRecordNumber = 0;
  workerIdentifier.clear();
}

void Unit::goToNextWaypoint(
) {
  if (waypoints.empty()) {
    return;
  }

  if (Time::now() - lastWaypointOrderIssuedAt <= Time::Interval::ofDays(1)) {
    return;
  }

  const auto waypoint = waypoints.front();
  waypoints.pop_front();
  sendToDestination({ _7kaaSpriteRecordNumber }, waypoint);
  lastWaypointOrderIssuedAt = Time::now();
}

void Unit::migrated(
  const Town *destination
) {
  if (workerIdentifier.town_recno > -1) {
    workerIdentifier.town_recno = destination->town_recno;
  } else {
    insideBuildingRecordNumber = destination->town_recno;
  }
}

void Unit::retired(
  const Time::Stamp stamp
) {
  status = Status::Retired;
  retiredAt = stamp;

  clearWaypoints();
}

void Unit::toggleWaypoint(
  const Waypoint& waypoint
) {
  for (auto iterator = waypoints.begin();
     iterator != waypoints.end();
     iterator++
  ) {
    if (iterator->point == waypoint.point) {
      waypoints.erase(iterator);
      return;
    }
  }

  waypoints.push_back(waypoint);
}


/* Protected functions. */

Coordinates::Point Unit::currentDestination(
) const {
  const auto _7kaaUnit = unit_array[_7kaaSpriteRecordNumber];
  return Coordinates::Point::from7kaaCoordinates(
    {
      .x = _7kaaUnit->result_node_array
        ? _7kaaUnit->result_node_array[_7kaaUnit->result_node_count - 1].node_x
        : _7kaaUnit->go_x_loc(),
      .y = _7kaaUnit->result_node_array
        ? _7kaaUnit->result_node_array[_7kaaUnit->result_node_count - 1].node_y
        : _7kaaUnit->go_y_loc(),
    }
  );
}


void Unit::WorkerIdentifier::clear(
) {
  extra_para = -1;
  name_id = 0;
  race_id = -1;
  rank_id = -1;
  skill_id = -1;
  skill_potential = -1;
  town_recno = -1;
  unit_id = -1;
}


/* Private functions. */

std::vector<short> getAvailableBuildersRecordNumbers(
  char nationRecordNumber,
  uint8_t regionId
) {
  constexpr auto FIRM_HITPOINTS_FRACTION_THRESHOLD = 0.9;

  std::vector<short> availableBuildersRecordNumbers;

  for (auto i = 1; i <= unit_array.size(); i++) {
    if (unit_array.is_deleted(i)) {
         continue;
    }

    const auto unit = unit_array[i];

    if (unit->nation_recno != nationRecordNumber
        || unit->race_id == 0
        || unit->skill.skill_id != SKILL_CONSTRUCTION
      || Unit::_7kaaRegionId(unit) != regionId
        || unit->unit_mode == UNIT_MODE_UNDER_TRAINING
        || (unit->action_mode != ACTION_ASSIGN_TO_FIRM
            && unit->action_mode != ACTION_STOP)
    ) {
         continue;
    }

    if (unit->unit_mode == UNIT_MODE_CONSTRUCT) {
         const auto firm = firm_array[unit->unit_mode_para];
         if (firm->under_construction
             || (firm->hit_points / firm->max_hit_points
                 <= FIRM_HITPOINTS_FRACTION_THRESHOLD)
             || info.game_date <= firm->last_attacked_date + 8
         ) {
            continue;
         }
    }

    availableBuildersRecordNumbers.push_back(i);
  }

  return availableBuildersRecordNumbers;
}

} // namespace Ambition
