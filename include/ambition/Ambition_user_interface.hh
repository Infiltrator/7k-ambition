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
 * Header file for Ambition::UserInterface.
 */

#pragma once

#include "Ambition_coordinates.hh"


namespace Ambition::UserInterface {

struct Point {
  int left;
  int top;
};

struct Rectangle {
  Point start;
  Point end;

  constexpr static Rectangle fromPoint(
    const Point start,
    const int width,
    const int height
  ) {
    return {
      .start = start,
      .end = {
        .left = start.left + width,
        .top = start.top + height,
      },
    };
  }

  Rectangle outer(
    const int marginLeft,
    int marginTop = -1,
    int marginRight = -1,
    int marginBottom = -1
  ) const;
};


constexpr Rectangle BOUNDS = {
  .start = {
    .left = 0,
    .top = 0,
  },
  .end = {
    .left = 800,
    .top = 600,
  },
};

constexpr Rectangle INFO_PANE_CONTENTS = {
  .start = {
    .left = 585,
    .top = 265,
  },
  .end = {
    .left = 789,
    .top = 589,
  },
};

constexpr auto RALLY_POINT_BUTTON = Rectangle::fromPoint(
  {
    .left = INFO_PANE_CONTENTS.start.left + 29,
    .top = INFO_PANE_CONTENTS.start.top,
  },
  24,
  20
);


Point fromWorldPoint(
  Ambition::Coordinates::Point worldPoint,
  Ambition::Coordinates::Rectangle viewport = Ambition::Coordinates::viewport()
);

} // namespace Ambition::UserInterface
