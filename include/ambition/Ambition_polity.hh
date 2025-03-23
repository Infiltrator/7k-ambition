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

#include <memory>

#include "Ambition_entity.hh"
#include "Ambition_time.hh"
#include "boost-macros.hh"

class Nation;
class NationBase;


namespace Ambition {

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

  void dissolve(
    Time::Stamp stamp
  );

  int researchTarget(
  ) const;

  void setResearchTarget(
    const int target
  );

protected:
  Time::Stamp dissolvedAt;
  int _researchTarget;

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
  }
};

} // namespace Ambition

BOOST_CLASS_VERSION(Ambition::Polity, 0)
