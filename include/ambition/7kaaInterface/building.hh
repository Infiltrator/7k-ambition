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
class FirmResearch;
class Town;
struct Worker;


namespace _7kaaAmbitionInterface::Building {

void clearRallyPoint(
  const Firm* _7kaaFirm
);
void clearRallyPoint(
  const Town* _7kaaTown
);

void clearTrainingQueue(
  const Town* _7kaaTown
);

bool dequeueProduction(
  Firm* _7kaaFirm,
  const int _7kaaUnitId,
  const unsigned int amount
);

bool dequeueTraining(
  Town* _7kaaTown,
  const char _7kaaSkillId,
  const unsigned int amount
);

void destroy(
  const Firm* _7kaaFirm
);
void destroy(
  const Town* _7kaaTown
);

unsigned int enqueuedProductionCount(
  Firm* _7kaaFirm,
  const int _7kaaCalculation,
  const int _7kaaUnitId
);

unsigned int enqueuedTrainingCount(
  const Town* _7kaaTown,
  const int _7kaaCalculation,
  const short _7kaaSkillId
);

bool enqueueProduction(
  const Firm* _7kaaFirm,
  const int _7kaaUnitId,
  const unsigned int amount
);

bool enqueueTraining(
  Town* _7kaaTown,
  const char _7kaaRaceId,
  const char _7kaaSkillId,
  const unsigned int amount
);

bool printWarMachineIdentifier(
  const Worker* _7kaaWorker,
  const int left,
  const int top,
  const int right
);

void processIdleTowerOfScience(
  FirmResearch* _7kaaFirmResearch
);

void processProductionQueue(
  Firm* _7kaaFirm
);

void processTrainingQueue(
  Town* _7kaaTown
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
