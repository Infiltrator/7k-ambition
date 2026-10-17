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

void sendLobbyChatMessage(
  const std::string from,
  const std::string contents,
  DynArray& messageList,
  long& refreshFlag
) {
  MpStructChatMsg message(from, contents);

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
}

} // namespace Ambition::Remote

} // namespace Ambition
