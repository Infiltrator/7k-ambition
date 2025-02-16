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
 * Implementation file for _7kaaAmbitionInterface::Spy.
 */

#define _AMBITION_IMPLEMENTATION
#include "spy.hh"

#include "OSPY.h"

#include "Ambition_config.hh"


namespace _7kaaAmbitionInterface::Spy {

constexpr auto BRIBE_AMOUNT_STEP = 500;


int bribeChoiceCount(
) {
  constexpr auto _7KAA_BRIBE_CHOICE_COUNT = 6;

  if (!Ambition::config.enhancementsAvailable()) {
    return _7KAA_BRIBE_CHOICE_COUNT;
  }

  return MAX_BRIBE_AMOUNT / BRIBE_AMOUNT_STEP;
}

int bribeChoiceAmount(
  const int choice,
  const short _7kaaBribeAmountArray[6]
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaBribeAmountArray[choice];
  }

  return BRIBE_AMOUNT_STEP * (choice + 1);
}

} // namespace _7kaaAmbitionInterface::Spy
