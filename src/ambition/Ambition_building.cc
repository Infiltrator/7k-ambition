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
#include "OF_HARB.h"
#include "OF_WAR.h"
#include "OFIRM.h"
#include "OFONT.h"
#include "OIMGRES.h"
#include "OREMOTE.h"
#include "OTOWN.h"
#include "OUNIT.h"
#include "vga_util.h"

#include "Ambition_polity.hh"
#include "Ambition_repository.hh"
#include "Ambition_unit.hh"
#include "Ambition_user_interface.hh"
#include "Ambition_vga.hh"
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

std::shared_ptr<Building> Building::findBy7kaaFirm(
  const Firm* _7kaaFirm
) {
  return findBy7kaaFirmRecordNumber(_7kaaFirm->firm_recno);
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
std::shared_ptr<Building> Building::findBy7kaaTown(
  const Town* _7kaaTown
) {
  return findBy7kaaTownRecordNumber(_7kaaTown->town_recno);
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


bool Building::canProduce(
  const char _7kaaRaceId,
  const int _7kaaSkillId
) const {
  const auto _7kaaObject = underlying7kaaObject();

  if (type == _7kaaType::Town) {
    return _7kaaObject.object.town->can_train(_7kaaRaceId);
  }

  if (dynamic_cast<FirmWar*>(_7kaaObject.object.firm)) {
    return true;
  }

  if (const auto _7kaaHarbour
      = dynamic_cast<FirmHarbor*>(_7kaaObject.object.firm)
  ) {
    return _7kaaHarbour->ship_count < MAX_SHIP_IN_HARBOR;
  }

  return false;
}

void Building::clearRallyPoint(
) {
  rally.point = underlying7kaaObjectRectangle().centre();
}

void Building::clearTrainingQueue(
) {
  productionQueue.clear();
}

bool Building::currentlyProducing(
) const {
  const auto _7kaaObject = underlying7kaaObject();

  if (type == _7kaaType::Town) {
    return _7kaaObject.object.town->train_unit_recno;
  }

  if (const auto _7kaaWarFactory
      = dynamic_cast<FirmWar*>(_7kaaObject.object.firm)
  ) {
    return _7kaaWarFactory->build_unit_id;
  }

  if (const auto _7kaaHarbour
      = dynamic_cast<FirmHarbor*>(_7kaaObject.object.firm)
  ) {
    return _7kaaHarbour->build_unit_id;
  }

  return false;
}

void Building::dequeueTraining(
  const ProductionRequest& request
) {
  migrate7kaaQueue();

  int removedCount = 0;
  for (auto iterator = productionQueue.rbegin();
    removedCount < request.amount && iterator != productionQueue.rend();
  ) {
    if (iterator->_7kaaSkillId == request._7kaaSkillId) {
      const auto amountToRemove = request.amount - removedCount;
      if (iterator->amount <= amountToRemove) {
        removedCount += iterator->amount;
        iterator = static_cast<decltype(iterator)>(
          productionQueue.erase(std::next(iterator).base())
        );
      } else {
        removedCount += amountToRemove;
        iterator->amount -= amountToRemove;
        iterator++;
      }
    } else {
      iterator++;
    }
  }

  if (request.amount > removedCount) {
    const auto _7kaaObject = underlying7kaaObject();

    if (type == _7kaaType::Town) {
      if (_7kaaObject.object.town->train_unit_recno) {
        const auto _7kaaUnit = unit_array[_7kaaObject.object.town->train_unit_recno];
        if (_7kaaUnit->skill.skill_id == request._7kaaSkillId) {
          if (remote.is_enable()) {
            auto message = (short*)remote.new_send_queue_msg(
              MSG_TOWN_SKIP_RECRUIT,
              1 * sizeof(short)
            );
            message[0] = _7kaaRecordNumber;
          } else {
            _7kaaObject.object.town->cancel_train_unit();
          }
        }
      }
      return;
    }

    if (const auto _7kaaWarFactory
        = dynamic_cast<FirmWar*>(_7kaaObject.object.firm)
    ) {
      if (_7kaaWarFactory->build_unit_id == request._7kaaSkillId) {
        if (remote.is_enable()) {
          auto message = (short*)remote.new_send_queue_msg(
            MSG_F_WAR_SKIP_WEAPON,
            1 * sizeof(short)
          );
          message[0] = _7kaaRecordNumber;
        } else {
          _7kaaWarFactory->cancel_build_unit();
        }
      }
    }

    if (const auto _7kaaHarbour
        = dynamic_cast<FirmHarbor*>(_7kaaObject.object.firm)) {
      if (_7kaaHarbour->build_unit_id == request._7kaaSkillId) {
        if (remote.is_enable()) {
          auto message = (short*)remote.new_send_queue_msg(
            MSG_F_HARBOR_SKIP_SHIP,
            1 * sizeof(short)
          );
          message[0] = _7kaaRecordNumber;
        } else {
          _7kaaHarbour->cancel_build_unit();
        }
      }
    }
  }
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

unsigned int Building::enqueuedProductionCount(
  const short _7kaaSkillId
) const {
  int count = 0;

  for (const auto& request : productionQueue) {
    if (request._7kaaSkillId == _7kaaSkillId || _7kaaSkillId == -1) {
      count += request.amount;
    }
  }

  return count;
}

void Building::drawProductionQueueTotals(
  const int refreshFlag,
  const int doneButtonRight,
  const int top,
  const int bottom
) const {
  auto _7kaaObject = underlying7kaaObject();
  const auto _7kaaWarFactory = dynamic_cast<const FirmWar*>(_7kaaObject.object.firm);
  const auto _7kaaHarbour = dynamic_cast<const FirmHarbor*>(_7kaaObject.object.firm);

  const auto buildQueueCount
    = _7kaaWarFactory
    ? _7kaaWarFactory->build_queue_count
    : _7kaaHarbour->build_queue_count;
  const auto buildQueueArray
    = _7kaaWarFactory
    ? _7kaaWarFactory->build_queue_array
    : _7kaaHarbour->build_queue_array;
  const auto buildUnitId
    = _7kaaWarFactory
    ? _7kaaWarFactory->build_unit_id
    : _7kaaHarbour->build_unit_id;

  auto totalCost = 0;
  auto totalQueuedCount = 0;
  for (auto unitId = 1; unitId <= MAX_UNIT_TYPE; unitId++) {
    const auto unitInfo = unit_res[unitId];

    auto queuedCount = 0;
    for (auto i = 0 ; i < buildQueueCount; i++) {
      if (buildQueueArray[i] == unitId) {
        queuedCount++;
      }
    }
    if (buildUnitId == unitId) {
      queuedCount++;
    }
    queuedCount += enqueuedProductionCount(unitId);

    totalQueuedCount += queuedCount;
    totalCost += unitInfo->build_cost * queuedCount;
  }

  const auto costString = format("$%'d", totalCost);
  const auto textWidth = font_mid.text_width(costString.c_str());

  constexpr auto BOTTOM_PADDING = 8;
  constexpr auto COUNT_PANEL_WIDTH = 30;
  constexpr auto PANEL_WIDTH = 108;
  constexpr auto BUTTON_PANEL_GAP = 12;

  const auto panel = Ambition::UserInterface::Rectangle::fromPoint(
    {
      .left = doneButtonRight,
      .top = top,
    },
    {
      .width = PANEL_WIDTH,
      .height = bottom - top + 1,
    }
  ).inner(BUTTON_PANEL_GAP, 2, 0);

  const auto countPanel = panel.internal(
    {
      .width = COUNT_PANEL_WIDTH,
      .height = panel.height(),
    },
    Ambition::UserInterface::HorizontalAlignment::Right
  );

  if (refreshFlag == INFO_REPAINT) {
    Ambition::UserInterface::drawPanel(panel);
    Ambition::UserInterface::drawPanel(countPanel);
  }

  Ambition::UserInterface::printText(
    font_bible,
    costString,
    panel.inner(0, 0, countPanel.width() + 4, BOTTOM_PADDING - 4),
    Ambition::UserInterface::Clear::EntireArea,
    Ambition::UserInterface::HorizontalAlignment::Right,
    Ambition::UserInterface::VerticalAlignment::Bottom
  );
  Ambition::UserInterface::printText(
    font_mid,
    format("%d", totalQueuedCount),
    countPanel.inner(0, 0, 0, BOTTOM_PADDING),
    Ambition::UserInterface::Clear::EntireArea,
    Ambition::UserInterface::HorizontalAlignment::Centre,
    Ambition::UserInterface::VerticalAlignment::Bottom
  );
}

void Building::drawRallyButton(
) const {
  if (!rallyPointEnabled()) {
    vga_util.blt_buf(
      UserInterface::RALLY_POINT_BUTTON.start.left,
      UserInterface::RALLY_POINT_BUTTON.start.top,
      UserInterface::RALLY_POINT_BUTTON.end.left,
      UserInterface::RALLY_POINT_BUTTON.end.top,
      0
    );
    return;
  }

  const auto waypointIcon = image_icon.get_ptr("WAYPOINT");
  vga_front.put_bitmap_trans(
    UserInterface::RALLY_POINT_BUTTON.start.left,
    UserInterface::RALLY_POINT_BUTTON.start.top,
    waypointIcon
  );
}

void Building::drawRallyPoint(
) const {
  if (!rallyPointEnabled()) {
    return;
  }

  const auto locationRectangle = underlying7kaaObjectRectangle();

  auto *chPtr = image_icon.get_ptr("WAYPOINT");
  const auto chOffsetX = -(*(short*)chPtr / 2);
  const auto chOffsetY = -(*(1+(short*)chPtr) / 2);

  const auto buildingScreenCoordinates = UserInterface::fromWorldPoint(
    locationRectangle.centre()
  );
  const auto rallyPointScreenCoordinates = UserInterface::fromWorldPoint(
    rally.point
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

  if (rally.action == Unit::Waypoint::Action::MoveOnly) {
    world.zoom_matrix->put_bitmap_clip(
      rallyPointScreenCoordinates.left + 2,
      rallyPointScreenCoordinates.top,
      image_icon.get_ptr("NOPICK")
    );
  }

  drawMinimapLine(locationRectangle.centre(), rally.point, 2);
}

void Building::enqueueProduction(
  const ProductionRequest& request,
  const unsigned long long int order
) {
  auto trainingCount = 0;

  auto iterator = productionQueue.begin();
  for (;iterator != productionQueue.end(); iterator++) {
    trainingCount += iterator->amount;

    if (order < trainingCount) {
      break;
    }
  }

  if (iterator != productionQueue.end()
    && iterator->_7kaaRaceId == request._7kaaRaceId
    && iterator->_7kaaSkillId == request._7kaaSkillId
  ) {
    iterator->amount += request.amount;
  } else {
    productionQueue.insert(iterator, request);
  }
}

Coordinates::Point Building::getRallyLocation(
) const {
  return rally.point;
}

void Building::migrate7kaaQueue(
) {
  auto _7kaaObject = underlying7kaaObject();

  if (type == _7kaaType::Town) {
    if (!_7kaaObject.object.town->train_queue_count) {
      return;
    }

    for (auto i = 0; i < _7kaaObject.object.town->train_queue_count; i++) {
      enqueueProduction(
        {
          ._7kaaRaceId = _7kaaObject.object.town->train_queue_race_array[i],
          ._7kaaSkillId = _7kaaObject.object.town->train_queue_skill_array[i],
          .amount = 1,
        },
        -1
      );
    }
  } else if (const auto _7kaaWarFactory
    = dynamic_cast<FirmWar*>(_7kaaObject.object.firm)
  ) {
    if (!_7kaaWarFactory->build_queue_count) {
      return;
    }

    for (auto i = 0; i < _7kaaWarFactory->build_queue_count; i++) {
      enqueueProduction(
        {
          ._7kaaRaceId = -1,
          ._7kaaSkillId = _7kaaWarFactory->build_queue_array[i],
          .amount = 1,
        },
        -1
      );
    }
  } else if (const auto _7kaaHarbour
    = dynamic_cast<FirmHarbor*>(_7kaaObject.object.firm)
  ) {
    if (!_7kaaHarbour->build_queue_count) {
      return;
    }

    for (auto i = 0; i < _7kaaHarbour->build_queue_count; i++) {
      enqueueProduction(
        {
          ._7kaaRaceId = -1,
          ._7kaaSkillId = _7kaaHarbour->build_queue_array[i],
          .amount = 1,
        },
        -1
      );
    }
  }

  for (auto iterator = productionQueue.begin();
    iterator != productionQueue.end();
    iterator++
  ) {
    if (type == _7kaaType::Town) {
      if (remote.is_enable()) {
        auto message = (short*)remote.new_send_queue_msg(
          MSG_TOWN_RECRUIT,
          4 * sizeof(short)
        );
        message[0] = _7kaaRecordNumber;
        message[1] = iterator->_7kaaSkillId;
        message[2] = -1;
        message[3] = (short)iterator->amount;
      } else {
        _7kaaObject.object.town->remove_queue(
          iterator->_7kaaSkillId,
          iterator->amount
        );
      }
    } else if (const auto _7kaaWarFactory
      = dynamic_cast<FirmWar*>(_7kaaObject.object.firm)
    ) {
      if (remote.is_enable()) {
        auto message = (short*)remote.new_send_queue_msg(
          MSG_F_WAR_CANCEL_WEAPON,
          3 * sizeof(short)
        );
        message[0] = _7kaaRecordNumber;
        message[1] = iterator->_7kaaSkillId;
        message[2] = iterator->amount;
      } else {
        _7kaaWarFactory->remove_queue(iterator->_7kaaSkillId, iterator->amount);
      }
    } else if (const auto _7kaaHarbour
      = dynamic_cast<FirmHarbor*>(_7kaaObject.object.firm)
    ) {
      if (remote.is_enable()) {
        auto message = (short*)remote.new_send_queue_msg(
          MSG_F_HARBOR_BUILD_SHIP,
          3 * sizeof(short)
        );
        message[0] = _7kaaRecordNumber;
        message[1] = -iterator->_7kaaSkillId;
        message[2] = iterator->amount;
      } else {
        _7kaaHarbour->remove_queue(iterator->_7kaaSkillId, iterator->amount);
      }
    }
  }
}

void Building::popViableProductionRequest(
) {
  if (destroyed()) {
    return;
  }

  if (currentlyProducing()) {
    return;
  }

  for (auto iterator = productionQueue.begin();
    iterator != productionQueue.end();
    iterator++
  ) {
    if (canProduce(iterator->_7kaaRaceId, iterator->_7kaaSkillId)) {
      produce(iterator->_7kaaRaceId, iterator->_7kaaSkillId);

      iterator->amount--;
      if (iterator->amount == 0) {
        productionQueue.erase(iterator);
      }
      break;
    }
  }
}

void Building::produce(
  const char _7kaaRaceId,
  const int _7kaaSkillId
) {
  const auto _7kaaObject = underlying7kaaObject();

  if (type == _7kaaType::Town) {
    _7kaaObject.object.town->recruit(_7kaaSkillId, _7kaaRaceId, COMMAND_PLAYER);
    return;
  }

  if (const auto _7kaaWarFactory
      = dynamic_cast<FirmWar*>(_7kaaObject.object.firm)
  ) {
    if (remote.is_enable()) {
      auto message = (short*)remote.new_send_queue_msg(
        MSG_F_WAR_BUILD_WEAPON,
        3 * sizeof(short)
      );
      message[0] = _7kaaRecordNumber;
      message[1] = _7kaaSkillId;
      message[2] = 1;
    } else {
      _7kaaWarFactory->add_queue(_7kaaSkillId, 1);
    }
  }

  if (const auto _7kaaHarbour
      = dynamic_cast<FirmHarbor*>(_7kaaObject.object.firm)) {
    if (remote.is_enable()) {
      auto message = (short*)remote.new_send_queue_msg(
        MSG_F_HARBOR_BUILD_SHIP,
        3 * sizeof(short)
      );
      message[0] = _7kaaRecordNumber;
      message[1] = _7kaaSkillId;
      message[2] = 1;
    } else {
      _7kaaHarbour->add_queue(_7kaaSkillId, 1);
    }
  }
}

bool Building::rallyPointEnabled(
) const {
  return !rally.point.within(underlying7kaaObjectRectangle());
}

void Building::sendUnitsToRallyPoint(
  std::vector<short> _7kaaUnitRecordNumbers
) const {
  if (!rallyPointEnabled()) {
    return;
  }

  Unit::sendToDestination(_7kaaUnitRecordNumbers, rally);
}

void Building::toggleRallyPoint(
  const Unit::Waypoint& waypoint
) {
  if (waypoint.point == rally.point) {
    clearRallyPoint();
    return;
  }

  rally = waypoint;
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
  rally.point = underlying7kaaObjectRectangle().centre();
}


void setOrClearRallyPoint(
  const Coordinates::_7kaaCoordinates _7kaaCoordinates,
  const bool allowAction
) {
  if (firm_array.selected_recno) {
    const auto _7kaaFirmRecordNumber = firm_array.selected_recno;
    auto building = Building::getBy7kaaFirmRecordNumber(_7kaaFirmRecordNumber);

    const auto _7kaaFirm = firm_array[_7kaaFirmRecordNumber];
    if (_7kaaCoordinates.x >= _7kaaFirm->loc_x1
      && _7kaaCoordinates.x <= _7kaaFirm->loc_x2
      && _7kaaCoordinates.y >= _7kaaFirm->loc_y1
      && _7kaaCoordinates.y <= _7kaaFirm->loc_y2
    ) {
      building->clearRallyPoint();
    } else {
      building->toggleRallyPoint(
        {
          .action = allowAction
            ? Ambition::Unit::Waypoint::Action::InteractWithBuilding
            : Ambition::Unit::Waypoint::Action::MoveOnly,
          .point = Coordinates::Point::from7kaaCoordinates(_7kaaCoordinates),
        }
      );
    }
  }

  if (town_array.selected_recno) {
    const auto _7kaaTownRecordNumber = town_array.selected_recno;
    auto building = Building::getBy7kaaTownRecordNumber(_7kaaTownRecordNumber);

    const auto _7kaaTown = town_array[_7kaaTownRecordNumber];
    if (_7kaaCoordinates.x >= _7kaaTown->loc_x1
      && _7kaaCoordinates.x <= _7kaaTown->loc_x2
      && _7kaaCoordinates.y >= _7kaaTown->loc_y1
      && _7kaaCoordinates.y <= _7kaaTown->loc_y2
    ) {
      building->clearRallyPoint();
    } else {
      building->toggleRallyPoint(
        {
          .action = allowAction
            ? Ambition::Unit::Waypoint::Action::InteractWithBuilding
            : Ambition::Unit::Waypoint::Action::MoveOnly,
          .point = Coordinates::Point::from7kaaCoordinates(_7kaaCoordinates),
        }
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

std::shared_ptr<Building> Building::getBy7kaaFirm(
  const Firm* _7kaaFirm
) {
  return getBy7kaaFirmRecordNumber(_7kaaFirm->firm_recno);
}
std::shared_ptr<Building> Building::getBy7kaaFirmRecordNumber(
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
std::shared_ptr<Building> Building::getBy7kaaTown(
  const Town* _7kaaTown
) {
  return getBy7kaaTownRecordNumber(_7kaaTown->town_recno);
}
std::shared_ptr<Building> Building::getBy7kaaTownRecordNumber(
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
