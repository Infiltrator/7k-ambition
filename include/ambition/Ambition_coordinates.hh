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

  Point centre() const;
  Point topLeft() const;
};

/** The coordinates of the current viewport. */
Rectangle viewport(
);

} // namespace Ambition::Coordinates
