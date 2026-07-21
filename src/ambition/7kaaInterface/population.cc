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
 * Implementation file for _7kaaAmbitionInterface::Population.
 */

#define _AMBITION_IMPLEMENTATION
#include "population.hh"

#include "pragma_silence_7kaa_warnings.hh"
#include "ONATION.h"
#include "OTOWN.h"
#pragma GCC diagnostic pop

#include "Ambition_config.hh"
#include "Ambition_population.hh"


namespace _7kaaAmbitionInterface::Population {

int calculateTargetResistance(
  const int _7kaaCalculation,
  const Town* _7kaaTown,
  const Nation* _7kaaNation,
  const int _7kaaRaceId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  if (!_7kaaNation) {
    return _7kaaCalculation;
  }

  const auto nationIndex = _7kaaNation->nation_recno - 1;
  const auto currentResistance
    = _7kaaTown->race_resistance_array[_7kaaRaceId - 1][nationIndex];
  const auto targetResistance
    = _7kaaTown->race_target_resistance_array[_7kaaRaceId - 1][nationIndex];

  const auto polityHasWorkers = Ambition::Population::workingForPolity(
    _7kaaTown,
    _7kaaRaceId,
    _7kaaNation
  );

  if ((targetResistance == -1 || targetResistance >= currentResistance)
    && polityHasWorkers
  ) {
    return 0;
  }

  return _7kaaCalculation;
}

} // namespace _7kaaAmbitionInterface::Population
