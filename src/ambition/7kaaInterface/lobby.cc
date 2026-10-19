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
 * Implementation file for _7kaaAmbitionInterface::Lobby.
 */

#define _AMBITION_IMPLEMENTATION
#include "lobby.hh"

#include <map>
#include <SDL2/SDL_clipboard.h>

#include "gettext.h"
#include "multiplayer.h"
#include "OREMOTE.h"

#include "Ambition_config.hh"
#include "Ambition_error_handling.hh"
#include "Ambition_remote.hh"
#include "Ambition_vga.hh"
#include "format.hh"


namespace _7kaaAmbitionInterface::Lobby {

const std::map<
  PlayerStatusTransition,
  std::string
> PLAYER_STATUS_TRANSITION_MESSAGES = {
  {
    RefusedVersion,
    /* TRANSLATORS: These ones are preceded by the player name, and are used in
     * the multiplayer lobby. */
    N_("attempted to join the game, but has a mismatching version.>"),
  },
  {
    RefusedFull,
    N_("was unable to join the game, because it is already full.>")
  },
  {
    RefusedAuthorisation,
    N_("attempted to join the game, but did not provide the correct password.>"),
  },
  { Connected, N_("joined the game.>") },
  {
    RefusedSaveFileRequired,
    N_("attempted to join the game, but did not choose a save file.>"),
  },
  {
    RefusedNoSaveFileRequired,
    N_("attempted to join the game, but was trying to load a save file.>"),
  },
  {
    RefusedSaveFileMismatch,
    N_("attempted to join the game, but did not choose a matching save file.>"),
  },
  { Disconnected, N_("has left the game.>") },
  { Ready, N_("is ready to start.>") },
  { Unready, N_("is no longer ready to start.>") },
};


void displayChatMessages(
  DynArray& messageList,
  long& refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  constexpr auto REFRESH_MESSAGES_AREA = 0x2;
  if (!(refreshFlag & REFRESH_MESSAGES_AREA)) {
    return;
  }

  Ambition::Vga::displayLobbyChat(
    Ambition::Remote::decode7kaaMessageList(messageList)
  );
  refreshFlag &= ~REFRESH_MESSAGES_AREA;
}

void sendChatHistory(
  const int _7kaaPlayerId,
  DynArray& messageList
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (!remote.is_host) {
    return;
  }

  Ambition::Remote::sendLobbyChatHistory(_7kaaPlayerId, messageList);
}

void sendPlayerNotification(
  const PlayerStatusTransition transition,
  const int _7kaaPlayerId,
  DynArray& messageList,
  long& refreshFlag
) {
  const auto _7kaaPlayer = mp_obj.search_player(_7kaaPlayerId);
  const auto name = _7kaaPlayer ? _7kaaPlayer->name : "A player";
  sendPlayerNotification(transition, name, messageList, refreshFlag);
}
void sendPlayerNotification(
  const PlayerStatusTransition transition,
  const std::string name,
  DynArray& messageList,
  long& refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (!remote.is_host) {
    return;
  }

  assume(PLAYER_STATUS_TRANSITION_MESSAGES.contains(transition));
  Ambition::Remote::sendLobbyChatMessage(
    format("<%s", name.c_str()),
    PLAYER_STATUS_TRANSITION_MESSAGES.at(transition),
    messageList,
    refreshFlag
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

} // namespace _7kaaAmbitionInterface::Lobby
