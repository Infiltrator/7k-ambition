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
#include <gettext.h>
#include <SDL_events.h>
#include <SDL_timer.h>

#include "OAUDIO.h"
#include "OFIRM.h"
#include "OFONT.h"
#include "OIMGRES.h"
#include "ONATIONA.h"
#include "OREMOTE.h"
#include "OSNOW.h"
#include "OSTR.h"
#include "OSYS.h"
#include "OTERRAIN.h"
#include "OUNIT.h"
#include "vga_util.h"

#include "ambition/Ambition_config.hh"

namespace Ambition {

constexpr auto STEPS_PER_SECOND = 4;
constexpr auto MILLISECONDS_PER_STEP = 1000 / STEPS_PER_SECOND;

int calculateFirmFrame(
  const Firm* firm
);

void drawHitbar(
  const bool isOwn,
  const char baseColour,
  const double filledFraction,
  const int x,
  const int y,
  const int width,
  const int height
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

int calculateUnitHitbarWidth(
  const int _7kaaCalculation
) {
  if (!config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return ZOOM_LOC_WIDTH - 2;
}

int calculateUnitIconY(
  const int _7kaaCalculation
) {
  if (!config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  constexpr auto UNIT_HITBAR_HEIGHT = 4;
  constexpr auto ICON_OFFSET = UNIT_HITBAR_HEIGHT + 4;

  return _7kaaCalculation + ICON_OFFSET;
}

int calculateWorkerPortraitX(
  const int _7kaaCalculation,
  const int workerIndex
) {
  if (!config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  constexpr auto COLUMN_COUNT = 4;
  constexpr auto COLUMN_SIZE = 50;

  return INFO_X1 + 2 + (workerIndex % COLUMN_COUNT) * COLUMN_SIZE;
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

void displayGameSpeed(
  int speed
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  const auto savedUseBackBuffer = vga.use_back_buf;

  vga.use_front();

  String str = _("Speed: ");
  if (speed == 0) {
    str += _("PAUSED");
  } else if (speed >= 99) {
    str += _("UNLIMITED");
  } else {
    str += speed / 3;
  }
  font_news.right_put(
    ZOOM_X2 - 4,
    ZOOM_Y1 + 4,
    str
  );

  if (speed == 0) {
    font_news.center_put(
      ZOOM_X1,
      ZOOM_Y1,
      ZOOM_X2,
      ZOOM_Y2,
      _("GAME IS PAUSED")
    );
  }

  if (savedUseBackBuffer) {
    vga.use_back();
  }
}

bool drawBuildingOccupantHitbar(
  const int leftX,
  const int topY,
  const int width,
  const int currentHitpoints,
  const int maximumHitpoints
) {
  if (!config.enhancementsAvailable()) {
    return false;
  }

  constexpr auto HITBAR_COLOUR_COUNT = 3;
  constexpr auto GREY_COLOUR = VGA_GRAY + 8;

  struct ColourThreshold {
    int threshold;
    unsigned char colour;
  };

  constexpr ColourThreshold BASE_COLOUR_THRESHOLDS[HITBAR_COLOUR_COUNT] = {
    {
      .threshold = 0,
      .colour = VGA_LIGHT_GREEN + 1,
    },
    {
      .threshold = 50,
      .colour = VGA_YELLOW + 1,
    },
    {
      .threshold = 100,
      .colour = VGA_PURPLE + 1,
    },
  };

  unsigned char _baseColour;

  for (auto i = 0; i < HITBAR_COLOUR_COUNT; i++) {
    if (maximumHitpoints >= BASE_COLOUR_THRESHOLDS[i].threshold) {
      _baseColour = BASE_COLOUR_THRESHOLDS[i].colour;
    } else {
      break;
    }
  }

  const auto baseColour = _baseColour;
  const auto darkColour = baseColour + 2;
  const auto filledWidth = width * currentHitpoints / maximumHitpoints;
  /** The separating point between the filled and empty areas. */
  const auto emptyX = leftX + filledWidth;
  const auto rightX = leftX + width;

  // Top two lines filled.
  vga_front.bar(leftX, topY, emptyX, topY + 1, baseColour);
  // Clear out the empty portion.
  if (filledWidth < width) {
    vga_util.blt_buf(emptyX + 1, topY, rightX, topY + 1, V_BLACK);
  }

  // Dark line for the filled portion.
  vga_front.bar(leftX, topY + 2, emptyX, topY + 2, darkColour);
  // Grey line for the unfilled portion.
  if (filledWidth < width) {
    vga_front.bar(emptyX + 1, topY + 2, rightX, topY + 2, GREY_COLOUR);
  }
  // And a black pixel at the very end.
  vga_front.bar(rightX + 1, topY + 2, rightX + 1, topY + 2, V_BLACK);

  // Black underline all the way.
  vga_front.bar(leftX + 1, topY + 3, rightX + 1, topY + 3, V_BLACK);

  return true;
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
  constexpr auto ICON_VERTICAL_OFFSET = 10;

  // If there is a builder, draw an icon.
  if(firm->own_firm()
     && firm->builder_recno
     && unit_array[firm->builder_recno]->skill.skill_id == SKILL_CONSTRUCTION
  ) {
    const auto x = firm->loc_x1 * ZOOM_LOC_WIDTH - world.view_top_x + ZOOM_X1;
    const auto y
      = firm->loc_y1 * ZOOM_LOC_HEIGHT
      - world.view_top_y + ZOOM_Y1
      + ICON_VERTICAL_OFFSET;
    char iconName[] = "REPAIR-1";

    if (firm->hit_points < firm->max_hit_points
        && nation_array[firm->nation_recno]->cash > 0
        && info.game_date > firm->last_attacked_date + 1
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

void drawFirmHitBar(
  Firm* firm
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  if(firm->hit_points >= firm->max_hit_points) {
    return;
  }

  // If the firm is not at 100% HP, draw a hit bar.
  const auto barWidth = ZOOM_LOC_WIDTH * (firm->loc_x2 - firm->loc_x1 + 1);
  constexpr auto BAR_HEIGHT = 5;

  const auto barX = firm->loc_x1 * ZOOM_LOC_WIDTH - world.view_top_x + ZOOM_X1;
  const auto barY = firm->loc_y1 * ZOOM_LOC_HEIGHT - world.view_top_y + ZOOM_Y1;

  const auto hitpointsFraction = firm->hit_points / firm->max_hit_points;

  drawHitbar(
    firm->own_firm(),
    VGA_DARK_BLUE,
    hitpointsFraction,
    barX,
    barY,
    barWidth,
    BAR_HEIGHT
  );
}

void drawInnGuestCount(
  const short count,
  const int refreshFlag
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  const auto panelLeft = INFO_X1;
  constexpr auto PANEL_TOP = INFO_Y1 + 50;
  const auto panelRight = INFO_X2;
  constexpr auto PANEL_BOTTOM = PANEL_TOP + 24;
  const auto fieldLeft = panelLeft + 4;
  const auto fieldValueLeft = fieldLeft + 100;
  const auto fieldRight = panelRight - 4;
  constexpr auto FIELD_TOP = PANEL_TOP + 3;

  if (refreshFlag == INFO_REPAINT) {
    vga_util.d3_panel_up(panelLeft, PANEL_TOP, panelRight, PANEL_BOTTOM);
  }
  font_san.field(
    fieldLeft,
    FIELD_TOP,
    _("Guests"),
    fieldValueLeft,
    count,
    1,
    fieldRight,
    refreshFlag
  );
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

void unlockBuffer(
  VgaBuf& buffer
) {
  constexpr auto TARGET_FPS = 60;
  constexpr auto MILLISECONDS_PER_FRAME = 1000 / TARGET_FPS;

  static auto nextFlipTick = SDL_GetTicks64();

  const auto currentTick = SDL_GetTicks64();
  if (currentTick >= nextFlipTick) {
    nextFlipTick = currentTick + MILLISECONDS_PER_FRAME;

    if (buffer.buf_locked) {
      buffer.unlock_buf();
    }
  } else {
    buffer.buf_locked = 0;
  }
}


/* Private functions. */

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

void drawHitbar(
  const bool isOwn,
  const char baseColour,
  const double filledFraction,
  const int x,
  const int y,
  const int width,
  const int height
) {
  const auto HITBAR_LIGHT_BORDER_COLOUR = baseColour;
  const auto HITBAR_DARK_BORDER_COLOUR = baseColour + 3;
  const auto HITBAR_BODY_COLOUR = baseColour + 1;

  constexpr auto EMPTY_BASE_COLOUR = 0x40;
  constexpr auto EMPTY_LIGHT_BORDER_COLOUR = EMPTY_BASE_COLOUR + 11;
  constexpr auto EMPTY_DARK_BORDER_COLOUR = EMPTY_BASE_COLOUR + 3;
  constexpr auto EMPTY_BODY_COLOUR = EMPTY_BASE_COLOUR + 7;

  auto dataPtr = sys.common_data_buf;
  const auto pitch = width;
  /** The separating point between the filled and empty areas. */
  const int emptyX = (width - 1) * filledFraction;

  *((short*)dataPtr) = pitch;
  *(((short*)dataPtr)+1) = height;

  dataPtr += sizeof(short) * 2;

  const auto bottomY = height - 1;
  const auto darkY = bottomY - 2;

#define drawBar(x1, y1, x2, y2, colour) \
  IMGbar(dataPtr, pitch, (x1), (y1), (x2), (y2), (colour))

  // Light left line filled.
  drawBar(0, 0, 0, bottomY, HITBAR_LIGHT_BORDER_COLOUR);

  // Light top line filled.
  drawBar(0, 0, emptyX, 0, HITBAR_LIGHT_BORDER_COLOUR);
  // Light top line empty.
  if (emptyX < width - 1) {
    drawBar(emptyX + 1, 0, width - 1, 0, EMPTY_LIGHT_BORDER_COLOUR);
  }

  // Dark right line.
  if (emptyX == width - 1) { // Filled.
    drawBar(width - 1, 1, width - 1, 1, HITBAR_DARK_BORDER_COLOUR);
  } else { // Empty.
    drawBar(width - 1, 1, width - 1, 1, EMPTY_DARK_BORDER_COLOUR);
  }

  // Bar body filled.
  drawBar(1, 1, std::min(emptyX, width - 2), 1, HITBAR_BODY_COLOUR);
  // Bar body empty.
  if (emptyX < width - 2) {
    drawBar(emptyX + 1, 1, width - 2, darkY, EMPTY_BODY_COLOUR);
  }

  // Dark bottom line filled.
  drawBar(1, darkY, emptyX, bottomY, HITBAR_DARK_BORDER_COLOUR);
  // Dark bottom line empty.
  if (emptyX < width - 1) {
    drawBar(emptyX + 1, darkY, width - 1, bottomY, EMPTY_DARK_BORDER_COLOUR);
  }

  world.zoom_matrix->put_bitmap_clip(x, y, sys.common_data_buf);

  drawHitbarOutline(isOwn, x, y, pitch, height);
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
  const auto shadowColour = outlineColour + 3;

  IMGbar(dataPtr, pitch, 1, 1, width, height, 0xFF);
  IMGbar(dataPtr, pitch, 0, 0, width + 1, 0, outlineColour);
  IMGbar(dataPtr, pitch, 0, 0, 0, height + 1, outlineColour);
  IMGbar(dataPtr, pitch, 1, height + 1, width + 1, height + 1, shadowColour);
  IMGbar(dataPtr, pitch, width + 1, 1, width + 1, height + 1, shadowColour);

  world.zoom_matrix->put_bitmap_clip(x - 1, y - 1, sys.common_data_buf);
}

} // namespace Ambition
