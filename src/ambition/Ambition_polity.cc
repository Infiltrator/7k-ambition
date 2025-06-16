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
 * Implementation file for Ambition::Polity.
 */

#include "Ambition_polity.hh"

#include <cassert>

#define _AMBITION_IMPLEMENTATION
#include "ONATION.h"
#include "OSPY.h"
#include "OU_CARA.h"
#include "OUNIT.h"
#include "OWORLD.h"

#include "Ambition_coordinates.hh"
#include "Ambition_repository.hh"
#include "Ambition_trade.hh"


namespace Ambition {

Polity::Polity(
  const unsigned long long recordNumber,
  const short _7kaaRecordNumber
) :
  Entity(recordNumber),
  _7kaaRecordNumber(_7kaaRecordNumber),
  dissolvedAt(Time::START)
{ }

std::shared_ptr<Polity> Polity::create(
  const short _7kaaRecordNumber
) {
  return entityRepository.insert(
    std::make_shared<Polity>(
      entityRepository.takeRecordNumber(),
      _7kaaRecordNumber
    )
  );
}

std::shared_ptr<Polity> Polity::findBy7kaaRecordNumber(
  const short _7kaaRecordNumber
) {
  return entityRepository.findEntityBy<Polity>(
    [&_7kaaRecordNumber](std::shared_ptr<Polity> polity) {
      return polity->active()
        && polity->_7kaaRecordNumber == _7kaaRecordNumber;
    }
  );
}

std::shared_ptr<Polity> Polity::getBy7kaaRecordNumber(
  const short _7kaaRecordNumber
) {
  auto polity = Polity::findBy7kaaRecordNumber(_7kaaRecordNumber);
  if (!polity) {
    polity = create(
      _7kaaRecordNumber
    );
  }

  return polity;
}


bool Polity::active(
) const {
  return dissolvedAt == Time::START;
}

bool Polity::canHireCaravan(
) const {
  if (!active()) {
    return false;
  }

  const auto _7kaaNation = nation_array[_7kaaRecordNumber];

  if (_7kaaNation->cash <= 0) {
    return false;
  }

  if (firm_res[FIRM_MARKET]->nation_firm_count_array[_7kaaRecordNumber - 1] <= 0) {
    return false;
  }

  const auto supportedCount
    = _7kaaNation->total_population / POPULATION_PER_CARAVAN;
  const auto activeCount
    = unit_res[UNIT_CARAVAN]->nation_unit_count_array[_7kaaRecordNumber - 1];

  return (supportedCount > activeCount);

}

void Polity::dissolve(
  Time::Stamp stamp
) {
  dissolvedAt = stamp;
}

void Polity::cloneCaravan(
  const UnitCaravan* _7kaaCaravan
) {
  if (_7kaaCaravan->stop_defined_num == 0) {
    return;
  }

  if (idleCaravans().size() <= _7kaaCaravansToCloneRecordNumbers.size()) {
    const auto _7kaaMarketRecordNumber = findNearest7kaaMarket(
      Coordinates::Point::from7kaaCoordinates(
        {
          .x = _7kaaCaravan->stop_array[0].firm_loc_x1,
          .y = _7kaaCaravan->stop_array[0].firm_loc_y1,
        }
      )
    );
    assert(
      _7kaaMarketRecordNumber
      && !firm_array.is_deleted(_7kaaMarketRecordNumber)
    );
    const auto _7kaaMarket = dynamic_cast<FirmMarket*>(
      firm_array[_7kaaMarketRecordNumber]
    );
    assert(_7kaaMarket);
    _7kaaMarket->hire_caravan(COMMAND_PLAYER);
  }

  _7kaaCaravansToCloneRecordNumbers.push_back(_7kaaCaravan->sprite_recno);
}

short Polity::findNearest7kaaMarket(
  const Coordinates::Point location
) const {
  /* Check whether the location has a Market. */
  const auto _7kaaLocation = world.get_loc(
    location.to7kaaCoordinates().x,
    location.to7kaaCoordinates().y
  );
  const auto _7kaaFirmRecordNumber = _7kaaLocation->firm_recno();
  if (_7kaaFirmRecordNumber && !firm_array.is_deleted(_7kaaFirmRecordNumber)) {
    const auto firm = firm_array[_7kaaFirmRecordNumber];
    if (firm->nation_recno == _7kaaRecordNumber
      && firm->firm_id == FIRM_MARKET
    ) {
      return _7kaaFirmRecordNumber;
    }
  }

  /* Search all Firms for the nearest Market. */
  short nearest7kaaMarketRecordNumber = 0;
  int nearest7kaaMarketDistance = INT_MAX;

  for (auto i = 1; i < firm_array.size(); i++) {
    if (firm_array.is_deleted(i)) {
      continue;
    }

    const auto _7kaaFirm = firm_array[i];
    if (_7kaaFirm->nation_recno != _7kaaRecordNumber
      || _7kaaFirm->firm_id != FIRM_MARKET
    ) {
      continue;
    }

    const auto distance = misc.points_distance(
      _7kaaFirm->loc_x1,
      _7kaaFirm->loc_y1,
      location.to7kaaCoordinates().x,
      location.to7kaaCoordinates().y
    );

    if (distance < nearest7kaaMarketDistance) {
      nearest7kaaMarketDistance = distance;
      nearest7kaaMarketRecordNumber = i;
    }
  }
  return nearest7kaaMarketRecordNumber;
}

std::vector<short> Polity::idleCaravans(
) const {
  std::vector<short> _7kaaCaravanRecordNumbers;

  for (auto i = 1; i < unit_array.size(); i++) {
    if (unit_array.is_deleted(i)) {
      continue;
    }

    const auto _7kaaUnit = unit_array[i];
    if (_7kaaUnit->nation_recno != _7kaaRecordNumber
      || _7kaaUnit->unit_id != UNIT_CARAVAN
    ) {
      continue;
    }

    const auto _7kaaCaravan = dynamic_cast<UnitCaravan*>(_7kaaUnit);
    assert(_7kaaCaravan);
    if (Trade::isCaravanIdle(_7kaaCaravan)) {
      _7kaaCaravanRecordNumbers.push_back(_7kaaCaravan->sprite_recno);
    }
  }
  return _7kaaCaravanRecordNumbers;
}

unsigned long int Polity::nonSpyGeneralCount(
) const {
  auto nonSpyGeneralCount = 0;
  for (auto i = 1; i < unit_array.size(); i++) {
    if (unit_array.is_deleted(i)) {
      continue;
    }

    const auto unit = unit_array[i];
    if (unit->nation_recno == _7kaaRecordNumber
      && (!unit->spy_recno
        || spy_array[unit->spy_recno]->true_nation_recno != _7kaaRecordNumber)
      && unit->rank_id == RANK_GENERAL
    ) {
      nonSpyGeneralCount++;
    }
  }

  return nonSpyGeneralCount;
}

void Polity::processIdleCaravan(
  UnitCaravan* _7kaaCaravan
) {
  if (_7kaaCaravansToCloneRecordNumbers.empty()) {
    return;
  }

  const auto _7kaaCaravanToCopyRecordNumber
    = _7kaaCaravansToCloneRecordNumbers.front();
  assert(!unit_array.is_deleted(_7kaaCaravanToCopyRecordNumber));
  _7kaaCaravan->copy_route(_7kaaCaravanToCopyRecordNumber, COMMAND_PLAYER);
  _7kaaCaravansToCloneRecordNumbers.pop_front();
}

int Polity::researchTarget(
) const {
  return _researchTarget;
}

void Polity::setResearchTarget(
  const int target
) {
  _researchTarget = target;
}

} // namespace Ambition
