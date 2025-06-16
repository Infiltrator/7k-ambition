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
 * Header file for Ambition::Polity.
 */

#pragma once

#include <boost/serialization/deque.hpp>
#include <boost/serialization/set.hpp>
#include <deque>
#include <memory>
#include <set>
#include <vector>

#include "Ambition_entity.hh"
#include "Ambition_time.hh"
#include "boost-macros.hh"

class Nation;
class NationBase;
class UnitCaravan;


namespace Ambition {

namespace Coordinates {
struct Point;
}


class Polity : public Entity {
public:
  const short _7kaaRecordNumber;

  Polity(
    const unsigned long long recordNumber,
    const short _7kaaRecordNumber
  );

  static std::shared_ptr<Polity> create(
    const short _7kaaRecordNumber
  );

  static std::shared_ptr<Polity> findBy7kaaRecordNumber(
    const short _7kaaRecordNumber
  );

  static std::shared_ptr<Polity> getBy7kaaRecordNumber(
    const short _7kaaRecordNumber
  );

  bool active(
  ) const;

  bool canHireCaravan(
  ) const;

  void dissolve(
    Time::Stamp stamp
  );

  void cloneCaravan(
    const UnitCaravan* _7kaaCaravan
  );

  short findNearest7kaaMarket(
    const Coordinates::Point location
  ) const;

  std::vector<short> idleCaravans(
  ) const;

  unsigned long int nonSpyGeneralCount(
  ) const;

  void processIdleCaravan(
    UnitCaravan* _7kaaCaravan
  );

  void replaceCaravan(
    UnitCaravan* _7kaaCaravan
  );

  int researchTarget(
  ) const;

  void setResearchTarget(
    const int target
  );

protected:
  std::deque<short> _7kaaCaravansToCloneRecordNumbers;
  std::set<short> _7kaaCaravansToRetireRecordNumbers;
  Time::Stamp dissolvedAt;
  int _researchTarget {0};

protected:
  friend class boost::serialization::access;

  /** To be used only by Boost serialisation. */
  Polity() :
    Entity(),
    _7kaaRecordNumber()
  { }

  template<class Archive>
  void serialize(
    Archive& archive,
    const unsigned int version
  ) {
    archive & boost::serialization::make_nvp(
      "entity",
      boost::serialization::base_object<Entity>(*this)
    );
    archive & BOOST_SERIALIZATION_NVP_CONST(_7kaaRecordNumber);
    archive & BOOST_SERIALIZATION_NVP(dissolvedAt);
    archive & BOOST_SERIALIZATION_NVP(_researchTarget);
    if (version >= 1) {
      archive & BOOST_SERIALIZATION_NVP(_7kaaCaravansToCloneRecordNumbers);
      archive & BOOST_SERIALIZATION_NVP(_7kaaCaravansToRetireRecordNumbers);
    }
  }
};

} // namespace Ambition

BOOST_CLASS_VERSION(Ambition::Polity, 1)
