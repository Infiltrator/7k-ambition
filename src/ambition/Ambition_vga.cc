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
#include <cmath>
#include <gettext.h>
#include <SDL_events.h>
#include <SDL_timer.h>

#define _AMBITION_IMPLEMENTATION
#include "OANLINE.h"
#include "OAUDIO.h"
#include "OBUTT3D.h"
#include "OCONFIG.h"
#include "OF_HARB.h"
#include "OF_INN.h"
#include "OF_MARK.h"
#include "OF_RESE.h"
#include "OF_WAR.h"
#include "OFIRM.h"
#include "OFONT.h"
#include "OIMGRES.h"
#include "OMOUSE.h"
#include "ONATIONA.h"
#include "OPOWER.h"
#include "OREMOTE.h"
#include "OSNOW.h"
#include "OSPY.h"
#include "OSTR.h"
#include "OSYS.h"
#include "OTECHRES.h"
#include "OTERRAIN.h"
#include "OUNIT.h"
#include "vga_util.h"

#include "7kaaInterface/draw.hh"
#include "7kaaInterface/input.hh"
#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_polity.hh"
#include "Ambition_spy.hh"
#include "Ambition_unit.hh"
#include "Ambition_user_interface.hh"
#include "format.hh"


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


short calculateAnimatedLinePhase(
  const int animatedFlag,
  const int lineProgress
) {
  constexpr auto PHASE_COUNT = 8;
  constexpr auto PHASES_PER_SECOND = 60;
  constexpr auto MILLISECONDS_PER_PHASE = 1000 / PHASES_PER_SECOND;

  const auto timeProgress
    = animatedFlag
    ? SDL_GetTicks64() / PHASES_PER_SECOND
    : 0;

  return (unsigned short)(lineProgress - timeProgress) % PHASE_COUNT;
}


FirmBitmap* calculateFirmBitmap(
  FirmBitmap* _7kaaCalculation,
  Firm* firm
) {
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

int calculateHitbarBaseColour(
  const double maximumHitpoints
) {
  constexpr auto HITBAR_COLOUR_COUNT = 6;

  struct ColourThreshold {
    int threshold;
    unsigned char colour;
  };

  constexpr ColourThreshold COLOUR_THRESHOLDS[HITBAR_COLOUR_COUNT] = {
    {
      .threshold = -1,
      .colour = VGA_DARK_GREEN,
    },
    {
      .threshold = 60,
      .colour = VGA_LIGHT_GREEN,
    },
    {
      .threshold = 95,
      .colour = VGA_YELLOW,
    },
    {
      .threshold = 130,
      .colour = VGA_ORANGE,
    },
    {
      .threshold = 165,
      .colour = VGA_PURPLE,
    },
    {
      .threshold = 200,
      .colour = VGA_RED,
    },
  };

  for (auto i = 0; i < HITBAR_COLOUR_COUNT; i++) {
    if (maximumHitpoints < COLOUR_THRESHOLDS[i].threshold) {
      return COLOUR_THRESHOLDS[i - 1].colour;
    }
  }

  return COLOUR_THRESHOLDS[HITBAR_COLOUR_COUNT - 1].colour;
}

int calculateHitbarWidth(
  const int availableWidth,
  const double maximumHitpoints
) {
  constexpr auto FULL_WIDTH_HITPOINTS = 200.0;

  return availableWidth * std::sqrt(maximumHitpoints / FULL_WIDTH_HITPOINTS);
}

short calculateRainSpeed(
  const short _7kaaCalculation
) {
  return _7kaaCalculation / 4;
}

char calculateRockRemainingDelay(
  const char _7kaaCalculation
) {
  return _7kaaCalculation * 3;
}

char* calculateTerrainBitmap(
  const short terrainId,
  const int x,
  const int y
) {
  return terrain_res[terrainId]->get_bitmap(
    misc.get_time() / MILLISECONDS_PER_STEP
    + x - y
  );
}

char calculateTownFlagNumber(
  const int townRecordNumber
) {
  constexpr auto STEP_COUNT = 4;

  return
    '1'
    + (char)(
      (SDL_GetTicks64() / MILLISECONDS_PER_STEP + townRecordNumber)
      % STEP_COUNT
    );
}

int calculateUnitHitbarWidth(
) {
  return ZOOM_LOC_WIDTH - 2;
}

int calculateUnitIconY(
  const int _7kaaCalculation
) {
  constexpr auto UNIT_HITBAR_HEIGHT = 4;
  constexpr auto GAP = 4;
  constexpr auto ICON_OFFSET = UNIT_HITBAR_HEIGHT + GAP;

  return _7kaaCalculation + ICON_OFFSET;
}

int calculateWorkerPortraitX(
  const int workerIndex
) {
  constexpr auto COLUMN_COUNT = 4;
  constexpr auto COLUMN_SIZE = 50;

  return
    (UserInterface::INFO_PANE_CONTENTS.start.left + 2)
    + ((workerIndex % COLUMN_COUNT) * COLUMN_SIZE);
}

void copyFrontBufferToBack(
  const UserInterface::Rectangle& area
) {
  mouse.hide_area(
    area.start.left,
    area.start.top,
    area.end.left,
    area.end.top
  );

  IMGcopy(
    vga_back.buf_ptr(),
    vga_back.buf_pitch(),
    vga_front.buf_ptr(),
    vga_front.buf_pitch(),
    area.start.left,
    area.start.top,
    area.end.left,
    area.end.top
  );

  mouse.show_area();
}

void delayFrame(
  const unsigned long long int deadlineSdlTicks64
) {
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

int centreHitbar(
  const int left,
  const int maximumWidth,
  const int currentWidth
) {
  return left + (maximumWidth - currentWidth) / 2;
}

void displayAssassinationConfirmationMenu(
  const int refreshFlag,
  const ::Spy* _7kaaSpy,
  const Firm* _7kaaFirm,
  ::Unit* target
) {
  if (refreshFlag != INFO_REPAINT) {
    return;
  }

  const auto titleArea = UserInterface::INFO_PANE_CONTENTS.internal(
    {
      .width = UserInterface::INFO_PANE_CONTENTS.width(),
      .height = 19,
    }
  );

  UserInterface::drawPanel(titleArea);
  UserInterface::printText(
    font_san,
    _("Assassinate"),
    titleArea,
    UserInterface::Clear::None,
    UserInterface::HorizontalAlignment::Centre,
    UserInterface::VerticalAlignment::Centre
  );

  const auto panelArea
    = UserInterface::INFO_PANE_CONTENTS
    .inner(0, 22, 0, 0)
    .internal(
      {
        .width = UserInterface::INFO_PANE_CONTENTS.width(),
        .height = 46,
      }
    );

  UserInterface::drawPanel(panelArea);

  const auto bitmap
    = unit_res[target->unit_id]->get_large_icon_ptr(target->rank_id);
  const auto bitmapSize = UserInterface::bitmapSize(bitmap);

  const auto bitmapArea = panelArea.inner(6, 4);

  vga_front.put_bitmap(bitmapArea.start.left, bitmapArea.start.top, bitmap);

  const auto textArea = panelArea.inner(12 + bitmapSize.width, 4, 0, 0);

  UserInterface::printText(font_san, target->unit_name(), textArea);
  UserInterface::printText(
    font_san,
    format(_("Hit Points: %.0f"), target->hit_points),
    textArea.inner(0, 20)
  );
  printAssasinationEstimate(_7kaaSpy, _7kaaFirm, target);

  Spy::assassinationButton.paint(
    UserInterface::ASSASSINATION_BUTTON.start.left,
    UserInterface::ASSASSINATION_BUTTON.start.top,
    'A',
    "ASSASSIN"
  );
  _7kaaAmbitionInterface::Draw::buttonKeybind(
    _7kaaAmbitionInterface::Input::getKeyEvent(
      _7kaaAmbitionInterface::Input::Action::Common_Confirm
    ),
    Spy::assassinationButton
  );

  Spy::cancelButton.paint(
    UserInterface::CANCEL_BUTTON.start.left,
    UserInterface::CANCEL_BUTTON.start.top,
    'A',
    "PREVMENU"
  );
  _7kaaAmbitionInterface::Draw::buttonKeybind(
    _7kaaAmbitionInterface::Input::getKeyEvent(
      _7kaaAmbitionInterface::Input::Action::Common_Cancel
    ),
    Spy::cancelButton
  );
}

void displayGameSpeed(
  int speed
) {
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

void displayInnGuestDetailsStayingInformation(
  const InnUnit* _7kaaInnGuest,
  const int refreshFlag
) {
  const auto fieldArea
    = UserInterface::INFO_PANE_CONTENTS
    .inner(4)
    .inner(0, 247, 0, 0)
    .internal(
      {
        .width = 190,
        .height = 16,
      }
    );

  font_san.field(
    fieldArea.start.left,
    fieldArea.start.top,
    _("Status"),
    fieldArea.inner(100, 0, 0, 0).start.left,
    _7kaaInnGuest->stay_count <= 2 ? _("Leaving soon") : _("Enjoying stay"),
    fieldArea.end.left,
    refreshFlag
  );
}

void displayInnGuestLeavingSoonMark(
  const InnUnit* _7kaaInnGuest,
  const int top
) {
  constexpr auto STAY_CYCLE_LOW_THRESHOLD = 2;

  if (_7kaaInnGuest->stay_count <= STAY_CYCLE_LOW_THRESHOLD) {
    const auto textArea = Ambition::UserInterface::Rectangle::fromPoint(
      {
        .left = 750,
        .top = top + 2,
      },
      {
        .width = 20,
        .height = 20,
      }
    );
    Ambition::UserInterface::printText(
      font_san,
      _("*"),
      textArea,
      Ambition::UserInterface::Clear::None,
      Ambition::UserInterface::HorizontalAlignment::Right
    );
  }
}

void displayStealReportsConfirmationMenu(
  const int refreshFlag,
  const ::Spy* _7kaaSpy,
  const int report
) {
  if (refreshFlag != INFO_REPAINT) {
    return;
  }

  const auto titleArea = UserInterface::INFO_PANE_CONTENTS.internal(
    {
      .width = UserInterface::INFO_PANE_CONTENTS.width(),
      .height = 19,
    }
  );

  UserInterface::drawPanel(titleArea);
  UserInterface::printText(
    font_san,
    _("Steal Reports"),
    titleArea,
    UserInterface::Clear::None,
    UserInterface::HorizontalAlignment::Centre,
    UserInterface::VerticalAlignment::Centre
  );

  const auto panelArea
    = UserInterface::INFO_PANE_CONTENTS
    .inner(0, 22, 0, 0)
    .internal(
      {
        .width = UserInterface::INFO_PANE_CONTENTS.width(),
        .height = 19,
      }
    );

  UserInterface::drawPanel(panelArea);

  const auto textArea = panelArea.inner(4);

  constexpr const char* REPORT_NAMES[] = {
    "Kingdoms Report",
    "Villages Report",
    "Economy Report",
    "Trade Report",
    "Military Report",
    "Technology Report",
    "Espionage Report",
  };

  UserInterface::printText(font_san, _(REPORT_NAMES[report]), textArea);
  printStealReportsEstimate(_7kaaSpy, report);

  Spy::stealReportsButton.paint(
    UserInterface::ASSASSINATION_BUTTON.start.left,
    UserInterface::ASSASSINATION_BUTTON.start.top,
    'A',
    "VSECRET"
  );
  _7kaaAmbitionInterface::Draw::buttonKeybind(
    _7kaaAmbitionInterface::Input::getKeyEvent(
      _7kaaAmbitionInterface::Input::Action::Common_Confirm
    ),
    Spy::stealReportsButton
  );

  Spy::cancelButton.paint(
    UserInterface::CANCEL_BUTTON.start.left,
    UserInterface::CANCEL_BUTTON.start.top,
    'A',
    "PREVMENU"
  );
  _7kaaAmbitionInterface::Draw::buttonKeybind(
    _7kaaAmbitionInterface::Input::getKeyEvent(
      _7kaaAmbitionInterface::Input::Action::Common_Cancel
    ),
    Spy::cancelButton
  );
}

void displayTownQualityOfLife(
  Town* town,
  const int refreshFlag,
  const int displayTop
) {
  // Only show for the town's owner.
  if (town->nation_recno != nation_array.player_recno) {
    return;
  }

  if (refreshFlag == INFO_REPAINT) {
    vga_util.d3_panel_up(
      UserInterface::INFO_PANE_CONTENTS.start.left,
      displayTop,
      UserInterface::INFO_PANE_CONTENTS.end.left,
      displayTop + 21
    );
  }

  const auto goodsDemand
    = town->jobless_population * PEASANT_GOODS_MONTH_DEMAND
    + town->worker_population() * WORKER_GOODS_MONTH_DEMAND;

  const auto value = format(
    "%d%%/%.1f",
    town->quality_of_life,
    goodsDemand
  );

  font_san.field(
    UserInterface::INFO_PANE_CONTENTS.start.left + 2,
    displayTop + 2,
    _("Goods supplied"),
    UserInterface::INFO_PANE_CONTENTS.start.left + 112,
    value.c_str(),
    UserInterface::INFO_PANE_CONTENTS.end.left - 2,
    refreshFlag
  );
}

void displayUnitContribution(
  ::Unit* unit,
  const int x,
  const int y
) {
   if (unit->rank_id == RANK_KING
       || unit->is_civilian()
   ) {
     return;
   }

   const auto formatString = unit->is_own_spy() ? "%'.1f" : "%'.1f/$%'d";
   String str = format(
     formatString,
     unit->nation_contribution / 2.0,
     unit->total_reward
   ).c_str();

   font_san.field(
     x,
     y,
     "_UNUSED",
     x + 92,
     str,
     UserInterface::INFO_PANE_CONTENTS.end.left - 2,
     INFO_SPECIAL
   );
}

void drawBuildingLinkLine(
  const int sourceFirmId,
  const int destinationFirmId,
  int sourceLeft,
  int sourceTop,
  int destinationLeft,
  int destinationTop,
  const bool animated,
  const bool thick
) {
  constexpr auto BI_DIRECTIONAL_CHANGE_TIME_MILLISECONDS = 1000;

  enum class Direction {
    INWARD,
    OUTWARD,
    BI_DIRECTIONAL,
  };

  Direction direction = Direction::OUTWARD;

  if (config.enhancementsAvailable()) {
    switch (sourceFirmId) {
    case FIRM_ID_TOWN:
      switch (destinationFirmId) {
      case FIRM_CAMP:
      case FIRM_MARKET:
        direction = Direction::INWARD;
        break;
      case FIRM_BASE:
      case FIRM_FACTORY:
      case FIRM_MINE:
      case FIRM_RESEARCH:
      case FIRM_WAR_FACTORY:
        direction = Direction::OUTWARD;
        break;
      case FIRM_ID_TOWN:
        direction = Direction::BI_DIRECTIONAL;
        break;
      }
      break;

    case FIRM_BASE:
    case FIRM_RESEARCH:
    case FIRM_WAR_FACTORY:
      switch (destinationFirmId) {
      case FIRM_ID_TOWN:
        direction = Direction::INWARD;
        break;
      }
      break;

    case FIRM_FACTORY:
      switch (destinationFirmId) {
      case FIRM_ID_TOWN:
      case FIRM_MINE:
        direction = Direction::INWARD;
        break;
      case FIRM_HARBOR:
      case FIRM_MARKET:
        direction = Direction::BI_DIRECTIONAL;
        break;
      }
      break;

    case FIRM_CAMP:
      switch (destinationFirmId) {
      case FIRM_ID_TOWN:
        direction = Direction::OUTWARD;
        break;
      }
      break;

    case FIRM_MINE:
      switch (destinationFirmId) {
      case FIRM_ID_TOWN:
        direction = Direction::INWARD;
        break;
      case FIRM_FACTORY:
      case FIRM_MARKET:
      case FIRM_HARBOR:
        direction = Direction::OUTWARD;
        break;
      }
      break;

    case FIRM_MARKET:
      switch (destinationFirmId) {
      case FIRM_MINE:
        direction = Direction::INWARD;
        break;
      case FIRM_ID_TOWN:
        direction = Direction::OUTWARD;
        break;
      case FIRM_FACTORY:
      case FIRM_HARBOR:
        direction = Direction::BI_DIRECTIONAL;
        break;
      }
      break;

    case FIRM_HARBOR:
      switch (destinationFirmId) {
      case FIRM_MINE:
        direction = Direction::INWARD;
        break;
      case FIRM_FACTORY:
      case FIRM_MARKET:
        direction = Direction::BI_DIRECTIONAL;
        break;
      }
      break;
    }
  }

  if (direction == Direction::INWARD
      || (direction == Direction::BI_DIRECTIONAL
          && (SDL_GetTicks64() / BI_DIRECTIONAL_CHANGE_TIME_MILLISECONDS) % 2 == 0)
  ) {
    std::swap(sourceLeft, destinationLeft);
    std::swap(sourceTop, destinationTop);
  }

  if (thick) {
    anim_line.thick_line(
      &vga_back,
      sourceLeft,
      sourceTop,
      destinationLeft,
      destinationTop,
      animated,
      -1
    );
  } else {
    anim_line.draw_line(
      &vga_back,
      sourceLeft,
      sourceTop,
      destinationLeft,
      destinationTop,
      animated
    );
  }
}

bool drawBuildingOccupantHitbar(
  const int leftX,
  const int topY,
  const int maximumWidth,
  const int currentHitpoints,
  const int maximumHitpoints
) {
  constexpr auto GREY_COLOUR = VGA_GRAY + 8;

  const auto baseColour = calculateHitbarBaseColour(maximumHitpoints) + 1;
  const auto darkColour = baseColour + 2;

  const auto width = Ambition::calculateHitbarWidth(
    maximumWidth,
    maximumHitpoints
  );
  const auto filledWidth = width * currentHitpoints / maximumHitpoints;
  /** The separating point between the filled and empty areas. */
  const auto emptyX = leftX + filledWidth;
  const auto rightX = leftX + width;

  // Top two lines filled.
  vga_front.bar(leftX, topY, emptyX, topY + 1, baseColour);
  // Clear out the empty portion.
  if (filledWidth < width) {
    vga_util.blt_buf(emptyX + 1, topY, rightX, topY + 1, 0);
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

  if (rightX + 2 < leftX + maximumWidth) {
    vga_util.blt_buf(rightX + 2, topY, leftX + maximumWidth, topY + 3, 0);
  }

  return true;
}

void drawBuildingProgressBar(
  Firm* firm
) {
  if (firm->nation_recno != nation_array.player_recno) {
    return;
  }

  if (firm->firm_id != FIRM_RESEARCH
      && firm->firm_id != FIRM_HARBOR
      && firm->firm_id != FIRM_WAR_FACTORY
  ) {
    return;
  }

  auto progress = 0.0;

  if (firm->firm_id == FIRM_RESEARCH) {
    if (!((FirmResearch*) firm)->tech_id) {
      return;
    }

    progress
      = (tech_res[((FirmResearch*) firm)->tech_id]
         ->get_progress(nation_array.player_recno))
      / 100.0;
  } else if (firm->firm_id == FIRM_HARBOR) {
    const auto harbour = (FirmHarbor*) firm;
    if (!harbour->build_unit_id) {
      return;
    }

    progress
      = static_cast<double>(sys.frame_count - harbour->start_build_frame_no)
      / (unit_res[harbour->build_unit_id]->build_days * FRAMES_PER_DAY);
  } else if (firm->firm_id == FIRM_WAR_FACTORY) {
    if (!((FirmWar*) firm)->build_unit_id) {
      return;
    }

    progress
      = ((FirmWar*) firm)->build_progress_days
      / unit_res[((FirmWar*) firm)->build_unit_id]->build_days;
  }

  const auto barWidth = ZOOM_LOC_WIDTH * (firm->loc_x2 - firm->loc_x1 + 1);
  constexpr auto BAR_HEIGHT = 3;

  const auto barLeft = firm->loc_x1 * ZOOM_LOC_WIDTH - world.view_top_x + ZOOM_X1;
  const auto barTop = firm->loc_y1 * ZOOM_LOC_HEIGHT - world.view_top_y + ZOOM_Y1 + 9;

  drawHitbar(
    firm->own_firm(),
    VGA_YELLOW,
    progress,
    barLeft,
    barTop,
    barWidth,
    BAR_HEIGHT
  );
}

void drawBuildingRallyPoint(
  Firm* _7kaaFirm
) {
  if (firm_array.selected_recno == _7kaaFirm->firm_recno
    && _7kaaFirm->own_firm()
  ) {
    const auto building = Building::findBy7kaaFirm(_7kaaFirm);
    if (building) {
      building->drawRallyPoint();
    }
  }
}
void drawBuildingRallyPoint(
  Town* _7kaaTown
) {
  if (town_array.selected_recno == _7kaaTown->town_recno
    && _7kaaTown->nation_recno == nation_array.player_recno
  ) {
    const auto building = Building::findBy7kaaTown(_7kaaTown);
    if (building) {
      building->drawRallyPoint();
    }
  }
}

void drawBuildMarkerGridLines(
  const UserInterface::Rectangle marker
) {
  int colour;
  assert(::config.terrain_set >= 1 && ::config.terrain_set <= 3);
  switch (::config.terrain_set) {
  case 1:
    colour = VGA_GRAY + 13;
    break;
  case 2:
    colour = VGA_GRAY + 6;
    break;
  case 3:
    colour = VGA_GRAY + 4;
    break;
  }

  constexpr auto PIXELS_PER_TILE = 32;

  const auto buildMarker = marker.intersection(UserInterface::VIEWPORT);
  const auto perimeterMarker
    = buildMarker.outer(PIXELS_PER_TILE)
    .intersection(UserInterface::VIEWPORT);

  vga_back.bar(
    Ambition::UserInterface::VIEWPORT.start.left,
    perimeterMarker.start.top,
    Ambition::UserInterface::VIEWPORT.end.left,
    perimeterMarker.start.top,
    colour
  );
  vga_back.bar(
    Ambition::UserInterface::VIEWPORT.start.left,
    buildMarker.start.top,
    Ambition::UserInterface::VIEWPORT.end.left,
    buildMarker.start.top,
    colour
  );
  vga_back.bar(
    Ambition::UserInterface::VIEWPORT.start.left,
    buildMarker.end.top,
    Ambition::UserInterface::VIEWPORT.end.left,
    buildMarker.end.top,
    colour
  );
  vga_back.bar(
    Ambition::UserInterface::VIEWPORT.start.left,
    perimeterMarker.end.top,
    Ambition::UserInterface::VIEWPORT.end.left,
    perimeterMarker.end.top,
    colour
  );

  vga_back.bar(
    perimeterMarker.start.left,
    Ambition::UserInterface::VIEWPORT.start.top,
    perimeterMarker.start.left,
    Ambition::UserInterface::VIEWPORT.end.top,
    colour
  );
  vga_back.bar(
    buildMarker.start.left,
    Ambition::UserInterface::VIEWPORT.start.top,
    buildMarker.start.left,
    Ambition::UserInterface::VIEWPORT.end.top,
    colour
  );
  vga_back.bar(
    buildMarker.end.left,
    Ambition::UserInterface::VIEWPORT.start.top,
    buildMarker.end.left,
    Ambition::UserInterface::VIEWPORT.end.top,
    colour
  );
  vga_back.bar(
    perimeterMarker.end.left,
    Ambition::UserInterface::VIEWPORT.start.top,
    perimeterMarker.end.left,
    Ambition::UserInterface::VIEWPORT.end.top,
    colour
  );
}

void drawBuildModeHighlighting(
  const Coordinates::Point locationCoordinates,
  const UserInterface::Point screenCoordinates,
  const Coordinates::Rectangle worldBounds
) {
  const auto firmId = power.command_para;
  const auto firmInfo = firm_res[firmId];

  const auto width
    = power.command_id == COMMAND_BUILD_FIRM
    ? firmInfo->loc_width
    : STD_TOWN_LOC_WIDTH;
  const auto height
    = power.command_id == COMMAND_BUILD_FIRM
    ? firmInfo->loc_height
    : STD_TOWN_LOC_HEIGHT;

  auto canBuild = false;
  auto ideal = false;
  if (firmId == FIRM_HARBOR) {
    const auto rectangle = Coordinates::Rectangle::fromPoint(
      locationCoordinates,
      {
        -(width - 1) * Coordinates::SCALING_FACTOR,
        (height - 1) * Coordinates::SCALING_FACTOR,
      }
    );

    for (const auto point
           : rectangle.points(Coordinates::_7KAA_COORDINATE_STEP)
    ) {
      if (!point.within(worldBounds)) {
        continue;
      }

      const auto _7kaaPoint = point.to7kaaCoordinates();
      if (world.can_build_firm(
          _7kaaPoint.x,
          _7kaaPoint.y,
          FIRM_HARBOR,
          unit_array.selected_recno
        )
      ) {
        canBuild = true;

        const auto linkCounts = Building::countLinks(
          nation_array.player_recno,
          firmId,
          Coordinates::Rectangle::fromPoint(
            point,
            {
              (width - 1) * Coordinates::SCALING_FACTOR,
              -(height - 1) * Coordinates::SCALING_FACTOR,
            }
          )
        );

        if (linkCounts[FIRM_FACTORY] > 0
          || linkCounts[FIRM_MARKET] > 0
          || linkCounts[FIRM_MINE] > 0
        ) {
          ideal = true;
          break;
        }
      }
    }
  } else {
    if (firmId == FIRM_BASE || firmId == FIRM_INN) {
      ideal = true;
    }

    const auto rectangle = Coordinates::Rectangle::fromPoint(
      locationCoordinates,
      {
        -(width - 1) * Coordinates::SCALING_FACTOR,
        (height - 1) * Coordinates::SCALING_FACTOR,
      }
    );

    for (const auto point
           : rectangle.points(Coordinates::_7KAA_COORDINATE_STEP)
    ) {
      if (!point.within(worldBounds)) {
        continue;
      }

      const auto buildRectangle = Coordinates::Rectangle::fromPoint(
        point,
        {
          (width - 1) * Coordinates::SCALING_FACTOR,
          -(height - 1) * Coordinates::SCALING_FACTOR,
        }
      );
      auto space = true;
      auto rawMaterial = false;
      for (const auto buildPoint
             : buildRectangle.points(Coordinates::_7KAA_COORDINATE_STEP)
      ) {
        if (!buildPoint.within(worldBounds)) {
          continue;
        }

        const auto location = world.get_loc(
          buildPoint.to7kaaCoordinates().x,
          buildPoint.to7kaaCoordinates().y
        );
        if (location->has_site()) {
          rawMaterial = true;
        }
        if (!(location->loc_flag & LOCATE_WALK_LAND)
          || (location->has_site() && firmId != FIRM_MINE)
          || location->is_power_off()
        ) {
          space = false;
          break;
        }
      }
      if (space) {
        canBuild = true;
        if (firmId == FIRM_MINE) {
          if (rawMaterial) {
            ideal = true;
            break;
          }
        } else {
          const auto linkCounts = Building::countLinks(
            nation_array.player_recno,
            firmId,
            Coordinates::Rectangle::fromPoint(
              point,
              {
                (width - 1) * Coordinates::SCALING_FACTOR,
                -(height - 1) * Coordinates::SCALING_FACTOR,
              }
            )
          );

          switch (firmId) {
          case FIRM_ID_TOWN:
            if (linkCounts[FIRM_BASE] == 0
              && linkCounts[FIRM_INN] == 0
              && (linkCounts[FIRM_ID_TOWN] > 0
                || linkCounts[FIRM_FACTORY] > 0
                || linkCounts[FIRM_MARKET] > 0
                || linkCounts[FIRM_CAMP] > 0
                || linkCounts[FIRM_MINE] > 0
                || linkCounts[FIRM_RESEARCH] > 0
                || linkCounts[FIRM_WAR_FACTORY] > 0
              )
            ) {
              ideal = true;
            }
            break;
          case FIRM_BASE:
            if (linkCounts[FIRM_ID_TOWN] > 0) {
              ideal = false;
            }
            break;
          case FIRM_FACTORY:
            if (linkCounts[FIRM_ID_TOWN] > 0
              || linkCounts[FIRM_MINE] > 0
              || linkCounts[FIRM_MARKET] > 0
              || linkCounts[FIRM_HARBOR] > 0
            ) {
              ideal = true;
            }
            break;
          case FIRM_INN:
            if (linkCounts[FIRM_ID_TOWN] > 0
              || linkCounts[FIRM_INN] > 0
            ) {
              ideal = false;
            }
            break;
          case FIRM_MARKET:
            if (linkCounts[FIRM_ID_TOWN] > 0
              || linkCounts[FIRM_MINE] > 0
              || linkCounts[FIRM_FACTORY] > 0
              || linkCounts[FIRM_HARBOR] > 0
            ) {
              ideal = true;
            }
            break;
          case FIRM_CAMP:
          case FIRM_RESEARCH:
          case FIRM_WAR_FACTORY:
            if (linkCounts[FIRM_ID_TOWN] > 0) {
              ideal = true;
            }
            break;
          default:
            assert(false);
          }
          if (firmId == FIRM_BASE || firmId == FIRM_INN) {
            if (!ideal) {
              break;
            }
          } else {
            if (ideal) {
              break;
            }
          }
        }
      }
    }
  }

  if (!canBuild) {
    ideal = false;
  }

  const auto _7kaaCoordinates = locationCoordinates.to7kaaCoordinates();
  int colour;
  if (ideal) {
    colour = V_WHITE;
  } else if (canBuild) {
    colour = VGA_GRAY + 8;
  } else if (!world.get_loc(_7kaaCoordinates.x, _7kaaCoordinates.y)->walkable()
  ) {
    colour = V_RED;
  } else {
    colour = V_BLACK;
  }
  vga_back.pixelize_32x32(screenCoordinates.left, screenCoordinates.top, colour);
}

void drawButtonOverlay(
  UserInterface::Rectangle buttonCoordinates,
  std::string text
) {
  const auto box
    = buttonCoordinates
    .inner(0, 3, 0, 0)
    .internal(
      {
        .width = 30,
        .height = 21,
      },
      UserInterface::HorizontalAlignment::Centre,
      UserInterface::VerticalAlignment::Centre
    );

  vga_front.bar(
    box.start.left,
    box.start.top,
    box.end.left,
    box.end.top,
    V_WHITE
  );
  vga_front.rect(
    box.start.left,
    box.start.top,
    box.end.left,
    box.end.top,
    1,
    V_BLACK
  );
  font_mid.center_put(
    box.start.left,
    box.start.top,
    box.end.left,
    box.end.top,
    text.c_str()
  );
}

void drawFeedbackLink(
) {
  UserInterface::printParagraph(
    font_news,
    std::string("_") + _("Feedback") + "_",
    UserInterface::FEEDBACK_LINK,
    2,
    UserInterface::Clear::None,
    UserInterface::HorizontalAlignment::Left,
    UserInterface::VerticalAlignment::Bottom
  );
}

void drawFirmBuilderIcon(
  Firm* firm
) {
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
      && (Time::Stamp::from7kaaTimestamp({ .gameDate = info.game_date })
          - Time::Stamp::from7kaaTimestamp({ .gameDate = firm->last_attacked_date }))
        > Time::Interval::ofDays(1)
    ) {
      iconName[7]
        = '1' + ((SDL_GetTicks64() / MILLISECONDS_PER_FRAME) % FRAME_COUNT);
    }
    const auto icon = image_icon.get_ptr(iconName);
    world.zoom_matrix->put_bitmap_clip(x, y, icon, 1);

    const auto iconSize = UserInterface::bitmapSize(icon);

    const auto textArea = UserInterface::Rectangle::fromPoint(
      {
        .left = x,
        .top = y,
      },
      {
        .width = iconSize.width,
        .height = 20,
      }
    );

    if (UserInterface::VIEWPORT.contains(textArea)) {
      const auto savedUseBackBuffer = vga.use_back_buf;
      vga.use_back();

      UserInterface::printText(
        font_news,
        format("%d", unit_array[firm->builder_recno]->skill.skill_level),
        textArea,
        UserInterface::Clear::None,
        UserInterface::HorizontalAlignment::Centre,
        UserInterface::VerticalAlignment::Top
      );

      if (!savedUseBackBuffer) {
        vga.use_front();
      }
    }
  }
}

void drawFirmFrame(
  Firm* firm,
  const int displayLayer
) {
  const auto firmBuild = firm_res.get_build(firm->firm_build_id);

  if (!firmBuild->animate_full_size) {
    firm->draw_frame(1, displayLayer);
  }
  firm->draw_frame(calculateFirmFrame(firm), displayLayer);
}

void drawFirmHitBar(
  Firm* firm
) {
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
  const auto panel = UserInterface::INFO_PANE_CONTENTS.inner(0, 50).internal(
    {
      .width = UserInterface::INFO_PANE_CONTENTS.width(),
      .height = 24,
    }
  );
  const auto field = panel.inner(4, 3);

  if (refreshFlag == INFO_REPAINT) {
    UserInterface::drawPanel(panel);
  }
  font_san.field(
    field.start.left,
    field.start.top,
    _("Guests"),
    field.start.left + 100,
    count,
    1,
    field.end.left,
    refreshFlag
  );
}

void drawLoadMenuDeleteButton(
  Button3D& button,
  const int left,
  const int top
) {
  button.paint(left, top, "DELETE", "CANCEL1D");
}

void drawMinimapLine(
  Coordinates::Point from,
  Coordinates::Point to,
  int effectFlag
) {
  const auto saveBoundLeft = anim_line.bound_x1;
  const auto saveBoundTop = anim_line.bound_y1;
  const auto saveBoundRight = anim_line.bound_x2;
  const auto saveBoundBottom = anim_line.bound_y2;
  anim_line.bound_x1 = MAP_X1;
  anim_line.bound_y1 = MAP_Y1;
  anim_line.bound_x2 = MAP_X2;
  anim_line.bound_y2 = MAP_Y2;

  const auto _7kaaFrom = from.to7kaaCoordinates();
  const auto _7kaaTo = to.to7kaaCoordinates();
  anim_line.draw_line(
    &vga_back,
    MAP_X1 + _7kaaFrom.x,
    MAP_Y1 + _7kaaFrom.y,
    MAP_X1 + _7kaaTo.x,
    MAP_Y1 + _7kaaTo.y,
    0,
    effectFlag
  );

  anim_line.bound_x1 = saveBoundLeft;
  anim_line.bound_y1 = saveBoundTop;
  anim_line.bound_x2 = saveBoundRight;
  anim_line.bound_y2 = saveBoundBottom;
}

void drawOutsideLeadershipIcon(
  const ::Unit* _7kaaUnit
) {
  const UserInterface::Point displayLocation = {
    .left = ZOOM_X1 + _7kaaUnit->cur_x - World::view_top_x + 10,
    .top = calculateUnitIconY(
      ZOOM_Y1 + _7kaaUnit->cur_y - World::view_top_y - 23
    ),
  };

  world.zoom_matrix->put_bitmap_clip(
    displayLocation.left,
    displayLocation.top,
    image_icon.get_ptr("U_NOLEAD")
  );
}

void drawTownTrainingProgressBar(
  const Town* town
) {
  if (town->nation_recno != nation_array.player_recno) {
    return;
  }

  if (!town->train_unit_recno) {
    return;
  }

  const auto progress
    = (sys.frame_count - town->start_train_frame_no)
    / static_cast<double>(TOTAL_TRAIN_DAYS * FRAMES_PER_DAY);

  const auto barWidth = ZOOM_LOC_WIDTH * (town->loc_x2 - town->loc_x1 + 1);
  constexpr auto BAR_HEIGHT = 3;

  const auto barLeft = town->loc_x1 * ZOOM_LOC_WIDTH - world.view_top_x + ZOOM_X1;
  const auto barTop = town->loc_y1 * ZOOM_LOC_HEIGHT - world.view_top_y + ZOOM_Y1;

  drawHitbar(
    town->nation_recno == nation_array.player_recno,
    VGA_YELLOW,
    progress,
    barLeft,
    barTop,
    barWidth,
    BAR_HEIGHT
  );

  const auto _7kaaUnit = unit_array[town->train_unit_recno];
  std::string iconKey = "U_";
  iconKey += _7kaaUnit->skill.skill_id
    ? Skill::skill_code_array[_7kaaUnit->skill.skill_id - 1]
    : "SPY";
  world.zoom_matrix->put_bitmap_clip(
    barLeft,
    barTop + 6,
    image_icon.get_ptr(iconKey.c_str())
  );
}

void drawWorldLine(
  Coordinates::Point from,
  Coordinates::Point to,
  int animatedFlag,
  int effectFlag
) {
  const auto screenFrom = UserInterface::fromWorldPoint(from);
  const auto screenTo = UserInterface::fromWorldPoint(to);
  anim_line.draw_line(
    &vga_back,
    screenFrom.left,
    screenFrom.top,
    screenTo.left,
    screenTo.top,
    animatedFlag,
    effectFlag
  );
}

bool initialiseSnowLayer(
  SnowLayer& layer,
  const int level,
  const char animationSpeed,
  double slideSpeed
) {
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

void printAssasinationEstimate(
  const ::Spy* _7kaaSpy,
  const Firm* _7kaaFirm,
  ::Unit* target
) {
  const auto panelArea
    = UserInterface::INFO_PANE_CONTENTS
    .inner(0, 68, 0, 0)
    .inner(4)
    .internal(
      {
        .width = UserInterface::INFO_PANE_CONTENTS.width() - 8,
        .height = 44,
      },
      UserInterface::HorizontalAlignment::Left,
      UserInterface::VerticalAlignment::Top
    );
  const auto textArea = panelArea.inner(4);

  const int estimatedChance = std::round(
    std::min(
      99.0, /* You know what happens if you tell people 100%. */
      Ambition::Spy::assassinationChanceEstimate(
        _7kaaSpy,
        _7kaaFirm,
        target
      ) * 100
    )
  );

  UserInterface::drawPanel(panelArea);
  UserInterface::printParagraph(
    font_std,
    format(
      _("Your spy estimates a success chance of %d%%."),
      estimatedChance
    ),
    textArea,
    DEFAULT_LINE_SPACE,
    UserInterface::Clear::None,
    UserInterface::HorizontalAlignment::Centre,
    UserInterface::VerticalAlignment::Centre
  );
}

void printBribeEstimate(
  const ::Spy* _7kaaSpy,
  Firm* _7kaaFirm,
  const char selectedWorkerId
) {
  const auto panelArea
    = UserInterface::INFO_PANE_CONTENTS
    .inner(4)
    .internal(
      {
        .width = UserInterface::INFO_PANE_CONTENTS.width() - 8,
        .height = 44,
      },
      UserInterface::HorizontalAlignment::Left,
      UserInterface::VerticalAlignment::Bottom
    );
  const auto textArea = panelArea.inner(4);

  const auto bribeEstimate = std::max(
    1,
    selectedWorkerId > 0
      ? Ambition::Spy::bribeAmountEstimate(
        _7kaaSpy,
        _7kaaFirm->worker_array[selectedWorkerId - 1]
      ) : Ambition::Spy::bribeAmountEstimate(
        _7kaaSpy,
        unit_array[_7kaaFirm->overseer_recno]
      )
  );

  UserInterface::drawPanel(panelArea);
  UserInterface::printParagraph(
    font_std,
    format(_("Your spy suggests a bribe of $%'d."), bribeEstimate),
    textArea,
    DEFAULT_LINE_SPACE,
    UserInterface::Clear::None,
    UserInterface::HorizontalAlignment::Centre,
    UserInterface::VerticalAlignment::Centre
  );
}

void printLeadershipStatus(
  ::Unit *_7kaaUnit,
  const int top,
  const int refreshFlag
) {
  constexpr auto PANEL_HEIGHT = 17;
  const auto PANEL = UserInterface::Rectangle {
    .start = {
      .left = UserInterface::INFO_PANE_CONTENTS.start.left,
      .top = top,
    },
    .end = {
      .left = UserInterface::INFO_PANE_CONTENTS.end.left,
      .top = top + PANEL_HEIGHT,
    },
  };
  const auto TEXT_BOX = PANEL.inner(4, 1);

  if (refreshFlag == INFO_REPAINT) {
    drawPanel(PANEL);
  }

  std::string statusText;
  if (!_7kaaUnit->leader_unit_recno) {
    statusText = _("No leader assigned");
  } else if (!Unit::isReceivingLeadershipBonus(_7kaaUnit)) {
    statusText = _("Leader out of range");
  } else {
    statusText = _("Leader is providing bonuses");
  }
  printText(font_san, statusText, TEXT_BOX, UserInterface::Clear::EntireArea);
}

void printMilitaryReportCostHeadings(
) {
  UserInterface::printText(
    font_san,
    _("Unit Cost"),
    UserInterface::MilitaryReport::UnitList::UnitCost::COLUMN.inner(0, 3),
    UserInterface::Clear::None,
    UserInterface::HorizontalAlignment::Centre
  );
  UserInterface::printText(
    font_san,
    _("Yearly Expense"),
    UserInterface::MilitaryReport::UnitList::TotalCost::COLUMN.inner(0, 3),
    UserInterface::Clear::None,
    UserInterface::HorizontalAlignment::Centre
  );
}

void printMilitaryReportCostLine(
  const UserInterface::Rectangle rowArea,
  const int unitID,
  const int unitCount
) {
  const auto viewedNation7kaaRecordNumber = info.viewing_nation_recno;

  auto yearlyUnitCost
    = unitID == -1
      ? GENERAL_YEAR_SALARY
      : unit_res[unitID]->year_cost;
  /* Adjust for the day accounting bug. */
  yearlyUnitCost = yearlyUnitCost * 11 / 10;

  /* Generals who are spies only get paid their spy salary, and do not show up
     under general expenses. */
  const auto paidUnitCount = unitID == -1
    ? Polity::getBy7kaaRecordNumber(viewedNation7kaaRecordNumber)
      ->nonSpyGeneralCount()
    : unitCount;
  const auto totalYearyCost = paidUnitCount * yearlyUnitCost;

  const auto unitCostCell = rowArea.intersection(
    Ambition::UserInterface::MilitaryReport::UnitList::UnitCost::COLUMN_CONTENTS
  );

  Ambition::UserInterface::printText(
    font_san,
    _("$"),
    unitCostCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Left
  );
  Ambition::UserInterface::printText(
    font_san,
    format("%'d", yearlyUnitCost),
    unitCostCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right
  );

  const auto totalCostValueCell = rowArea.intersection(
    Ambition::UserInterface::MilitaryReport::UnitList::TotalCost::VALUE
  );

  Ambition::UserInterface::printText(
    font_san,
    _("$"),
    totalCostValueCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Left
  );
  Ambition::UserInterface::printText(
    font_san,
    format("%'d", totalYearyCost),
    totalCostValueCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right
  );

  const auto totalCostPercentageCell = rowArea.intersection(
    Ambition::UserInterface::MilitaryReport::UnitList::TotalCost::PERCENTAGE
  );

  Ambition::UserInterface::printText(
    font_san,
    "(",
    totalCostPercentageCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Left
  );
  const auto percentage = totalYearyCost
    / nation_array[viewedNation7kaaRecordNumber]->fixed_expense_365days()
    * 100;
  Ambition::UserInterface::printText(
    font_san,
    format(
      percentage < 1 ? "%#.2g%%)" : "%#.3g%%)",
      percentage
    ),
    totalCostPercentageCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right
  );
}

void printMilitaryReportTotalCost(
  const UserInterface::Rectangle rowArea
) {
  const auto viewedNation7kaaRecordNumber = info.viewing_nation_recno;

  auto totalUnitCost
    = GENERAL_YEAR_SALARY
    * Polity::getBy7kaaRecordNumber(viewedNation7kaaRecordNumber)
      ->nonSpyGeneralCount();
  for (auto unitId = 1; unitId <= MAX_UNIT_TYPE; unitId++) {
    const auto unitInfo = unit_res[unitId];

    if (!unitInfo->race_id) {
      totalUnitCost
        += unitInfo->year_cost
        * unitInfo->nation_unit_count_array[
          viewedNation7kaaRecordNumber - 1
        ];
    }
  }
  /* Adjust for the day accounting bug. */
  totalUnitCost = totalUnitCost * 11 / 10;

  const auto totalCostCell = rowArea.intersection(
    Ambition::UserInterface::MilitaryReport::UnitList::TotalCost::VALUE
  );

  Ambition::UserInterface::printText(
    font_san,
    _("$"),
    totalCostCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Left
  );
  Ambition::UserInterface::printText(
    font_san,
    format("%'d", totalUnitCost),
    totalCostCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right
  );

  const auto typeCostPercentageCell = rowArea.intersection(
    Ambition::UserInterface::MilitaryReport::UnitList::TotalCost::PERCENTAGE
  );

  Ambition::UserInterface::printText(
    font_san,
    "(",
    typeCostPercentageCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Left
  );
  const auto percentage = totalUnitCost
    / nation_array[viewedNation7kaaRecordNumber]->fixed_expense_365days()
    * 100;
  Ambition::UserInterface::printText(
    font_san,
    format(
      percentage < 1 ? "%#.2g%%)" : "%#.3g%%)",
      percentage
    ),
    typeCostPercentageCell,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right
  );
}

void printWarMachineInBuildingInformation(
  const Worker* _7kaaWorker
) {
  constexpr UserInterface::Size FIELD_NAME_SIZE = {
    .width = 98,
    .height = 16,
  };
  constexpr UserInterface::Size FIELD_VALUE_SIZE = {
    .width = 88,
    .height = 16,
  };
  constexpr UserInterface::Size TOTAL_SIZE = {
    .width = FIELD_NAME_SIZE.width + FIELD_VALUE_SIZE.width + 4,
    .height = FIELD_NAME_SIZE.height * 2,
  };

  const auto FIELD_AREAS
    = UserInterface::INFO_PANE_CONTENTS
    .inner(4)
    .inner(0, 174, 0, 0)
    .inner(2)
    .internal(TOTAL_SIZE);

  const auto FIELD_1_NAME_AREA = FIELD_AREAS.internal(FIELD_NAME_SIZE);
  const auto FIELD_1_VALUE_AREA = FIELD_AREAS.internal(
    FIELD_VALUE_SIZE,
    UserInterface::HorizontalAlignment::Right
  );
  const auto FIELD_2_NAME_AREA
    = FIELD_AREAS
    .inner(0, FIELD_NAME_SIZE.height, 0, 0)
    .internal(FIELD_NAME_SIZE);
  const auto FIELD_2_VALUE_AREA
    = FIELD_AREAS
    .inner(0, FIELD_NAME_SIZE.height, 0, 0)
    .internal(
      FIELD_VALUE_SIZE,
      UserInterface::HorizontalAlignment::Right
    );

  if (!Unit::isWarMachine(_7kaaWorker)) {
    UserInterface::printText(
      font_san,
      _("Loyalty"),
      FIELD_1_NAME_AREA,
      UserInterface::Clear::EntireArea
    );
    UserInterface::printText(
      font_san,
      _("Leadership"),
      FIELD_2_NAME_AREA,
      UserInterface::Clear::EntireArea
    );
    return;
  }

  const auto _7kaaUnitInformation = unit_res[_7kaaWorker->unit_id];

  UserInterface::printText(
    font_san,
    _("Yearly Upkeep"),
    FIELD_1_NAME_AREA,
    UserInterface::Clear::EntireArea
  );
  UserInterface::printText(
    font_san,
    /* Adjust for the day accounting bug. */
    format("$%'.0f", _7kaaUnitInformation->year_cost * 1.1),
    FIELD_1_VALUE_AREA,
    UserInterface::Clear::EntireArea
  );

  UserInterface::printText(
    font_san,
    _("Attack Range"),
    FIELD_2_NAME_AREA,
    UserInterface::Clear::EntireArea
  );
  UserInterface::printText(
    font_san,
    format("%'d", Unit::attackRange(_7kaaWorker)),
    FIELD_2_VALUE_AREA,
    UserInterface::Clear::EntireArea
  );
}

int printWarMachineInformation(
  const ::Unit* _7kaaUnit,
  const int refreshFlag
) {
  const UserInterface::Size SIZE = {
    .width = UserInterface::INFO_PANE_CONTENTS.width(),
    .height = 42,
  };
  const auto AREA
    = UserInterface::INFO_PANE_CONTENTS
    .inner(0, 98, 0, 0)
    .internal(SIZE);
  const UserInterface::Size FIELD_SIZE = {
    .width = AREA.width(),
    .height = 16,
  };
  const UserInterface::Size NAME = {
    .width = 100,
    .height = FIELD_SIZE.height,
  };

  const auto FIELD_1_AREA
    = AREA
    .inner(4)
    .internal(FIELD_SIZE);
  const auto FIELD_2_AREA
    = AREA
    .inner(4)
    .inner(0, FIELD_SIZE.height, 0, 0)
    .internal(FIELD_SIZE);

  const auto _7kaaUnitInformation = unit_res[_7kaaUnit->unit_id];

  if (refreshFlag == INFO_REPAINT) {
    UserInterface::drawPanel(AREA);
  }

  font_san.field(
    FIELD_1_AREA.start.left,
    FIELD_1_AREA.start.top,
    _("Yearly Upkeep"),
    FIELD_1_AREA.inner(NAME.width, 0, 0, 0).start.left,
    /* Adjust for the day accounting bug. */
    _7kaaUnitInformation->year_cost * 1.1,
    2,
    FIELD_1_AREA.end.left,
    refreshFlag
  );

  font_san.field(
    FIELD_2_AREA.start.left,
    FIELD_2_AREA.start.top,
    _("Attack Range"),
    FIELD_2_AREA.inner(NAME.width, 0, 0, 0).start.left,
    Unit::attackRange(_7kaaUnit),
    1,
    FIELD_2_AREA.end.left,
    refreshFlag
  );

  return SIZE.height + 1;
}

void printStealReportsEstimate(
  const ::Spy* _7kaaSpy,
  const char report
) {
  const auto panelArea
    = UserInterface::INFO_PANE_CONTENTS
    .inner(0, 41, 0, 0)
    .inner(4)
    .internal(
      {
        .width = UserInterface::INFO_PANE_CONTENTS.width() - 8,
        .height = 44,
      },
      UserInterface::HorizontalAlignment::Left,
      UserInterface::VerticalAlignment::Top
    );
  const auto textArea = panelArea.inner(4);

  const int estimatedChance = std::round(
    std::min(
      99.0, /* You know what happens if you tell people 100%. */
      Ambition::Spy::stealReportEspaceChanceEstimate(
        _7kaaSpy,
        report
      ) * 100
    )
  );

  UserInterface::drawPanel(panelArea);
  UserInterface::printParagraph(
    font_std,
    format(
      _("Your spy estimates an escape chance of %d%%."),
      estimatedChance
    ),
    textArea,
    DEFAULT_LINE_SPACE,
    UserInterface::Clear::None,
    UserInterface::HorizontalAlignment::Centre,
    UserInterface::VerticalAlignment::Centre
  );
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
    frameSteps[i] = 1 + firmBuild->frame_delay(i + 1);
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
  const auto darkY = bottomY - (height >= 5 ? 1 : 0);

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
    drawBar(width - 1, 1, width - 1, darkY, HITBAR_DARK_BORDER_COLOUR);
  } else { // Empty.
    drawBar(width - 1, 1, width - 1, darkY, EMPTY_DARK_BORDER_COLOUR);
  }

  // Bar body filled.
  drawBar(1, 1, std::min(emptyX, width - 2), darkY, HITBAR_BODY_COLOUR);
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
  const auto pitch = width + 2;

  auto dataPtr = sys.common_data_buf;

  *((short*)dataPtr) = pitch;
  *(((short*)dataPtr) + 1) = height + 2;

  dataPtr += sizeof(short) * 2;

  const auto outlineColour = isOwn ? VGA_LIGHT_GREEN : VGA_RED;
  const auto shadowColour = outlineColour + 3;

  IMGbar(dataPtr, pitch, 1, 1, width, height, 0xFF);
  IMGbar(dataPtr, pitch, 0, 0, width + 1, 0, outlineColour);
  IMGbar(dataPtr, pitch, 0, 0, 0, height, outlineColour);
  IMGbar(dataPtr, pitch, 0, height + 1, width + 1, height + 1, shadowColour);
  IMGbar(dataPtr, pitch, width + 1, 1, width + 1, height + 1, shadowColour);

  world.zoom_matrix->put_bitmap_clip(x - 1, y - 1, sys.common_data_buf);
}

} // namespace Ambition
