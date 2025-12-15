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

#include <SDL.h>

#include "OCONFIG.h"
#include "OFIRMA.h"
#include "OINFO.h"
#include "OMUSIC.h"
#include "OREMOTE.h"
#include "OSYS.h"
#include "OU_CARA.h"
#include "OU_MARI.h"

#include "Ambition_config.hh"
#include "Ambition_control.hh"
#include "Ambition_news.hh"
#include "Ambition_user_interface.hh"
#include "Ambition_version.hh"
#include "Ambition_vga.hh"
#include "format.hh"


namespace _7kaaAmbitionInterface::Control {

bool tradeStopRequestValid(
  const int _7kaaStopId,
  const char _7kaaStopsDefinedCount,
  const short _7kaaFirmRecordNumber
);


void copyMapIdToClipboard(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  constexpr const char* LAND_MASS_STRINGS[] = {
    "Small",
    "Medium",
    "Large",
  };

  SDL_SetClipboardText(
    format(
      "%llu %s",
      info.random_seed,
      LAND_MASS_STRINGS[config.land_mass - 1]
    ).c_str()
  );
}

void delayFrame(
  const unsigned long long int deadlineSdlTicks64
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::delayFrame(deadlineSdlTicks64);
}

void displayNews(
) {
  if (Ambition::versionString() != Ambition::News::lastDisplayedNewsVersion()) {
    Ambition::News::display();
    Ambition::News::saveDisplayedNewsVersion();
  }
}

void finishReplay(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::Control::finishReplay();
}

void migrateLocalDataDirectories(
) {
  Ambition::Control::migrateLocalDataDirectories();
}

void pasteFromClipboard(
  char* destination,
  const unsigned int maximumSize
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto buffer = SDL_GetClipboardText();
  strncpy(destination, buffer, maximumSize);
  SDL_free(buffer);
}

void playEncyclopaediaMusic(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  constexpr auto MUSIC_TRACK_NUMBER_VICTORY = 9;

  if (config.music_flag) {
    if (!music.is_playing(1)) {
      music.play(
        MUSIC_TRACK_NUMBER_VICTORY,
        MUSIC_PLAY_LOOPED | (sys.cdrom_drive ? MUSIC_CD_THEN_WAV : 0)
      );
    }
  } else {
    music.stop();
  }
}

bool preventReplayDeleteStopDesync(
  const UnitCaravan* _7kaaCaravan,
  const int _7kaaStopId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  if (_7kaaCaravan->stop_array[_7kaaStopId - 1].firm_recno == 0) {
    return true;
  }

  return false;
}
bool preventReplayDeleteStopDesync(
  const UnitMarine* _7kaaTradeShip,
  const int _7kaaStopId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  if (_7kaaTradeShip->stop_array[_7kaaStopId - 1].firm_recno == 0) {
    return true;
  }

  return false;
}

bool preventReplaySetStopPickupDesync(
  const int _7kaaActionType
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return (remote.is_replay() && _7kaaActionType == COMMAND_PLAYER);
}

bool preventReplaySetStopPickupDesync(
  const UnitCaravan* _7kaaCaravan,
  const int _7kaaStopId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return !tradeStopRequestValid(
    _7kaaStopId,
    _7kaaCaravan->stop_defined_num,
    _7kaaCaravan->stop_array[_7kaaStopId - 1].firm_recno
  );
}
bool preventReplaySetStopPickupDesync(
  const UnitMarine* _7kaaTradeShip,
  const int _7kaaStopId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return !tradeStopRequestValid(
    _7kaaStopId,
    _7kaaTradeShip->stop_defined_num,
    _7kaaTradeShip->stop_array[_7kaaStopId - 1].firm_recno
  );
}

void requestFeedback(
) {
  Ambition::requestFeedback();
}

void resetGameState(
) {
  Ambition::resetGameState();
}

void resetUiState(
) {
  Ambition::UserInterface::resetState();
}

bool shouldDrawFrame(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return true;
  }

  constexpr auto FRAMES_PER_SECOND = 60;
  constexpr auto MILLISECONDS_PER_FRAME = 1000 / FRAMES_PER_SECOND;

  static auto nextFrameTime = SDL_GetTicks64();
  const auto now = SDL_GetTicks64();

  if (now >= nextFrameTime) {
    nextFrameTime = now + MILLISECONDS_PER_FRAME;
    return true;
  }
  return false;
}

void unlockBuffer(
  VgaBuf& buffer
) {
  Ambition::unlockBuffer(buffer);
}


/* Private functions */

bool tradeStopRequestValid(
  const int _7kaaStopId,
  const char _7kaaStopsDefinedCount,
  const short _7kaaFirmRecordNumber
) {
  if (_7kaaStopId > _7kaaStopsDefinedCount) {
    return false;
  }

  if (firm_array.is_deleted(_7kaaFirmRecordNumber)) {
    return false;
  }

  return true;
}

} // namespace _7kaaAmbitionInterface::Control
