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
 * Implementation file for Ambition::Population.
 */

#include "Ambition_population.hh"

#include <iterator>
#include <numeric>
#include <ranges>

#define _AMBITION_IMPLEMENTATION
#include "ONATION.h"
#include "OTOWN.h"

#include "7kaaInterface/population.hh"


namespace Ambition::Population {

double averageTargetResistance(
  const Town* _7kaaTown,
  const Nation* _7kaaNation
) {
  const auto _7kaaRaceIndices = std::views::iota(0, MAX_RACE);

  return (
    std::transform_reduce(
      _7kaaRaceIndices.begin(),
      _7kaaRaceIndices.end(),
      0.0,
      std::plus{},
      [&_7kaaTown, &_7kaaNation](const auto _7kaaRaceIndex) {
        const auto _7kaaTargetResistanceValue
          = _7kaaTown->race_target_resistance_array[_7kaaRaceIndex]
            [nation_array.player_recno - 1];
        const auto _7kaaTargetResistance
          = _7kaaTargetResistanceValue >= 0
          ? _7kaaTargetResistanceValue
          : _7kaaTown->race_resistance_array[_7kaaRaceIndex]
            [nation_array.player_recno - 1];

        return (
          _7kaaAmbitionInterface::Population::calculateTargetResistance(
            _7kaaTargetResistance,
            _7kaaTown,
            _7kaaNation,
            _7kaaRaceIndex + 1
          ) * _7kaaTown->race_pop_array[_7kaaRaceIndex]
        );
      }
    ) / _7kaaTown->population
  );
}

int raceCount(
  const Town* _7kaaTown
) {
  return std::transform_reduce(
    std::cbegin(_7kaaTown->race_pop_array),
    std::cend(_7kaaTown->race_pop_array),
    0,
    std::plus{},
    [](const auto population) {
      return population > 0;
    }
  );
}

} // namespace Ambition::Population
