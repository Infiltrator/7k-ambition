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
 * Implementation file for Ambition::Time.
 */

#include "Ambition_time.hh"

#define _AMBITION_IMPLEMENTATION
#include "OINFO.h"
#include "ODATE.h"


namespace Ambition::Time {

constexpr auto FRAMES_PER_DAY = 10;


Interval Interval::ofDays(
  const unsigned long long int days
) {
  return { .frameCount = days * FRAMES_PER_DAY };
}

unsigned long long int Interval::days(
) const {
  return frameCount / FRAMES_PER_DAY;
}


Stamp Stamp::from7kaaTimestamp(
  const _7kaaTimestamp& _7kaaTimestamp
) {
  return {
    .frameCount = static_cast<unsigned>(
      _7kaaTimestamp.gameDate - date.julian(1000, 6, 1)
    ) * FRAMES_PER_DAY
  };
}

Interval Stamp::operator-(
  const Stamp &rhs
) {
  return { .frameCount = frameCount - rhs.frameCount };
}

Stamp now(
) {
  return Time::Stamp::from7kaaTimestamp({ .gameDate = info.game_date });
}

} // namespace Ambition::Time
