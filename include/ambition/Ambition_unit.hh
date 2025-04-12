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
 * Header file for Ambition::Unit.
 */

#pragma once

#include <boost/serialization/deque.hpp>
#include <deque>
#include <memory>
#include <stdint.h>
#include <vector>

#include "Ambition_coordinates.hh"
#include "Ambition_entity.hh"
#include "Ambition_time.hh"

class Firm;
class Town;
class Unit;
struct Worker;


namespace Ambition {

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


class Unit : public Entity {
public:
  struct Waypoint {
    enum class Action {
      MoveOnly,
      InteractWithBuilding,
    };

    Action action;
    Coordinates::Point point;

    template<class Archive>
    void serialize(
      Archive& archive,
      const unsigned int version
    ) {
      archive & BOOST_SERIALIZATION_NVP(action);
      archive & BOOST_SERIALIZATION_NVP(point);
    }
  };

  Unit(
    const unsigned long long recordNumber,
    const short _7kaaSpriteRecordNumber
  );

  static std::shared_ptr<Unit> create(
    const short _7kaaSpriteRecordNumber
  );

  static std::shared_ptr<Unit> findBy7kaaSpriteRecordNumber(
    const short _7kaaSpriteRecordNumber
  );
  static std::shared_ptr<Unit> findBy7kaaSpyRecordNumber(
    const short _7kaaSpyRecordNumber
  );
  static std::shared_ptr<Unit> findBy7kaaWorker(
    const Firm* _7kaaFirm,
    const Worker* _7kaaworker
  );

  static std::shared_ptr<Unit> getBy7kaaSpriteRecordNumber(
    const short _7kaaSpriteRecordNumber
  );


  static uint8_t _7kaaRegionId(
    ::Unit* _7kaaUnit
  );

  static void sendToDestination(
    std::vector<short> _7kaaUnitRecordNumbers,
    const Waypoint& destination
  );

  bool active (
  );

  void addWaypoint(
    const Waypoint& waypoint
  );

  void clearWaypoints(
  );

  void died(
    const Time::Stamp stamp
  );

  void drawWaypointsOnWorld(
  ) const;

  void drawWaypointsOnMinimap(
  ) const;

  void dropSpyIdentity(
  );

  void enteredBuilding(
    const Firm* _7kaaFirm,
    const Worker* _7kaaWorker
  );
  void enteredBuilding(
    const Town* _7kaaTown,
    const Time::Stamp stamp
  );

  void exitedBuilding(
    const ::Unit* _7kaaUnit
  );

  void goToNextWaypoint(
  );

  void migrated(
    const Town* destination
  );

  void retired(
    const Time::Stamp stamp
  );

  void toggleWaypoint(
    const Waypoint& waypoint
  );

protected:
  enum class Status {
    Active,
    InsideBuilding,
    Retired,
    Dead,
  };

  struct WorkerIdentifier {
    short extra_para;
    uint16_t name_id;
    char race_id;
    char rank_id;
    char skill_id;
    char skill_potential;
    short town_recno;
    char unit_id;

    void clear(
    );

    template<class Archive>
    void serialize(
      Archive& archive,
      const unsigned int version
    ) {
      archive & BOOST_SERIALIZATION_NVP(extra_para);
      archive & BOOST_SERIALIZATION_NVP(name_id);
      archive & BOOST_SERIALIZATION_NVP(race_id);
      archive & BOOST_SERIALIZATION_NVP(rank_id);
      archive & BOOST_SERIALIZATION_NVP(skill_id);
      archive & BOOST_SERIALIZATION_NVP(skill_potential);
      archive & BOOST_SERIALIZATION_NVP(town_recno);
      archive & BOOST_SERIALIZATION_NVP(unit_id);
    }
  };

  short _7kaaSpriteRecordNumber;
  short _7kaaSpyRecordNumber;
  Time::Stamp diedAt;
  unsigned long long insideBuildingRecordNumber;
  Time::Stamp lastWaypointOrderIssuedAt;
  Time::Stamp retiredAt;
  Status status;
  std::deque<Waypoint> waypoints;
  WorkerIdentifier workerIdentifier;

  Coordinates::Point currentDestination(
  ) const;

protected:
  friend class boost::serialization::access;

  /** To be used only by Boost serialisation. */
  Unit() :
    Entity()
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
    archive & BOOST_SERIALIZATION_NVP(_7kaaSpriteRecordNumber);
    archive & BOOST_SERIALIZATION_NVP(_7kaaSpyRecordNumber);
    archive & BOOST_SERIALIZATION_NVP(diedAt);
    archive & BOOST_SERIALIZATION_NVP(insideBuildingRecordNumber);
    archive & BOOST_SERIALIZATION_NVP(retiredAt);
    archive & BOOST_SERIALIZATION_NVP(status);
    archive & BOOST_SERIALIZATION_NVP(waypoints);
    archive & BOOST_SERIALIZATION_NVP(workerIdentifier);
    if (version == 1) {
      archive & BOOST_SERIALIZATION_NVP(lastWaypointOrderIssuedAt);
    }
  }
};

} // namespace Ambition

BOOST_CLASS_VERSION(Ambition::Unit, 2)
