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
 * Implementation file for _7kaaAmbitionInterface::Control.
 */

#define _AMBITION_IMPLEMENTATION
#include "7kaaInterface/control.hh"

#include "Ambition_config.hh"
#include "Ambition_control.hh"
#include "Ambition_vga.hh"


namespace _7kaaAmbitionInterface::Control {

void delayFrame(
  const unsigned long long int deadlineSdlTicks64
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::delayFrame(deadlineSdlTicks64);
}

void requestFeedback(
) {
  Ambition::requestFeedback();
}

void resetGameState(
) {
  Ambition::resetGameState();
}

void unlockBuffer(
  VgaBuf& buffer
) {
  Ambition::unlockBuffer(buffer);
}

} // namespace _7kaaAmbitionInterface::Control
