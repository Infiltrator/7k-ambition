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
 * Header file for Ambition::Time.
 */

#pragma once

#include <boost/serialization/nvp.hpp>


namespace Ambition::Time {

struct _7kaaTimestamp {
  int gameDate;
};


struct Interval {
public:
  unsigned long long int frameCount;

  static Interval ofDays(
    unsigned long long int days
  );

  inline int compare(
    const Interval& rhs
  ) const {
    return frameCount - rhs.frameCount;
  }

  /** How many full days have elapsed. */
  unsigned long long int days(
  ) const;

  inline bool operator==(const Interval& rhs) const { return compare(rhs) == 0; }
  inline bool operator!=(const Interval& rhs) const { return compare(rhs) != 0; }
  inline bool operator< (const Interval& rhs) const { return compare(rhs) <  0; }
  inline bool operator> (const Interval& rhs) const { return compare(rhs) >  0; }
  inline bool operator<=(const Interval& rhs) const { return compare(rhs) <= 0; }
  inline bool operator>=(const Interval& rhs) const { return compare(rhs) >= 0; }

  template<class Archive>
  void serialize(
    Archive& archive,
    const unsigned int version
  ) {
    archive & BOOST_SERIALIZATION_NVP(frameCount);
  }
};

struct Stamp {
public:
  unsigned long long int frameCount;

  static Stamp from7kaaTimestamp(
    const _7kaaTimestamp& _7kaaTimestamp
  );

  inline int compare(
    const Stamp& rhs
  ) const {
    return frameCount - rhs.frameCount;
  }

  _7kaaTimestamp to7kaaTimestamp(
  ) const;

  inline bool operator==(const Stamp& rhs) const { return compare(rhs) == 0; }
  inline bool operator!=(const Stamp& rhs) const { return compare(rhs) != 0; }
  inline bool operator< (const Stamp& rhs) const { return compare(rhs) <  0; }
  inline bool operator> (const Stamp& rhs) const { return compare(rhs) >  0; }
  inline bool operator<=(const Stamp& rhs) const { return compare(rhs) <= 0; }
  inline bool operator>=(const Stamp& rhs) const { return compare(rhs) >= 0; }

  bool operator==(const _7kaaTimestamp& rhs) const;

  Interval operator-(const Stamp& rhs);
  Stamp operator+(const Interval& rhs);
  Stamp& operator+=(const Interval& rhs);
  Stamp operator-(const Interval& rhs);
  Stamp& operator-=(const Interval& rhs);

  template<class Archive>
  void serialize(
    Archive& archive,
    const unsigned int version
  ) {
    archive & BOOST_SERIALIZATION_NVP(frameCount);
  }
};


constexpr auto START = Stamp({ .frameCount = 0 });


Stamp now(
);

} // namespace Ambition::Time
