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
#include "ONATIONA.h"
#include "OREMOTE.h"
#include "OSNOW.h"
#include "OSYS.h"
#include "OTERRAIN.h"
#include "OUNIT.h"

#include "ambition/Ambition_config.hh"

namespace Ambition {

constexpr auto STEPS_PER_SECOND = 4;
constexpr auto MILLISECONDS_PER_STEP = 1000 / STEPS_PER_SECOND;

int calculateFirmFrame(
  const Firm* firm
);

FirmBitmap* calculateFirmBitmap(
  FirmBitmap* _7kaaCalculation,
  Firm* firm
) {
  if (!config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  if (!firm->under_construction
    && firm->is_operating()
  ) {
    const auto firmBuild = firm_res.get_build(firm->firm_build_id);
    return firm_res.get_bitmap(
      firmBuild->first_bitmap(calculateFirmFrame(firm))
    );
  }

  return _7kaaCalculation;
}

short calculateRainSpeed(
  const short _7kaaCalculation
) {
  if (!config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return _7kaaCalculation / 4;
}

char calculateRockRemainingDelay(
  const char _7kaaCalculation
) {
  if (!config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return _7kaaCalculation * 3;
}

char* calculateTerrainBitmap(
  char* _7kaaCalculation,
  const short terrainId,
  const int x,
  const int y
) {
  if (!config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return terrain_res[terrainId]->get_bitmap(
    misc.get_time() / MILLISECONDS_PER_STEP
    + x - y
  );
}

char calculateTownFlagNumber(
  const char _7kaaCalculation,
  const int townRecordNumber
) {
  if (!config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  constexpr auto STEP_COUNT = 4;

  return
    '1'
    + (char)(
      (SDL_GetTicks64() / MILLISECONDS_PER_STEP + townRecordNumber)
      % STEP_COUNT
    );
}

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

void drawFirmBuilderIcon(
  Firm* firm
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  constexpr auto FRAME_COUNT = 2;
  constexpr auto FRAME_RATE = 2;
  constexpr auto MILLISECONDS_PER_FRAME = 1000 / FRAME_RATE;

  // If there is a builder, draw an icon.
  if(firm->own_firm()
     && firm->builder_recno
     && unit_array[firm->builder_recno]->skill.skill_id == SKILL_CONSTRUCTION
  ) {
    const auto x = firm->loc_x1 * ZOOM_LOC_WIDTH - world.view_top_x + ZOOM_X1;
    const auto y = firm->loc_y1 * ZOOM_LOC_HEIGHT - world.view_top_y + ZOOM_Y1;
    char iconName[] = "REPAIR-1";

    if (firm->hit_points < firm->max_hit_points
        && nation_array[firm->nation_recno]->cash > 0
        && info.game_date > firm->last_attacked_date
    ) {
      iconName[7]
        = '1' + ((SDL_GetTicks64() / MILLISECONDS_PER_FRAME) % FRAME_COUNT);
    }
    const auto icon = image_icon.get_ptr(iconName);
    world.zoom_matrix->put_bitmap_clip(x, y, icon, 1);
  }
}

void drawFirmFrame(
  Firm* firm,
  const int displayLayer
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  const auto firmBuild = firm_res.get_build(firm->firm_build_id);

  if (!firmBuild->animate_full_size) {
    firm->draw_frame(1, displayLayer);
  }
  firm->draw_frame(calculateFirmFrame(firm), displayLayer);
}

bool initialiseSnowLayer(
  SnowLayer& layer,
  const int level,
  const char animationSpeed,
  double slideSpeed
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  layer.init(
    15 + 10 * level + animationSpeed,
    20 + 10 * level + animationSpeed,
    1 + level,
    level + 2,
    level / 2,
    slideSpeed,
    animationSpeed
  );

  return true;
}


/** Private functions. */

int calculateFirmFrame(
  const Firm* firm
) {
  constexpr auto STEPS_PER_SECOND = 12;
  constexpr auto MILLISECONDS_PER_STEP = 1000 / STEPS_PER_SECOND;

  const auto firmBuild = firm_res.get_build(firm->firm_build_id);

  const auto startingFrame = firmBuild->animate_full_size ? 1 : 2;
  const auto animatedFrameCount = firmBuild->frame_count - startingFrame + 1;

  int totalSteps = 0;

  short frameSteps[animatedFrameCount];
  for (int i = 0; i < animatedFrameCount; i++) {
    frameSteps[i] = 1 + firmBuild->frame_delay(i);
    totalSteps += frameSteps[i];
  }

  auto step
    = (SDL_GetTicks64() / MILLISECONDS_PER_STEP
      + firm->firm_recno)
    % totalSteps;

  int frame = startingFrame;
  while(step > frameSteps[frame - startingFrame]) {
    step -= frameSteps[frame - startingFrame];
    frame++;
  }

  return frame;
}

void drawHitbarOutline(
  const bool isOwn,
  const int x,
  const int y,
  const int width,
  const int height
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  const auto pitch = width + 2;

  auto dataPtr = sys.common_data_buf;

  *((short*)dataPtr) = pitch;
  *(((short*)dataPtr) + 1) = height + 2;

  dataPtr += sizeof(short) * 2;

  const auto outlineColour = isOwn ? VGA_LIGHT_GREEN : VGA_RED;

  IMGbar(dataPtr, pitch, 1, 1, width, height, 0xFF);
  IMGbar(dataPtr, pitch, 0, 0, width + 1, 0, outlineColour);
  IMGbar(dataPtr, pitch, 0, 0, 0, height + 1, outlineColour);
  IMGbar(dataPtr, pitch, 0, height + 1, width + 1, height + 1, outlineColour);
  IMGbar(dataPtr, pitch, width + 1, 0, width + 1, height + 1, outlineColour);

  world.zoom_matrix->put_bitmap_clip(x - 1, y - 1, sys.common_data_buf);
}

} // namespace Ambition
