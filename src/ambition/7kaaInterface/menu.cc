/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2025–2026 Tim Sviridov
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
 * Implementation file for _7kaaAmbitionInterface::Menu.
 */

#define _AMBITION_IMPLEMENTATION
#include "menu.hh"

#include <SDL2/SDL_clipboard.h>

#include "ConfigAdv.h"
#include "GAMEDEF.h"

#include "Ambition_config.hh"
#include "Ambition_user_interface.hh"
#include "Ambition_vga.hh"


namespace _7kaaAmbitionInterface::Menu {

constexpr auto RANDOM_CIVILISATION_INDEX = MAX_RACE;
constexpr auto RANDOM_CIVILISATION_ID = -1;

bool randomCivilisationSelected = false;


int calculateSelectedCivilisationSelectionButton(
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  if (randomCivilisationSelected) {
    return RANDOM_CIVILISATION_INDEX;
  }

  return _7kaaCalculation;
}

int calculateSelectedCivilisation7kaaRaceId(
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  if (_7kaaCalculation != RANDOM_CIVILISATION_ID) {
    randomCivilisationSelected = false;
    return _7kaaCalculation;
  }

  randomCivilisationSelected = true;
  return (
    config_adv.race_random_list[
      SDL_GetTicks64() % config_adv.race_random_list_max
    ]
  );
}

int civilisationSelectionButtonCount(
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return RANDOM_CIVILISATION_INDEX + 1;
}

void initialiseRandomCivilisationSelectionButton(
  bool multiplayer,
  ButtonCustomGroup& _7kaaButtonGroup,
  const ButtonCustomFP _7kaaButtonDraw
) {
  randomCivilisationSelected = false;

  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto randomCivilisationButtonArea
    = multiplayer
    ? Ambition::UserInterface::Multiplayer::GameSetup
      ::RANDOM_CIVILISATION_BUTTON
    : Ambition::UserInterface::Singleplayer::GameSetup
      ::RANDOM_CIVILISATION_BUTTON;

  constexpr auto IS_NOT_ELASTIC = 0;
  constexpr auto IS_NOT_PUSHED = 0;
  _7kaaButtonGroup[RANDOM_CIVILISATION_INDEX].create(
    randomCivilisationButtonArea.start.left,
    randomCivilisationButtonArea.start.top,
    randomCivilisationButtonArea.end.left,
    randomCivilisationButtonArea.end.top,
    _7kaaButtonDraw,
    ButtonCustomPara(&_7kaaButtonGroup, RANDOM_CIVILISATION_ID),
    IS_NOT_ELASTIC,
    IS_NOT_PUSHED
  );
}

std::string versionMismatchMessage(
  const std::string _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  SDL_SetClipboardText(Ambition::DirectoryPath::config().string().c_str());

  return Ambition::Vga::versionMismatchExtraMessage();
}

} // namespace _7kaaAmbitionInterface::Menu
