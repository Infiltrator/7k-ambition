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
 * Implementation file for Ambition::Building.
 */

#include "Ambition_building.hh"

#define _AMBITION_IMPLEMENTATION
#include "OANLINE.h"
#include "OFIRM.h"
#include "OIMGRES.h"
#include "OTOWN.h"
#include "OUNIT.h"
#include "OVGABUF.h"

#include "Ambition_repository.hh"
#include "Ambition_unit.hh"
#include "Ambition_user_interface.hh"
#include "format.hh"


namespace Ambition {

struct Building::Underlying7kaaObject {
  Underlying7kaaObject(Firm* firm);
  Underlying7kaaObject(Town* town);

  union Object {
    Firm* firm;
    Town* town;
  } object;
  _7kaaType type;

  inline auto loc_x1() const {
    return type == _7kaaType::Firm ? object.firm->loc_x1 : object.town->loc_x1;
  }
  inline auto loc_x2() const {
    return type == _7kaaType::Firm ? object.firm->loc_x2 : object.town->loc_x2;
  }
  inline auto loc_y1() const {
    return type == _7kaaType::Firm ? object.firm->loc_y1 : object.town->loc_y1;
  }
  inline auto loc_y2() const {
    return type == _7kaaType::Firm ? object.firm->loc_y2 : object.town->loc_y2;
  }
  inline auto setup_date() const {
    return type == _7kaaType::Firm ? object.firm->setup_date : object.town->setup_date;
  }
};

std::shared_ptr<Building> getBuildingBy7kaaFirmRecordNumber(
  int _7kaaFirmRecordNumber
);
std::shared_ptr<Building> getBuildingBy7kaaTownRecordNumber(
  int _7kaaTownRecordNumber
);


std::shared_ptr<Building> Building::create(
  const _7kaaType type,
  const short _7kaaRecordNumber,
  const Time::Stamp erected
) {
  return entityRepository.insert(
    std::make_shared<Building>(
      entityRepository.takeRecordNumber(),
      type,
      _7kaaRecordNumber,
      erected
    )
  );
}

std::shared_ptr<Building> Building::findBy7kaaFirmRecordNumber(
  const short _7kaaFirmRecordNumber
) {
  return entityRepository.findEntityBy<Building>(
    [&_7kaaFirmRecordNumber](std::shared_ptr<Building> building) {
      return !building->destroyed()
        && building->type == _7kaaType::Firm
        && building->_7kaaRecordNumber == _7kaaFirmRecordNumber;
    }
  );
}
std::shared_ptr<Building> Building::findBy7kaaTownRecordNumber(
  const short _7kaaTownRecordNumber
) {
  return entityRepository.findEntityBy<Building>(
    [&_7kaaTownRecordNumber](std::shared_ptr<Building> building) {
      return !building->destroyed()
        && building->type == _7kaaType::Town
        && building->_7kaaRecordNumber == _7kaaTownRecordNumber;
    }
  );
}


void Building::clearRallyPoint(
) {
  rallyPoint = underlying7kaaObjectRectangle().centre();
}

void Building::destroy(
  Time::Stamp stamp
) {
  if (stamp == Time::START) {
    throw std::invalid_argument(
      format(
        "[%d] Cannot set destruction timestamp to beginning",
        recordNumber
      )
    );
  }

  destroyedAt = stamp;
}

bool Building::destroyed(
) const {
  return destroyedAt > Time::START;
}

void Building::drawRallyPoint(
) const {
  const auto locationRectangle = underlying7kaaObjectRectangle();

  if (rallyPoint.within(locationRectangle)) {
    return;
  }

  auto *chPtr = image_icon.get_ptr("WAYPOINT");
  const auto chOffsetX = -(*(short*)chPtr / 2);
  const auto chOffsetY = -(*(1+(short*)chPtr) / 2);

  const auto buildingScreenCoordinates = UserInterface::fromWorldPoint(
    locationRectangle.centre()
  );
  const auto rallyPointScreenCoordinates = UserInterface::fromWorldPoint(
    rallyPoint
  );

  anim_line.draw_line(
    &vga_back,
    buildingScreenCoordinates.left,
    buildingScreenCoordinates.top,
    rallyPointScreenCoordinates.left,
    rallyPointScreenCoordinates.top,
    0,
    1
  );
  world.zoom_matrix->put_bitmap_clip(
    rallyPointScreenCoordinates.left + chOffsetX,
    rallyPointScreenCoordinates.top + chOffsetY,
    chPtr
  );

  const auto animLineBoundX1 = anim_line.bound_x1;
  const auto animLineBoundY1 = anim_line.bound_y1;
  const auto animLineBoundX2 = anim_line.bound_x2;
  const auto animLineBoundY2 = anim_line.bound_y2;
  anim_line.bound_x1 = MAP_X1;
  anim_line.bound_y1 = MAP_Y1;
  anim_line.bound_x2 = MAP_X2;
  anim_line.bound_y2 = MAP_Y2;

  const auto _7kaaBuildingLocation = locationRectangle.centre().to7kaaCoordinates();

  anim_line.draw_line(
    &vga_back,
    MAP_X1 + _7kaaBuildingLocation.x,
    MAP_Y1 + _7kaaBuildingLocation.y,
    MAP_X1 + rallyPoint.to7kaaCoordinates().x,
    MAP_Y1 + rallyPoint.to7kaaCoordinates().y,
    0,
    2
  );

  anim_line.bound_x1 = animLineBoundX1;
  anim_line.bound_y1 = animLineBoundY1;
  anim_line.bound_x2 = animLineBoundX2;
  anim_line.bound_y2 = animLineBoundY2;
}

void Building::sendUnitsToRallyPoint(
  std::vector<short> _7kaaUnitRecordNumbers
) const {
  if (rallyPoint.within(underlying7kaaObjectRectangle())) {
    return;
  }

  unit_array.move_to(
    rallyPoint.to7kaaCoordinates().x,
    rallyPoint.to7kaaCoordinates().y,
    0,
    &_7kaaUnitRecordNumbers[0],
    _7kaaUnitRecordNumbers.size(),
    0
  );
}

void Building::setRallyPoint(
  const Coordinates::Point coordinates
) {
  rallyPoint = coordinates;
}

Building::Building(
  const unsigned long long recordNumber,
  const _7kaaType type,
  const short _7kaaRecordNumber,
  const Time::Stamp erected
) :
  Entity(recordNumber),
  _7kaaRecordNumber(_7kaaRecordNumber),
  destroyedAt(Time::START),
  erected(erected),
  type(type)
{
  rallyPoint = underlying7kaaObjectRectangle().centre();
}


void setOrClearRallyPoint(
  const Coordinates::_7kaaCoordinates _7kaaCoordinates
) {
  if (firm_array.selected_recno) {
    const auto _7kaaFirmRecordNumber = firm_array.selected_recno;
    auto building = getBuildingBy7kaaFirmRecordNumber(_7kaaFirmRecordNumber);

    const auto _7kaaFirm = firm_array[_7kaaFirmRecordNumber];
    if (_7kaaCoordinates.x >= _7kaaFirm->loc_x1
      && _7kaaCoordinates.x <= _7kaaFirm->loc_x2
      && _7kaaCoordinates.y >= _7kaaFirm->loc_y1
      && _7kaaCoordinates.y <= _7kaaFirm->loc_y2
    ) {
      building->clearRallyPoint();
    } else {
      building->setRallyPoint(
        Coordinates::Point::from7kaaCoordinates(_7kaaCoordinates)
      );
    }
  }

  if (town_array.selected_recno) {
    const auto _7kaaTownRecordNumber = town_array.selected_recno;
    auto building = getBuildingBy7kaaTownRecordNumber(_7kaaTownRecordNumber);

    const auto _7kaaTown = town_array[_7kaaTownRecordNumber];
    if (_7kaaCoordinates.x >= _7kaaTown->loc_x1
      && _7kaaCoordinates.x <= _7kaaTown->loc_x2
      && _7kaaCoordinates.y >= _7kaaTown->loc_y1
      && _7kaaCoordinates.y <= _7kaaTown->loc_y2
    ) {
      building->clearRallyPoint();
    } else {
      building->setRallyPoint(
        Coordinates::Point::from7kaaCoordinates(_7kaaCoordinates)
      );
    }
  }
}


/* Protected functions. */

Building::Underlying7kaaObject Building::underlying7kaaObject(
) const {
  if (destroyed()) {
    throw std::domain_error(
      format(
        "[%llu] 7kaa Firm/Town %d was destroyed.",
        recordNumber,
        _7kaaRecordNumber
      )
    );
  }

  if (type == _7kaaType::Firm) {
    if (firm_array.is_deleted(_7kaaRecordNumber)) {
      throw std::domain_error(
        format(
          "[%llu] 7kaa Firm record number %d is deleted.",
          recordNumber,
          _7kaaRecordNumber
        )
      );
    }

    return Underlying7kaaObject(firm_array[_7kaaRecordNumber]);
  }

  if (type == _7kaaType::Town) {
    if (town_array.is_deleted(_7kaaRecordNumber)) {
      throw std::domain_error(
        format(
          "[%llu] 7kaa Town record number %d is deleted.",
          recordNumber,
          _7kaaRecordNumber
        )
      );
    }

    return Underlying7kaaObject(town_array[_7kaaRecordNumber]);
  }

  throw std::logic_error(
    format("[%llu] Must be either a 7kaa Firm or Town.", recordNumber)
  );
}

const Coordinates::Rectangle Building::underlying7kaaObjectRectangle(
) const {
  const auto object = underlying7kaaObject();

  return {
    .start = Coordinates::Point::from7kaaCoordinates({
        .x = object.loc_x1(), .y = object.loc_y1() }
    ),
    .end = Coordinates::Point::from7kaaCoordinates(
      { .x = object.loc_x2(), .y = object.loc_y2() }
    ),
  };
}


/* Private functions. */

Building::Underlying7kaaObject::Underlying7kaaObject(
  Firm *firm
) :
  type(_7kaaType::Firm)
{
  object.firm = firm;
}
Building::Underlying7kaaObject::Underlying7kaaObject(
  Town *town
) :
  type(_7kaaType::Town)
{
  object.town = town;
}

std::shared_ptr<Building> getBuildingBy7kaaFirmRecordNumber(
  int _7kaaFirmRecordNumber
) {
  auto building = Building::findBy7kaaFirmRecordNumber(_7kaaFirmRecordNumber);
  if (!building) {
    building = Building::create(
      Building::_7kaaType::Firm,
      _7kaaFirmRecordNumber,
      Time::Stamp::from7kaaTimestamp(
        { .gameDate = firm_array[_7kaaFirmRecordNumber]->setup_date }
      )
    );
  }

  return building;
}
std::shared_ptr<Building> getBuildingBy7kaaTownRecordNumber(
  int _7kaaTownRecordNumber
) {
  auto building = Building::findBy7kaaTownRecordNumber(_7kaaTownRecordNumber);
  if (!building) {
    building = Building::create(
      Building::_7kaaType::Town,
      _7kaaTownRecordNumber,
      Time::Stamp::from7kaaTimestamp(
        { .gameDate = town_array[_7kaaTownRecordNumber]->setup_date }
      )
    );
  }

  return building;
}

} // namespace Ambition
