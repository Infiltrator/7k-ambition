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
#include <vector>

#define _AMBITION_IMPLEMENTATION
#include "OFIRM.h"
#include "OREMOTE.h"
#include "OUNIT.h"


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
        || (unit->is_visible() && unit->region_id() != regionId)
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
