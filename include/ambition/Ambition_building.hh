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

#include <boost/serialization/vector.hpp>
#include <memory>
#include <vector>

#include "Ambition_coordinates.hh"
#include "Ambition_entity.hh"
#include "Ambition_time.hh"
#include "Ambition_unit.hh"
#include "boost-macros.hh"


namespace Ambition {

class Building : public Entity {
public:
  enum class _7kaaType {
    Firm,
    Town,
  };

  struct TrainingRequest {
    char _7kaaRaceId;
    char _7kaaSkillId;
    unsigned int amount;

    template<class Archive>
    void serialize(
      Archive& archive,
      const unsigned int version
    ) {
      archive & BOOST_SERIALIZATION_NVP(_7kaaRaceId);
      archive & BOOST_SERIALIZATION_NVP(_7kaaSkillId);
      archive & BOOST_SERIALIZATION_NVP(amount);
    }
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

  static std::shared_ptr<Building> getBy7kaaFirmRecordNumber(
    int _7kaaFirmRecordNumber
  );
  static std::shared_ptr<Building> getBy7kaaTownRecordNumber(
    const int _7kaaTownRecordNumber
  );

  void clearRallyPoint(
  );

  void clearTrainingQueue(
  );

  void dequeueTraining(
    const TrainingRequest& request
  );

  void destroy(
    Time::Stamp stamp
  );

  bool destroyed(
  ) const;

  void drawRallyPoint(
  ) const;

  unsigned int enqueuedTrainingCount(
    const short _7kaaSkillId
  ) const;

  void enqueueTraining(
    const TrainingRequest& request
  );

  void popViableTrainingRequest(
  );

  bool rallyPointEnabled(
  ) const;

  void sendUnitsToRallyPoint(
    std::vector<short> _7kaaUnitRecordNumbers
  ) const;

  void setRallyPoint(
    const Unit::Waypoint& waypoint
  );

protected:
  struct Underlying7kaaObject;

  Time::Stamp destroyedAt;
  Unit::Waypoint rally;
  std::vector<TrainingRequest> trainingQueue;

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
    archive & BOOST_SERIALIZATION_NVP(rally);
    archive & BOOST_SERIALIZATION_NVP(trainingQueue);
    archive & BOOST_SERIALIZATION_NVP_CONST(type);
  }
};


void setOrClearRallyPoint(
  const Coordinates::_7kaaCoordinates _7kaaCoordinates,
  const bool allowAction
);

} // namespace Ambition

BOOST_CLASS_VERSION(Ambition::Building, 0)
