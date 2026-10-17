/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2026 Tim Sviridov
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
 * Header file for _7kaaAmbitionInterface::Lobby.
 */

#pragma once

#include <string>

class DynArray;


namespace _7kaaAmbitionInterface::Lobby {

enum PlayerStatusTransition {
  RefusedVersion,
  RefusedFull,
  RefusedAuthorisation,
  Connected,
  RefusedSaveFileRequired,
  RefusedNoSaveFileRequired,
  RefusedSaveFileMismatch,
  Disconnected,
  Ready,
  Unready,
};


void sendPlayerNotification(
  const PlayerStatusTransition transition,
  const int _7kaaPlayerId,
  DynArray& messageList,
  long& refreshFlag
);
void sendPlayerNotification(
  const PlayerStatusTransition transition,
  const std::string name,
  DynArray& messageList,
  long& refreshFlag
);

} // namespace _7kaaAmbitionInterface::Lobby

#ifndef _AMBITION_IMPLEMENTATION
/** Allow 7kaa to call using Ambition::*. */
namespace Ambition = _7kaaAmbitionInterface;
#endif
