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
 * Header file for Ambition::Coordinates.
 */

#pragma once

#include <boost/serialization/nvp.hpp>


namespace Ambition::Coordinates {

constexpr auto SCALING_FACTOR = 16;


struct _7kaaCoordinates {
  short x;
  short y;
};


struct Interval;
struct Rectangle;

struct Point {
  long long int x;
  long long int y;

  static Point from7kaaCoordinates(
    const _7kaaCoordinates& _7kaaCoordinates
  );

  Interval asInterval(
  ) const;

  _7kaaCoordinates to7kaaCoordinates(
  ) const;

  bool within(
    const Rectangle& rectangle
  ) const;

  bool operator==(const Point& rhs) const noexcept;

  bool operator==(const _7kaaCoordinates& rhs) const noexcept;

  Interval operator-(const Point& rhs) const;

  Point operator+(const Interval& rhs) const;
  Point& operator+=(const Interval& rhs);
  Point operator-(const Interval& rhs) const;
  Point& operator-=(const Interval& rhs);

  template<class Archive>
  void serialize(
    Archive& archive,
    const unsigned int version
  ) {
    archive & BOOST_SERIALIZATION_NVP(x);
    archive & BOOST_SERIALIZATION_NVP(y);
  }
};

struct Interval {
  long long int x;
  long long int y;

  Point asCoordinates(
  ) const;

  bool operator==(const Interval& rhs) const noexcept;

  Interval operator+(const Interval& rhs) const;
  Interval& operator+=(const Interval& rhs);
  Interval operator-(const Interval& rhs) const;
  Interval& operator-=(const Interval& rhs);
  Interval operator*(const long long int rhs) const;
  Interval& operator*=(const long long int rhs);
  Interval operator/(const long long int rhs) const;
  Interval& operator/=(const long long int rhs);

  template<class Archive>
  void serialize(
    Archive& archive,
    const unsigned int version
  ) {
    archive & BOOST_SERIALIZATION_NVP(x);
    archive & BOOST_SERIALIZATION_NVP(y);
  }
};

struct Rectangle {
  Point start;
  Point end;

  constexpr static Rectangle fromPoint(
    const Point start,
    const Interval size
  ) {
    return {
      .start = start,
      .end = {
        .x = start.x + size.x,
        .y = start.y + size.y,
      },
    };
  }

  bool operator==(const Rectangle& rhs) const noexcept {
    return start == rhs.start && end == rhs.end;
  }

  Point point(
    const long int index,
    const Interval step
  ) const;
  long int pointCount(
    const Interval step
  ) const;

private:
  struct PointRange;
public:
  PointRange points(
    const Interval step = { 1, 1 }
  ) const {
      return PointRange(*this, step);
  }

  Point centre() const;
  Point topLeft() const;
  Point bottomRight() const;

  long int height() const;
  long int width() const;

  Rectangle intersection(
    const Rectangle with
  ) const;

private:
  struct PointRange {
    auto begin() { return Iterator(rectangle, step, 0); }
    auto end() { return Iterator(rectangle, step, rectangle.pointCount(step)); }
    PointRange(
      const Rectangle& rectangle,
      const Interval step
    ): rectangle(rectangle),
       step(step)
    { }

  private:
    struct Iterator {
      inline auto operator*() const { return rectangle.point(index, step); }
      inline auto& operator++() { index++; return *this; }
      inline bool operator!=(const Iterator& rhs) {
        return rectangle != rhs.rectangle
          || step != rhs.step
          || index != rhs.index;
      }

      Iterator(
        const Rectangle& rectangle,
        const Interval step,
        const unsigned int index
      ): rectangle(rectangle),
         step(step),
         index(index)
      { }

    private:
      const Rectangle& rectangle;
      const Interval step;
      unsigned long int index;
    };

    const Rectangle& rectangle;
    const Interval step;
  };
};


constexpr auto _7KAA_COORDINATE_STEP = Interval {
  SCALING_FACTOR,
  SCALING_FACTOR,
};
constexpr auto _7KAA_OFFSET = Interval {
  -SCALING_FACTOR / 2,
  SCALING_FACTOR / 2
};


/** The coordinates of the current viewport. */
Rectangle viewport(
);

} // namespace Ambition::Coordinates
