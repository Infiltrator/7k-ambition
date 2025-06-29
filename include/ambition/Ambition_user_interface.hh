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

  inline bool operator==(const Point& rhs) const noexcept {
    return left == rhs.left && top == rhs.top;
  }
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
        .left = start.left + size.width - 1,
        .top = start.top + size.height - 1,
      },
    };
  }

  inline bool operator==(const Rectangle& rhs) const noexcept {
    return start == rhs.start && end == rhs.end;
  }

  inline int width() const { return end.left - start.left + 1; }
  inline int height() const { return end.top - start.top + 1; }

  bool contains(
    const Point& point
  ) const;
  bool contains(
    const Rectangle& rectangle
  ) const;

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

  Rectangle intersection(
    const Rectangle with
  ) const;
};


constexpr Size BUTTON_SIZE = {
  .width = 46,
  .height = 38,
};

constexpr Size BUTTON_SIZE_WITH_SHADOW = {
  .width = BUTTON_SIZE.width + 4,
  .height = BUTTON_SIZE.height + 6,
};

constexpr auto BOUNDS = Rectangle::fromPoint(
  {
    .left = 0,
    .top = 0,
  },
  {
    .width = 800,
    .height = 600,
  }
);

const auto VIEWPORT = BOUNDS.internal(
  {
    .width = 576,
    .height = 544,
  },
  HorizontalAlignment::Left,
  VerticalAlignment::Bottom
);

const auto REPORT_AREA = VIEWPORT.inner(6);

namespace MilitaryReport {

namespace UnitList {

const auto AREA = REPORT_AREA.inner(0, 246, 0, 0);

namespace UnitCost {

const auto COLUMN
  = AREA.inner(180, 0, 0, 0)
  .internal({ .width = 100, .height = AREA.height() });
const auto COLUMN_CONTENTS = COLUMN.internal(
  { .width = 40, .height = AREA.height() },
  HorizontalAlignment::Centre
);

} // namespace MilitaryReport::UnitList::UnitCost

namespace TotalCost {

const auto COLUMN
  = AREA.inner(390, 0, 0, 0)
  .internal({ .width = 150, .height = AREA.height() });
const auto COLUMN_CONTENTS = COLUMN.inner(12, 0);
const auto VALUE = COLUMN_CONTENTS.internal(
  { .width = 70, .height = AREA.height() },
  HorizontalAlignment::Left
);
const auto PERCENTAGE = COLUMN_CONTENTS.internal(
  { .width = 50, .height = AREA.height() },
  HorizontalAlignment::Right
);

} // namespace MilitaryReport::UnitList::TotalCost

} // namespace MilitaryReport::UnitList

} // namespace MilitaryReport

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

const auto BUTTON_ROW_LOWER
  = INFO_PANE_CONTENTS
  .outer(0, 1)
  .internal(
    {
      .width = INFO_PANE_CONTENTS.width(),
      .height = BUTTON_SIZE_WITH_SHADOW.height,
    },
    HorizontalAlignment::Left,
    VerticalAlignment::Bottom
  );

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


Size bitmapSize(
  const char* bitmap
);

void drawPanel(
  const Rectangle rectangle
);

std::string keyString(
  const unsigned int keyCode
);

Point fromWorldPoint(
  Ambition::Coordinates::Point worldPoint,
  Ambition::Coordinates::Rectangle viewport = Ambition::Coordinates::viewport()
);

void printParagraph(
  Font& font,
  const std::string text,
  const Rectangle area,
  const int lineSpacing = 2,
  const Clear clear = Clear::None,
  const HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left,
  const VerticalAlignment verticalAlignment = VerticalAlignment::Top
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
