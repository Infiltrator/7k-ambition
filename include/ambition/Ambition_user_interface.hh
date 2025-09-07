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

#include "COLOR.h"

#include "Ambition_coordinates.hh"

class Font;


namespace Ambition::UserInterface {

enum class BuildingMenu {
  _7kaa,

  AssassinationConfirmation,
  StealReportConfirmation,
};

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


extern int rankReportComparison7kaaNationRecordNumber;

extern BuildingMenu buildingMenu;
extern int reportType;
extern short selected7kaaFirmRecordNumber;


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


namespace ScenarioList {

const auto HEADING_AREA = BOUNDS.internal(
  {
    .width = BOUNDS.width(),
    .height = 172,
  }
);

const auto DESCRIPTION_AREA
  = BOUNDS
  .inner(0, HEADING_AREA.height(), 0, 0)
  .internal({ .width = BOUNDS.width(), .height = 148 });

const auto LIST_AREA
  = BOUNDS
  .inner(0, HEADING_AREA.height() + DESCRIPTION_AREA.height(), 0, 0)
  .internal({ .width = BOUNDS.width(), .height = 280 });

} // namespace ScenarioList

namespace Report {

const auto AREA = VIEWPORT.inner(6);

namespace Villages {

namespace Towns {

const auto AREA = Report::AREA.internal(
  {
    .width = Report::AREA.width(),
    .height = 301,
  }
);

const auto VILLAGERS_COLUMN
  = AREA
  .inner(184, 0, 0, 0)
  .internal({ .width = 14, .height = AREA.height() });

const auto PEASANTS_COLUMN
  = AREA
  .inner(250, 0, 0, 0)
  .internal({ .width = 14, .height = AREA.height() });

const auto UPPER_TOTAL_AREA
  = AREA
  .inner(0, 0, 0, 20)
  .internal(
    {
      .width = AREA.width(),
      .height = 19,
    },
    HorizontalAlignment::Centre,
    VerticalAlignment::Bottom
  );
const auto UPPER_TOTAL_TEXT_AREA
  = UPPER_TOTAL_AREA
  .inner(2, 2, 3, 3)
  .inner(7, 0);

const auto SURPLUS_TEXT_AREA
  = UPPER_TOTAL_TEXT_AREA
  .inner(360, 0, 0, 0)
  .internal({ .width = 200, .height = 16 });

} // namespace Report::Villages::Towns

} // namespace Report::Villages

namespace Rank {

namespace Scores {

const Size SIZE = { .width = Report::AREA.width(), .height = 151 };
const auto AREA
  = Report::AREA.inner(0, 226, 0, 0)
  .internal(SIZE);

const auto COMPARISON_AREA = AREA.inner(372, 0, 0, 0);
const auto COMPARISON_TEXT_AREA = COMPARISON_AREA.inner(6);

constexpr auto COLUMN_GAP = 8;

const auto COMPARISON_SCORE_COLUMN = COMPARISON_TEXT_AREA.internal(
  { .width = 40, .height = COMPARISON_TEXT_AREA.height() }
);
const auto COMPARISON_SCORE_DIFFERENCE_COLUMN
  = COMPARISON_TEXT_AREA
  .inner(COMPARISON_SCORE_COLUMN.width() + COLUMN_GAP, 0, 0, 0)
  .internal({ .width = 56, .height = COMPARISON_TEXT_AREA.height() });

const auto COMPARISON_BUTTON
  = COMPARISON_TEXT_AREA
  .outer(0, 0, 0, 4)
  .internal(
    { .width = COMPARISON_TEXT_AREA.width(), .height = 40 },
    HorizontalAlignment::Centre,
    VerticalAlignment::Bottom
  );

} // namespace Report::Rank::Scores

} // namespace Report::Rank

} // namespace Report

namespace MilitaryReport {

namespace UnitList {

const auto AREA = Report::AREA.inner(0, 246, 0, 0);

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

constexpr Ambition::UserInterface::Size SAVE_SLOT_SIZE = {
  .width = 538,
  .height = 62,
};
constexpr Ambition::UserInterface::Size SAVE_INFORMATION_TEXT_AREA_SIZE = {
  .width = 190,
  .height = 17,
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

const auto ASSASSINATION_BUTTON = BUTTON_ROW_LOWER.internal(
  BUTTON_SIZE_WITH_SHADOW,
  HorizontalAlignment::Left,
  VerticalAlignment::Bottom
);

const auto CANCEL_BUTTON = BUTTON_ROW_LOWER.internal(
  BUTTON_SIZE_WITH_SHADOW,
  HorizontalAlignment::Right,
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

const auto DISCORD_BUTTON = BOUNDS
  .inner(4)
  .internal(
    { .width = 132, .height = 20 },
    HorizontalAlignment::Left,
    VerticalAlignment::Bottom
  );
const auto DISCORD_BUTTON_CLICK_AREA = DISCORD_BUTTON.outer(4, 2, 4, 4);

const auto FEEDBACK_LINK = BOUNDS
  .inner(0, 0, 0, DISCORD_BUTTON.outer(4, 2, 4, 4).height())
  .inner(12, 0, 0, 2)
  .internal(
    { .width = 132, .height = 20 },
    HorizontalAlignment::Left,
    VerticalAlignment::Bottom
  );
const auto FEEDBACK_LINK_CLICK_AREA = FEEDBACK_LINK.outer(4, 4, 4, 2);


Size bitmapSize(
  const char* bitmap
);

bool detectMouseClick(
  const Rectangle area
);

void drawRectangle(
  const Rectangle& rectangle,
  const int colourIndex = V_BLACK
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

void resetState(
);

} // namespace Ambition::UserInterface
