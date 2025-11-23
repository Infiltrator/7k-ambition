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
 * Implementation file for _7kaaAmbitionInterface::Menu.
 */

#define _AMBITION_IMPLEMENTATION
#include "menu.hh"

#include <SDL2/SDL_clipboard.h>

#include "Ambition_config.hh"
#include "Ambition_vga.hh"


namespace _7kaaAmbitionInterface::Menu {

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
