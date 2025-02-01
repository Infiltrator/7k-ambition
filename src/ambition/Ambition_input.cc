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
 * Implementation file for Ambition::Input.
 */

#include "Ambition_input.hh"

#include "OMOUSE.h"

#include "Ambition_config.hh"

namespace Ambition {

void calculateScroll(
  int& x,
  int& y
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  if (mouse.cur_x <= mouse.bound_x1 + 1) {
    x = -1;
  }
  if (mouse.cur_x >= mouse.bound_x2 - 1) {
    x = 1;
  }
  if (mouse.cur_y <= mouse.bound_y1 + 1) {
    y = -1;
  }
  if (mouse.cur_y >= mouse.bound_y2 - 1) {
    y = 1;
  }
}

} // namespace Ambition
