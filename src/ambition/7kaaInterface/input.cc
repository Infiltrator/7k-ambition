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
 * Implementation file for _7kaaAmbitionInterface::Input.
 */

#define _AMBITION_IMPLEMENTATION
#include "input.hh"

#include "OWORLD.h"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_input.hh"


namespace _7kaaAmbitionInterface::Input {

void calculateScroll(
  int& x,
  int& y
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::calculateScroll(x, y);
}

bool detectModeSelectClick(
) {
  return Ambition::detectModeSelectClick();
}

void setOrClearRallyPoint(
  const short _7kaaX,
  const short _7kaaY,
  const bool allowAction
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto location = world.get_loc(_7kaaX, _7kaaY);
  Ambition::setOrClearRallyPoint(
    { .x =_7kaaX, .y = _7kaaY },
    allowAction && location->explored()
  );
}

} // namespace _7kaaAmbitionInterface::Input
