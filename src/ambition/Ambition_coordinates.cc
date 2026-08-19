/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2025–26 Tim Sviridov
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
 * Implementation file for Ambition::Coordinates.
 */

#include "Ambition_coordinates.hh"

#include <limits>

#define _AMBITION_IMPLEMENTATION
#include "pragma_silence_7kaa_warnings.hh"
#include "OWORLD.h"
#pragma GCC diagnostic pop


namespace Ambition::Coordinates {

/** The 7kaa coordinates of the Ambition origin (0, 0).  In other words, where
 * in 7kaa do the Ambition coordinates of (0, 0) refer to. */
constexpr _7kaaCoordinates ORIGIN_7KAA_COORDINATES = {
  .x = 100,
  .y = 100,
};


Interval Point::asInterval(
) const {
  return { .x = x, .y = y };
}

bool Point::within(
  const Rectangle& rectangle
) const {
  return (
    x >= std::min(rectangle.start.x, rectangle.end.x)
    && x <= std::max(rectangle.start.x, rectangle.end.x)
    && y >= std::min(rectangle.start.y, rectangle.end.y)
    && y <= std::max(rectangle.start.y, rectangle.end.y)
  );
}

bool Point::operator==(
  const Point& rhs
) const noexcept {
    return
    x == rhs.x
    && y == rhs.y;
}

Interval Point::operator-(const Point& rhs) const {
  return {
    .x = x - rhs.x,
    .y = y - rhs.y,
  };
}

Point Point::operator+(const Interval& rhs) const {
  return {
    .x = x + rhs.x,
    .y = y + rhs.y,
  };
}
Point& Point::operator+=(const Interval& rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}
Point Point::operator-(const Interval& rhs) const {
  return {
    .x = x - rhs.x,
    .y = y - rhs.y,
  };
}
Point& Point::operator-=(const Interval& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}


Point Interval::asCoordinates(
) const {
  return { .x = x, .y = y };
}

bool Interval::operator==(const Interval &rhs) const noexcept {
  return x == rhs.x && y == rhs.y;
}

Interval Interval::operator+(const Interval& rhs) const {
  return {
    .x = x + rhs.x,
    .y = y + rhs.y,
  };
}
Interval& Interval::operator+=(const Interval& rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}
Interval Interval::operator-(const Interval& rhs) const {
  return {
    .x = x - rhs.x,
    .y = y - rhs.y,
  };
}
Interval& Interval::operator-=(const Interval& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}
Interval Interval::operator*(const long long int rhs) const {
  return {
    .x = x * rhs,
    .y = y * rhs,
  };
}
Interval& Interval::operator*=(const long long int rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}
Interval Interval::operator/(const long long int rhs) const {
  return {
    .x = x / rhs,
    .y = y / rhs,
  };
}
Interval& Interval::operator/=(const long long int rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
}


Rectangle Rectangle::_7kaaTile(
  const Point point
) {
  static_assert(
    SCALING_FACTOR % 2 == 0,
    "SCALING_FACTOR must be even in order for a 7kaa tile to be derived from"
    " a centre Point."
  );

  /* The Point passed for getting a 7kaa tile must not be on the border between
   * 7kaa tiles. */
  const auto corner
    = point - Interval{ SCALING_FACTOR / 2, SCALING_FACTOR / 2 };
  assert(corner.x % SCALING_FACTOR != 0 && corner.y % SCALING_FACTOR != 0);

  return fromPoint(corner, _7KAA_COORDINATE_STEP);
}

Rectangle Rectangle::from7kaaCoordinates(
  const _7kaaCoordinates& _7kaaCoordinates
) {
  return from7kaaRectangle(_7kaaCoordinates, _7kaaCoordinates);
}
Rectangle Rectangle::from7kaaRectangle(
  const _7kaaCoordinates& _7kaaCoordinatesStart,
  const _7kaaCoordinates& _7kaaCoordinatesEnd
) {
  return {
    .start = {
      .x = (_7kaaCoordinatesStart.x - ORIGIN_7KAA_COORDINATES.x) * SCALING_FACTOR
        - SCALING_FACTOR / 2,
      .y = (ORIGIN_7KAA_COORDINATES.y - _7kaaCoordinatesEnd.y) * SCALING_FACTOR
        - SCALING_FACTOR / 2,
    },
    .end = {
      .x = (_7kaaCoordinatesEnd.x - ORIGIN_7KAA_COORDINATES.x) * SCALING_FACTOR
        + SCALING_FACTOR / 2,
      .y = (ORIGIN_7KAA_COORDINATES.y - _7kaaCoordinatesStart.y) * SCALING_FACTOR
        + SCALING_FACTOR / 2,
    },
  };
}

_7kaaCoordinates Rectangle::to7kaaCoordinates(
) const {
  if (width() > SCALING_FACTOR || height() > SCALING_FACTOR) {
    throw std::invalid_argument("Rectangle is too large for 7kaa.");
  }

  const auto rectangle = to7kaaRectangle();
  return {
    .x = rectangle.x1,
    .y = rectangle.y1,
  };
}
_7kaaRectangle Rectangle::to7kaaRectangle(
) const {
  if (width() == 0 || height() == 0) {
    throw std::invalid_argument(
      "Zero-size Rectangle cannot be reliably converted to 7kaa co-ordinates."
    );
  }

  const auto corner = topLeft();
  const auto x1 = static_cast<short>(
    ORIGIN_7KAA_COORDINATES.x
    + (corner.x + SCALING_FACTOR / 2) / SCALING_FACTOR
  );
  const auto y1 = static_cast<short>(
    ORIGIN_7KAA_COORDINATES.y
    - (corner.y - SCALING_FACTOR / 2) / SCALING_FACTOR
  );

  return {
    .x1 = x1,
    .y1 = y1,
    .x2 = static_cast<short>(x1 + (width() - 1) / SCALING_FACTOR),
    .y2 = static_cast<short>(y1 + (height() - 1) / SCALING_FACTOR),
  };
}

Point Rectangle::centre(
) const {
  return ((start.asInterval() + end.asInterval()) / 2).asCoordinates();
}

Point Rectangle::topLeft(
) const {
  return {
    .x = std::min(start.x, end.x),
    .y = std::max(start.y, end.y),
  };
}

Point Rectangle::bottomRight(
) const {
  return {
    .x = std::max(start.x, end.x),
    .y = std::min(start.y, end.y),
  };
}

long int Rectangle::height(
) const {
  return std::abs(start.y - end.y);
}

long int Rectangle::width(
) const {
  return std::abs(start.x - end.x);
}

Rectangle Rectangle::intersection(
  const Rectangle with
) const {
    return {
    .start = {
      .x = std::max(start.x, with.start.x),
      .y = std::max(start.y, with.start.y),
    },
    .end = {
      .x = std::min(end.x, with.end.x),
      .y = std::min(end.y, with.end.y),
    },
  };
}

bool Rectangle::within(
  const Rectangle& rectangle
) const {
  return topLeft().within(rectangle) && bottomRight().within(rectangle);
}


Location* get7kaaLocation(
  const Rectangle& at
) {
  const auto _7kaaCoordinates = at.to7kaaCoordinates();
  return world.get_loc(_7kaaCoordinates.x, _7kaaCoordinates.y);
}

Rectangle viewport(
) {
  const auto zoomMatrix = world.zoom_matrix;

  return Rectangle::from7kaaRectangle(
    {
      .x = static_cast<short>(zoomMatrix->top_x_loc),
      .y = static_cast<short>(zoomMatrix->top_y_loc),
    },
    {
      .x = static_cast<short>(
        zoomMatrix->top_x_loc + zoomMatrix->image_width / ZOOM_LOC_WIDTH
      ),
      .y = static_cast<short>(
        zoomMatrix->top_y_loc + zoomMatrix->image_height / ZOOM_LOC_HEIGHT
      ),
    }
  );
}

} // namespace Ambition::Coordinates
