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

#define _AMBITION_IMPLEMENTATION
#include "OSPY.h"
#include "OUNIT.h"

#include "Ambition_repository.hh"


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

void Polity::dissolve(
  Time::Stamp stamp
) {
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
