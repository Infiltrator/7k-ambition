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
 * Implementation file for _7kaaAmbitionInterface::Input.
 */

#define _AMBITION_IMPLEMENTATION
#include "input.hh"

#include "OFIRMA.h"
#include "OMOUSE.h"
#include "OTOWN.h"
#include "OWORLD.h"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_input.hh"
#include "Ambition_user_interface.hh"


namespace _7kaaAmbitionInterface::Input {

void calculateScroll(
  int& x,
  int& y
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::calculateScroll(x, y);
}

bool detectModeSelectClick(
) {
  return Ambition::detectModeSelectClick();
}

bool detectRallyButtonClick(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  std::shared_ptr<Ambition::Building> building;
  if (firm_array.selected_recno) {
    building = Ambition::Building::findBy7kaaFirmRecordNumber(
      firm_array.selected_recno
    );
  }
  if (town_array.selected_recno) {
    building = Ambition::Building::findBy7kaaTownRecordNumber(
      town_array.selected_recno
    );
  }

  if (!building) {
    return false;
  }

  const auto buttonClickArea
    = Ambition::UserInterface::RALLY_POINT_BUTTON.outer(2);

  if (mouse.single_click(
      buttonClickArea.start.left,
      buttonClickArea.start.top,
      buttonClickArea.end.left,
      buttonClickArea.end.top,
      0
    )
  ) {
    world.go_loc(
      building->getRallyLocation().to7kaaCoordinates().x,
      building->getRallyLocation().to7kaaCoordinates().y
    );
    return true;
  }

  if (mouse.single_click(
      buttonClickArea.start.left,
      buttonClickArea.start.top,
      buttonClickArea.end.left,
      buttonClickArea.end.top,
      1
    )
  ) {
    building->clearRallyPoint();
    return true;
  }

  return false;
}

void setOrClearRallyPoint(
  const short _7kaaX,
  const short _7kaaY,
  const bool allowAction
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto location = world.get_loc(_7kaaX, _7kaaY);
  Ambition::setOrClearRallyPoint(
    { .x =_7kaaX, .y = _7kaaY },
    allowAction && location->explored()
  );
}

} // namespace _7kaaAmbitionInterface::Input
