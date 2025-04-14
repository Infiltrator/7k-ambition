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
 * Implementation file for _7kaaAmbitionInterface::Building.
 */

#define _AMBITION_IMPLEMENTATION
#include "7kaaInterface/building.hh"

#include "OF_RESE.h"
#include "OFIRM.h"
#include "OFONT.h"
#include "OTECHRES.h"
#include "OTOWN.h"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_coordinates.hh"
#include "Ambition_polity.hh"
#include "Ambition_unit.hh"
#include "Ambition_user_interface.hh"
#include "Ambition_vga.hh"
#include "format.hh"


namespace _7kaaAmbitionInterface::Building {

void clearRallyPoint(
  const Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto building = Ambition::Building::findBy7kaaFirm(_7kaaFirm);
  if (building) {
    building->clearRallyPoint();
  }
}
void clearRallyPoint(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);
  if (building) {
    building->clearRallyPoint();
  }
}

void clearTrainingQueue(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);
  if (building) {
    building->clearTrainingQueue();
  }
}

bool dequeueTraining(
  Town* _7kaaTown,
  const char _7kaaSkillId,
  const unsigned int amount
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);
  if (building) {
    building->dequeueTraining(
      {
        ._7kaaRaceId = 0,
        ._7kaaSkillId = _7kaaSkillId,
        .amount = amount,
      }
    );
  }
  return true;
}

void destroy(
  const Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto building = Ambition::Building::findBy7kaaFirm(_7kaaFirm);
  if (building) {
    building->destroy(Ambition::Time::now());
  }
}
void destroy(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);
  if (building) {
    building->destroy(Ambition::Time::now());
  }
}

unsigned int enqueuedTrainingCount(
  const Town* _7kaaTown,
  const int _7kaaCalculation,
  const short _7kaaSkillId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  const auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);

  if (!building) {
    return 0;
  }

  return _7kaaCalculation + building->enqueuedTrainingCount(_7kaaSkillId);
}

bool enqueueTraining(
  Town* _7kaaTown,
  const char _7kaaRaceId,
  const char _7kaaSkillId,
  const unsigned int amount
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  auto building = Ambition::Building::getBy7kaaTown(_7kaaTown);
  building->enqueueTraining(
    {
      ._7kaaRaceId = _7kaaRaceId,
      ._7kaaSkillId = _7kaaSkillId,
      .amount = amount,
    }
  );
  return true;
}

bool printWarMachineIdentifier(
  const Worker* _7kaaWorker,
  const int left,
  const int top,
  const int right
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  std::string text;

  if (_7kaaWorker->unit_id != UNIT_F_BALLISTA
      && (_7kaaWorker->unit_id < UNIT_CATAPULT
          || _7kaaWorker->unit_id > UNIT_EXPLOSIVE_CART)
  ) {
    return false;
  }

  const std::string LETTERS[] = { "C", "B", "S", "N", "P", "U" };
  if (_7kaaWorker->unit_id == UNIT_F_BALLISTA) {
    text = "U";
  } else {
    text = LETTERS[_7kaaWorker->unit_id - UNIT_CATAPULT];
  }
  text += format("%d", _7kaaWorker->extra_para);

  Ambition::printText(
    font_san,
    text,
    {
      .start = {
        .left = left,
        .top = top,
      },
      .end = {
        .left = right,
        .top = top,
      },
    }
  );

  return true;
}

void processIdleTowerOfScience(
  FirmResearch* _7kaaFirmResearch
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (_7kaaFirmResearch->tech_id) {
    return;
  }

  const auto _7kaaNationRecordNumber = _7kaaFirmResearch->nation_recno;
  const auto polity
    = Ambition::Polity::findBy7kaaRecordNumber(_7kaaNationRecordNumber);
  if (polity) {
    const auto researchTarget = polity->researchTarget();
    const auto technologyInfo = tech_res[researchTarget];
    if (researchTarget
      && technologyInfo->can_research(_7kaaNationRecordNumber)
    ) {
      _7kaaFirmResearch->start_research(researchTarget, COMMAND_PLAYER);
    }
  }
}

void processTrainingQueue(
  Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);

  if (!building) {
    return;
  }

  building->popViableTrainingRequest();
}

void sendUnitsToRallyPoint(
  const Firm* _7kaaFirm,
  std::vector<short> _7kaaUnitRecordNumbers
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto building = Ambition::Building::findBy7kaaFirm(_7kaaFirm);
  if (building) {
    building->sendUnitsToRallyPoint(_7kaaUnitRecordNumbers);
  }
}
void sendUnitsToRallyPoint(
  const Town* _7kaaTown,
  std::vector<short> _7kaaUnitRecordNumbers
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);
  if (building) {
    building->sendUnitsToRallyPoint(_7kaaUnitRecordNumbers);
  }
}

} // namespace _7kaaAmbitionInterface::Building
