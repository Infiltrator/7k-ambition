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
 * Header file for _7kaaAmbitionInterface::Building.
 */

#pragma once

#include <vector>

class Firm;
class Town;


namespace _7kaaAmbitionInterface::Building {

void clearRallyPoint(
  const Firm* _7kaaFirm
);
void clearRallyPoint(
  const Town* _7kaaTown
);

void destroy(
  const Firm* _7kaaFirm
);
void destroy(
  const Town* _7kaaTown
);

void sendUnitsToRallyPoint(
  const Firm* _7kaaFirm,
  const std::vector<short> _7kaaUnitRecordNumbers
);
void sendUnitsToRallyPoint(
  const Town* _7kaaTown,
  const std::vector<short> _7kaaUnitRecordNumbers
);

} // namespace _7kaaAmbitionInterface::Building

#ifndef _AMBITION_IMPLEMENTATION
/** Allow 7kaa to call using Ambition::*. */
namespace Ambition = _7kaaAmbitionInterface;
#endif
