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
 * Header file for _7kaaAmbitionInterface::Unit.
 */

#pragma once

#include <vector>

class Firm;
class Spy;
class Town;
class Unit;
struct Worker;


namespace _7kaaAmbitionInterface::Unit {

void clearWaypoints(
  const std::vector<short> _7kaaUnitRecordNumbers
);

void died(
  const ::Unit* _7kaaUnit
);
void died(
  const Spy* _7kaaSpy
);

void dropSpyIdentity(
  const ::Unit* _7kaaUnit
);
void dropSpyIdentity(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
);

void enteredBuilding(
  const ::Unit* _7kaaUnit,
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
);
void enteredBuilding(
  const ::Unit* _7kaaUnit,
  const Town* _7kaaTown
);

void exitedBuilding(
  const Worker* _7kaaWorker,
  const Firm* _7kaaFirm,
  const ::Unit* _7kaaUnit
);
void exitedBuilding(
  const Spy* _7kaaSpy,
  const Town* _7kaaTown,
  const ::Unit* _7kaaUnit
);

void goToNextWaypoint(
  const ::Unit* _7kaaUnit
);

void migrated(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker,
  const Town* destination
);
void migrated(
  const Spy* _7kaaSpy,
  const Town* destination
);

void retired(
  const ::Unit* _7kaaUnit
);
void retired(
  const Firm* _7kaaFirm,
  const Worker* _7kaaWorker
);

/**
 * Send an available builder to a Firm.
 *
 * A builder is considered to be available if he is idle or repairing a building
 * that is above a certain hitpoint percentage.
 *
 * @param firm The Firm to send a builder to.
 * @return Whether the Ambition code took effect and so the rest of the 7kaa
 * code should be skipped.
 */
bool sendAvailableBuilderToFirm(
  const Firm* firm
);

bool toggleWaypoint(
  const std::vector<short> _7kaaUnitRecordNumbers,
  const short _7kaaX,
  const short _7kaaY,
  const bool allowAction
);

} // namespace _7kaaAmbitionInterface::Unit

#ifndef _AMBITION_IMPLEMENTATION
/** Allow 7kaa to call using Ambition::*. */
namespace Ambition = _7kaaAmbitionInterface;
#endif
