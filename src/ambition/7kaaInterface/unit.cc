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
 * Implementation file for _7kaaAmbitionInterface::Unit.
 */

#define _AMBITION_IMPLEMENTATION
#include "7kaaInterface/unit.hh"

#include "Ambition_config.hh"
#include "Ambition_unit.hh"


namespace _7kaaAmbitionInterface::Unit {

bool sendAvailableBuilderToFirm(
  const Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::sendAvailableBuilderToFirm(firm);
}

} // namespace _7kaaAmbitionInterface::Unit
