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
 * Header file for Ambition::Building.
 */

#pragma once

#include <memory>
#include <vector>

#include "Ambition_coordinates.hh"
#include "Ambition_entity.hh"
#include "Ambition_time.hh"
#include "boost-macros.hh"

class Firm;
class Town;


namespace Ambition {

class Building : public Entity {
public:
  enum class _7kaaType {
    Firm,
    Town,
  };

  Building(
    const unsigned long long recordNumber,
    const _7kaaType type,
    const short _7kaaRecordNumber,
    const Time::Stamp erected
  );

  const short _7kaaRecordNumber;
  const Time::Stamp erected;
  const _7kaaType type;

  static std::shared_ptr<Building> create(
    const _7kaaType type,
    const short _7kaaRecordNumber,
    const Time::Stamp erected
  );

  static std::shared_ptr<Building> findBy7kaaFirmRecordNumber(
    const short _7kaaFirmRecordNumber
  );
  static std::shared_ptr<Building> findBy7kaaTownRecordNumber(
    const short _7kaaTownRecordNumber
  );

  void clearRallyPoint(
  );

  void destroy(
    Time::Stamp stamp
  );

  bool destroyed(
  ) const;

  void drawRallyPoint(
  ) const;

  void sendUnitsToRallyPoint(
    std::vector<short> _7kaaUnitRecordNumbers
  ) const;

  void setRallyPoint(
    const Coordinates::Point coordinates
  );

protected:
  struct Underlying7kaaObject;

  Time::Stamp destroyedAt;
  Coordinates::Point rallyPoint;

  Underlying7kaaObject underlying7kaaObject(
  ) const;

  const Coordinates::Rectangle underlying7kaaObjectRectangle(
  ) const;

protected:
  friend class boost::serialization::access;

  /** To be used only by Boost serialisation. */
  Building() :
    Entity(),
    _7kaaRecordNumber(),
    erected(),
    type()
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
    archive & BOOST_SERIALIZATION_NVP(destroyedAt);
    archive & BOOST_SERIALIZATION_NVP_CONST(erected);
    archive & BOOST_SERIALIZATION_NVP(rallyPoint);
    archive & BOOST_SERIALIZATION_NVP_CONST(type);
  }
};


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

void setOrClearRallyPoint(
  const Coordinates::_7kaaCoordinates _7kaaCoordinates
);

} // namespace Ambition

BOOST_CLASS_VERSION(Ambition::Building, 0)
