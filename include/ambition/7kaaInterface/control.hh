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
 * Header file for _7kaaAmbitionInterface::Control.
 */

#pragma once

class VgaBuf;


namespace _7kaaAmbitionInterface::Control {

void copyMapIdToClipboard(
);

void delayFrame(
  const unsigned long long int deadlineSdlTicks64 = 0
);

void displayNews(
);

void requestFeedback(
);

void resetGameState(
);

/**
 * Unlock a VgaBuf, overriding the usual buffer unlock steps as necessary.
 *
 * This is needed because sometimes unlocking a VgaBuf also causes a Vga flip
 * and we want to control when the flips occur.
 *
 * @param buffer The VgaBuf to unlock.
 */
void unlockBuffer(
  VgaBuf& buffer
);

} // namespace _7kaaAmbitionInterface::Control

#ifndef _AMBITION_IMPLEMENTATION
/** Allow 7kaa to call using Ambition::*. */
namespace Ambition = _7kaaAmbitionInterface;
#endif
