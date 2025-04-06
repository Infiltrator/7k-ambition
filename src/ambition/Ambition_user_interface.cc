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
 * Implementation file for Ambition::UserInterface.
 */

#include "Ambition_user_interface.hh"

#define _AMBITION_IMPLEMENTATION
#include "OWORLD.h"


namespace Ambition::UserInterface {

Rectangle Rectangle::outer(
  const int marginLeft,
  int marginTop,
  int marginRight,
  int marginBottom
) const {
  if (marginTop < 0) {
    marginTop = marginLeft;
  }
  if (marginRight < 0) {
    marginRight = marginLeft;
  }
  if (marginBottom < 0) {
    marginBottom = marginTop;
  }

  return {
    .start = {
      .left = std::max(start.left - marginLeft, BOUNDS.start.left),
      .top = std::max(start.top - marginTop, BOUNDS.start.top),
    },
    .end = {
      .left = std::min(end.left + marginRight, BOUNDS.end.left),
      .top = std::min(end.top + marginBottom, BOUNDS.end.top),
    },
  };
}


Point fromWorldPoint(
  Ambition::Coordinates::Point worldPoint,
  Ambition::Coordinates::Rectangle viewport
) {
  const auto relative = (worldPoint - viewport.topLeft()).asCoordinates();

  return {
    .left = static_cast<int>(ZOOM_X1 + ZOOM_LOC_WIDTH / 2 + (relative.x * 2)),
    .top = static_cast<int>(ZOOM_Y1 + ZOOM_LOC_HEIGHT / 2 - (relative.y * 2)),
  };
}

} // namespace Ambition::UserInterface
