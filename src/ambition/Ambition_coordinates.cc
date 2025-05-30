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


Interval Point::asInterval(
) const {
  return { .x = x, .y = y };
}

Point Point::from7kaaCoordinates(
  const _7kaaCoordinates& _7kaaCoordinates
) {
  return (
    Interval {
      .x = (_7kaaCoordinates.x - ORIGIN_7KAA_COORDINATES.x),
      .y = (ORIGIN_7KAA_COORDINATES.y - _7kaaCoordinates.y),
    } * SCALING_FACTOR + _7KAA_OFFSET
  ).asCoordinates();
}

_7kaaCoordinates Point::to7kaaCoordinates(
) const {
  const auto point = (*this - _7KAA_OFFSET).asInterval() / SCALING_FACTOR;
  const Point temp = {
    .x = ORIGIN_7KAA_COORDINATES.x + point.x,
    .y = ORIGIN_7KAA_COORDINATES.y - point.y,
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


Rectangle Rectangle::from7kaaCoordinates(
  const _7kaaCoordinates& _7kaaCoordinates
) {
  return fromPoint(
    (
      Interval {
        .x = (_7kaaCoordinates.x - ORIGIN_7KAA_COORDINATES.x),
        .y = (ORIGIN_7KAA_COORDINATES.y - _7kaaCoordinates.y),
      } * SCALING_FACTOR + _7KAA_OFFSET * 2
    ).asCoordinates(),
    {
      .x = ZOOM_LOC_WIDTH,
      .y = -ZOOM_LOC_HEIGHT,
    }
  );
}

Point Rectangle::point(
  const long int index,
  const Interval step
) const {
  const auto low = Point {
    std::min(start.x, end.x),
    std::min(start.y, end.y),
  };
  const auto high = Point {
    std::max(start.x, end.x),
    std::max(start.y, end.y),
  };

  const auto stepCountX = (high.x - low.x) / step.x + 1;

  return {
    low.x + (index % stepCountX) * step.x,
    low.y + (index / stepCountX) * step.y,
  };
}
long int Rectangle::pointCount(
  const Interval step
) const {
  return ((std::max(start.x, end.x) - std::min(start.x, end.x)) / step.x + 1)
    * ((std::max(start.y, end.y) - std::min(start.y, end.y)) / step.y + 1);
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
  return std::abs(start.y - end.y);
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

Rectangle viewport(
) {
  const auto zoomMatrix = world.zoom_matrix;

  return {
    .start = Rectangle::from7kaaCoordinates(
      {
        .x = static_cast<short>(zoomMatrix->top_x_loc),
        .y = static_cast<short>(zoomMatrix->top_y_loc),
      }
    ).topLeft(),
    .end = Rectangle::from7kaaCoordinates(
      {
        .x = static_cast<short>(
          zoomMatrix->top_x_loc + zoomMatrix->image_width / ZOOM_LOC_WIDTH
        ),
        .y = static_cast<short>(
          zoomMatrix->top_y_loc + zoomMatrix->image_height / ZOOM_LOC_HEIGHT
        ),
      }
    ).bottomRight(),
  };
}

} // namespace Ambition::Coordinates
