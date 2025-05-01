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

class Font;


namespace Ambition::UserInterface {

enum class Clear {
  None,
  TextArea,
  EntireArea,
};

enum class HorizontalAlignment {
  Left,
  Centre,
  Right,
};
enum class VerticalAlignment {
  Top,
  Centre,
  Bottom,
};

struct Point {
  int left;
  int top;
};

struct Size {
  int width;
  int height;
};

struct Rectangle {
  Point start;
  Point end;

  constexpr static Rectangle fromPoint(
    const Point start,
    const int width,
    const int height
  ) {
    return fromPoint(start, { .width = width, .height = height });
  }
  constexpr static Rectangle fromPoint(
    const Point start,
    Size size
  ) {
    return {
      .start = start,
      .end = {
        .left = start.left + size.width,
        .top = start.top + size.height,
      },
    };
  }

  inline int width() const { return end.left - start.left; }
  inline int height() const { return end.top - start.top; }

  Rectangle inner(
    int paddingLeft,
    int paddingTop = -1,
    int paddingRight = -1,
    int paddingBottom = -1
  ) const;

  Rectangle outer(
    const int marginLeft,
    int marginTop = -1,
    int marginRight = -1,
    int marginBottom = -1
  ) const;

  Rectangle internal(
    const Size size,
    const HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left,
    const VerticalAlignment verticalAlignment = VerticalAlignment::Top
  ) const;
};


constexpr Size BUTTON_SIZE = {
  .width = 46,
  .height = 40,
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
  {
    .width = 24,
    .height = 20,
  }
);


Point fromWorldPoint(
  Ambition::Coordinates::Point worldPoint,
  Ambition::Coordinates::Rectangle viewport = Ambition::Coordinates::viewport()
);

void printText(
  Font& font,
  const std::string text,
  const Rectangle area,
  const Clear clear = Clear::None,
  const HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left,
  const VerticalAlignment verticalAlignment = VerticalAlignment::Top
);

} // namespace Ambition::UserInterface
