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
 * Header file for Ambition::Vga.
 */

#include "Ambition_vga.hh"

#include <algorithm>
#include <SDL_events.h>
#include <SDL_timer.h>

#include "OAUDIO.h"
#include "OFIRM.h"
#include "OIMGRES.h"
#include "OREMOTE.h"
#include "OSYS.h"

#include "ambition/Ambition_config.hh"

namespace Ambition {

void delayFrame(
  const unsigned long long int deadlineSdlTicks64
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  constexpr unsigned long long int STANDARD_DELAY = 250;

  while (true) {
    if (SDL_PollEvent(nullptr)) {
      break;
    }
    if (sys.signal_exit_flag == 1) {
      break;
    }
    if (deadlineSdlTicks64 > 0
      && SDL_GetTicks64() >= deadlineSdlTicks64
    ) {
      break;
    }

    const auto ticksAvailable = deadlineSdlTicks64 - SDL_GetTicks64();
    SDL_WaitEventTimeout(nullptr, std::min(ticksAvailable, STANDARD_DELAY));

    audio.yield();
    if (remote.is_enable()
      && remote.poll_msg()
    ) {
      break;
    }
  }
}

} // namespace Ambition
