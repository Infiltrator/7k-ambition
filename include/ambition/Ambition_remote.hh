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
 * Header file for Ambition::Remote.
 */

#pragma once

#include <string>

class DynArray;


namespace Ambition {

void printSyncError();

namespace Remote {

void sendLobbyChatHistory(
  const int _7kaaPlayerId,
  DynArray& messageList
);

void sendLobbyChatMessage(
  const std::string from,
  const std::string contents,
  DynArray& messageList,
  long& refreshFlag
);

} // namespace Ambition::Remote

} // namespace Ambition
