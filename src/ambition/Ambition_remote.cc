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
 * Implementation file for Ambition::Remote.
 */

#include "Ambition_remote.hh"

#include <cstdio>

#include "pragma_silence_7kaa_warnings.hh"
#include "multiplayer.h"
#include "OCRC_STO.h"
#include "ODYNARR.h"
#include "OFONT.h"
#include "OREMOTE.h"
#include "OWORLD.h"
#pragma GCC diagnostic pop


namespace Ambition {

namespace Remote {

/* This has to be the exact same as the definitions in OGAMEMP.cpp.
 *
 * Why not just have one definition in a header file to be included instead of
 * multiple definitions in multiple source files, you ask?  Yes.
 */
constexpr auto CONTENT_SIZE = 61;
constexpr auto CHAT_MESSAGE_ID = 0x1f5a0017;
struct MpStructChatMsg {
  uint32_t msg_id;
  char sender[MP_FRIENDLY_NAME_LEN + 1] {"\0"};
  char content[CONTENT_SIZE] {"\0"};

  MpStructChatMsg(
    const std::string fromName,
    const std::string message
  ) : msg_id(CHAT_MESSAGE_ID) {
    std::snprintf(sender, sizeof(sender), "%s", fromName.c_str());
    std::snprintf(content, sizeof(content), "%s", message.c_str());
  }
};


std::vector<LobbyChatMessage> decode7kaaMessageList(
  DynArray& messageList
) {
  std::vector<LobbyChatMessage> lobbyChatMessages;

  std::vector<bool> _7kaaMessageProcessed(messageList.size() + 1, false);
  for (auto i = 1; i <= messageList.size(); i++) {
    if (_7kaaMessageProcessed[i]) {
      continue;
    }

    const auto base7kaaMessage
      = static_cast<MpStructChatMsg*>(messageList.get(i));
    auto message = LobbyChatMessage(base7kaaMessage->sender, {});

    for (auto j = i; j <= messageList.size(); j++) {
      const auto _7kaaMessage
        = static_cast<MpStructChatMsg*>(messageList.get(j));

      if (_7kaaMessage->sender != message.sender) {
        continue;
      }

      _7kaaMessageProcessed[j] = true;
      message.contents += _7kaaMessage->content;

      const auto contentsLength
        = strnlen(_7kaaMessage->content, sizeof(_7kaaMessage->content));
      if (contentsLength == sizeof(_7kaaMessage->content)
        || _7kaaMessage->content[contentsLength + 1] != SDLK_BACKSPACE
      ) {
        break;
      }
    }

    lobbyChatMessages.push_back(message);
  }

  return lobbyChatMessages;
}

} // namespace Ambition::Remote

void printSyncError(
) {
  String errorMessage;
  errorMessage = "Sync error: ";
  errorMessage += crc_store.crc_error_string;

  font_news.disp(
    ZOOM_X1 + 10,
    ZOOM_Y1 + 42,
    errorMessage,
    MAP_X2
  );
}

namespace Remote {

void sendLobbyChatHistory(
  const int _7kaaPlayerId,
  DynArray& messageList
) {
  for (auto i = 1; i <= messageList.size(); i++) {
    const auto storedMessage = static_cast<MpStructChatMsg*>(messageList.get(i));
    MpStructChatMsg message(storedMessage->sender, storedMessage->content);
    mp_obj.send(_7kaaPlayerId, &message, sizeof(message));
  }
}

void sendLobbyChatMessage(
  const std::string from,
  const std::string contents,
  DynArray& messageList,
  long& refreshFlag
) {
  std::string::size_type charactersSent = 0;

  while(charactersSent < contents.length()) {
    constexpr auto MAXIMUM_SIZE = sizeof(MpStructChatMsg::content) - 2;
    auto substring = contents.substr(charactersSent, MAXIMUM_SIZE);

    if (charactersSent + MAXIMUM_SIZE < contents.length()) {
      const auto splitPosition = substring.find_last_of(" -");

      constexpr auto MAXIMUM_CUTOFF = 20;
      constexpr auto MINIMUM_SPLIT_POSITION = MAXIMUM_SIZE - MAXIMUM_CUTOFF;
      if (splitPosition != std::string::npos
        && splitPosition > MINIMUM_SPLIT_POSITION
      ) {
        substring = contents.substr(
          charactersSent,
          splitPosition + (substring[splitPosition] == '-' ? 1 : 0)
        );
      }
    }

    MpStructChatMsg message(from, substring);

    if (charactersSent + substring.length() < contents.length()) {
      message.content[substring.length() + 1] = SDLK_BACKSPACE;
    }

    /* Bypass sending/receiving own message and add it in directly.  (Less than
     * ideal, but that's the way that 7kaa works.) */
    while (messageList.size() >= 4) {
      messageList.linkout(1);
    }
    messageList.linkin(&message);
    constexpr auto REFRESH_MESSAGES_AREA = 0x2;
    refreshFlag |= REFRESH_MESSAGES_AREA;

    /* Send the message to the other players. */
    mp_obj.send(BROADCAST_PID, &message, sizeof(message));

    charactersSent += substring.length();
  }
}

} // namespace Ambition::Remote

} // namespace Ambition
