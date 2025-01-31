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
 * Implementation file for Ambition::Minimap.
 */

#include "Ambition_minimap.hh"

#include "COLOR.h"
#include "OVGABUF.h"
#include "OWORLDMT.h"

#include "Ambition_config.hh"

namespace Ambition {

bool Minimap::drawViewBoundsBox(
  const MapMatrix* _7kaaMapMatrix
) {
  if (!config.enhancementsAvailable()) {
    return false;
  }

  constexpr auto COLOUR_COUNT = 7;
  constexpr auto FRAMES_PER_COLOUR = 5;
  constexpr auto FULL_CYCLE_COUNT = COLOUR_COUNT * FRAMES_PER_COLOUR * 2;
  constexpr auto HALF_CYCLE_COUNT = FULL_CYCLE_COUNT / 2;

  static auto frameCount = HALF_CYCLE_COUNT;

  const auto x1
    = _7kaaMapMatrix->image_x1
    + (_7kaaMapMatrix->loc_width
       * (_7kaaMapMatrix->cur_x_loc - _7kaaMapMatrix->top_x_loc));
  const auto y1
    = _7kaaMapMatrix->image_y1
    + (_7kaaMapMatrix->loc_height
       * (_7kaaMapMatrix->cur_y_loc - _7kaaMapMatrix->top_y_loc));
  const auto x2
    = x1
    + (_7kaaMapMatrix->loc_width * _7kaaMapMatrix->cur_cargo_width)
    - 1;
  const auto y2
    = y1
    + (_7kaaMapMatrix->loc_height * _7kaaMapMatrix->cur_cargo_height)
    - 1;
  /** Cycle from 0 to COLOUR_COUNT - 1 and then back down again, giving a
      colour-flashing effect. */
  const auto colourOffset
    = std::abs((frameCount / FRAMES_PER_COLOUR) - HALF_CYCLE_COUNT);
  vga_back.rect(x1, y1, x2, y2, 1, VGA_YELLOW + colourOffset);

  frameCount = (frameCount + 1) % FULL_CYCLE_COUNT;

  return true;
}

} // namespace Ambition
