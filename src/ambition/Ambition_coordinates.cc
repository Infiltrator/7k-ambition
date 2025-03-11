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
 * Implementation file for Ambition::Coordinates.
 */

#include "Ambition_coordinates.hh"

#include <limits>

#define _AMBITION_IMPLEMENTATION
#include "OWORLD.h"


namespace Ambition::Coordinates {

/** The 7kaa coordinates of the Ambition origin (0, 0).  In other words, where
 * in 7kaa do the Ambition coordinates of (0, 0) refer to. */
constexpr _7kaaCoordinates ORIGIN_7KAA_COORDINATES = {
  .x = 100,
  .y = 100,
};

constexpr auto SCALING_FACTOR = 16;


Interval Point::asInterval(
) const {
  return { .x = x, .y = y };
}

Point Point::from7kaaCoordinates(
  const _7kaaCoordinates& _7kaaCoordinates
) {
  return {
    .x = (_7kaaCoordinates.x - ORIGIN_7KAA_COORDINATES.x) * SCALING_FACTOR,
    .y = (ORIGIN_7KAA_COORDINATES.y - _7kaaCoordinates.y) * SCALING_FACTOR,
  };
}

_7kaaCoordinates Point::to7kaaCoordinates(
) const {
  const Point temp = {
    .x = ORIGIN_7KAA_COORDINATES.x + x / SCALING_FACTOR,
    .y = ORIGIN_7KAA_COORDINATES.y - y / SCALING_FACTOR,
  };

  if (temp.x > std::numeric_limits<short>::max()
    || temp.x < std::numeric_limits<short>::min()
    || temp.y > std::numeric_limits<short>::max()
    || temp.y < std::numeric_limits<short>::min()
  ) {
    throw std::range_error("Coordinates out of range for 7kaa.");
  }

  return {
    .x = short(temp.x),
    .y = short(temp.y),
  };
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

bool Point::operator==(
  const _7kaaCoordinates &rhs
) const noexcept {
  return *this == Point::from7kaaCoordinates(rhs);
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

Rectangle viewport(
) {
  const auto zoomMatrix = world.zoom_matrix;

  return {
    .start = Point::from7kaaCoordinates(
      {
        .x = short(zoomMatrix->top_x_loc),
        .y = short(zoomMatrix->top_y_loc),
      }
    ),
    .end = Point::from7kaaCoordinates(
      {
        .x = short(zoomMatrix->top_x_loc + zoomMatrix->cur_cargo_width),
        .y = short(zoomMatrix->top_y_loc + zoomMatrix->cur_cargo_height),
      }
    ),
  };
}

} // namespace Ambition::Coordinates
