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
