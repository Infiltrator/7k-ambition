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
 * Implementation file for _7kaaAmbitionInterface::Draw.
 */

#define _AMBITION_IMPLEMENTATION
#include "draw.hh"

#include "OBUTT3D.h"
#include "OBUTTCUS.h"
#include "OCONFIG.h"
#include "OF_HARB.h"
#include "OF_WAR.h"
#include "OFIRM.h"
#include "OFONT.h"
#include "OIMGRES.h"
#include "ONATIONA.h"
#include "OPOWER.h"
#include "OTOWN.h"
#include "OUNIT.h"
#include "vga_util.h"

#include "Ambition_building.hh"
#include "Ambition_config.hh"
#include "Ambition_minimap.hh"
#include "Ambition_remote.hh"
#include "Ambition_trade.hh"
#include "Ambition_unit.hh"
#include "Ambition_user_interface.hh"
#include "Ambition_vga.hh"
#include "format.hh"


namespace _7kaaAmbitionInterface::Draw {

void buildingAnimationFrame(
  Firm* firm,
  const int displayLayer
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawFirmFrame(firm, displayLayer);
}

void buildingBuilderIcon(
  Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawFirmBuilderIcon(firm);
}

void buildingHitBar(
  Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawFirmHitBar(firm);
}

void buildingLinkLine(
  const int sourceFirmId,
  const int destinationFirmId,
  int sourceLeft,
  int sourceTop,
  int destinationLeft,
  int destinationTop,
  const bool animated,
  const bool thick
) {
  Ambition::drawBuildingLinkLine(
    sourceFirmId,
    destinationFirmId,
    sourceLeft,
    sourceTop,
    destinationLeft,
    destinationTop,
    animated,
    thick
  );
}

bool buildingOccupantHitbar(
  const int left,
  const int top,
  const int maximumWidth,
  const int currentHitpoints,
  const int maximumHitpoints
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::drawBuildingOccupantHitbar(
    left,
    top,
    maximumWidth,
    currentHitpoints,
    maximumHitpoints
  );
}

void buildingProgressBar(
  Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawBuildingProgressBar(firm);
}

void buildingProgressBar(
  const Town* town
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawTownTrainingProgressBar(town);
}

void buildingRallyButton(
  Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (_7kaaFirm->own_firm()) {
    const auto building = Ambition::Building::findBy7kaaFirm(_7kaaFirm);
    if (building) {
      building->drawRallyButton();
    }
  }
}
void buildingRallyButton(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (_7kaaTown->nation_recno == nation_array.player_recno) {
    const auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);
    if (building) {
      building->drawRallyButton();
    }
  }
}

void buildingRallyPoint(
  Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawBuildingRallyPoint(_7kaaFirm);
}
void buildingRallyPoint(
  Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawBuildingRallyPoint(_7kaaTown);
}

void buildMarkerGridLines(
  const int left,
  const int top,
  const int right,
  const int bottom
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto buildMarker = Ambition::UserInterface::Rectangle {
    .start = {
      .left = left,
      .top = top,
    },
    .end = {
      .left = right,
      .top = bottom,
    },
  };

  Ambition::drawBuildMarkerGridLines(buildMarker);
}

void buildModeHighlighting(
  const short _7kaaX,
  const short _7kaaY,
  const int left,
  const int top,
  const short maximum7kaaX,
  const short maximum7kaaY
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (power.command_id != COMMAND_BUILD_FIRM
    && power.command_id != COMMAND_SETTLE
  ) {
    return;
  }

  using Ambition::Coordinates::Point;

  Ambition::drawBuildModeHighlighting(
    Point::from7kaaCoordinates({ _7kaaX, _7kaaY }),
    { left, top },
    Ambition::Coordinates::Rectangle::fromPoint(
      Point::from7kaaCoordinates({ 0, 0 }),
      Point::from7kaaCoordinates(
        {
          static_cast<short>(maximum7kaaX - 1),
          static_cast<short>(maximum7kaaY - 1),
        }
      ) - Point::from7kaaCoordinates({ 0, 0 })
    )
  );
}

void buttonCost(
  Font& font,
  const int cost,
  const int left,
  const int top,
  const int right,
  const int bottom,
  const int verticalAlignment
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::UserInterface::VerticalAlignment alignment;
  if (verticalAlignment < 0) {
    alignment = Ambition::UserInterface::VerticalAlignment::Top;
  } else if (verticalAlignment == 0) {
    alignment = Ambition::UserInterface::VerticalAlignment::Centre;
  } else {
    alignment = Ambition::UserInterface::VerticalAlignment::Bottom;
  }

  printText(
    font,
    format("$%'d", cost),
    {
      .start = {
        .left = left,
        .top = top,
      },
      .end = {
        .left = right,
        .top = bottom,
      },
    },
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right,
    alignment
  );
}

void buttonKeybind(
  const unsigned int keyCode,
  const int left,
  const int top,
  const int right,
  const int bottom,
  const int refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (!keyCode) {
    return;
  }

  const Ambition::UserInterface::Rectangle area = {
    .start = {
      .left = left,
      .top = top,
    },
    .end = {
      .left = right,
      .top = bottom,
    },
  };

  const auto keyString = Ambition::UserInterface::keyString(keyCode);
  const auto textWidth = std::max(
    font_san.text_width("M"),
    font_san.text_width(keyString.c_str())
  ) + 1;
  const auto textHeight = font_san.text_height();

  constexpr auto PADDING = 3;
  const auto panel = area.inner(2).internal(
    {
      .width = textWidth + PADDING * 2,
      .height = textHeight + PADDING * 2,
    },
    Ambition::UserInterface::HorizontalAlignment::Left,
    Ambition::UserInterface::VerticalAlignment::Bottom
  );
  const auto textArea = panel.inner(PADDING);

  if (refreshFlag == INFO_REPAINT) {
    vga_util.d3_panel_up(
      panel.start.left,
      panel.start.top,
      panel.end.left,
      panel.end.top,
      1
    );

    Ambition::UserInterface::printText(
      font_san,
      keyString,
      textArea,
      Ambition::UserInterface::Clear::None,
      Ambition::UserInterface::HorizontalAlignment::Centre
    );

    Ambition::copyFrontBufferToBack(panel);
  } else {
    vga_util.blt_buf(
      panel.start.left,
      panel.start.top,
      panel.end.left,
      panel.end.top,
      0
    );
  }
}
void buttonKeybind(
  const unsigned int keyCode,
  const Button& button
) {
  constexpr auto BOTTOM_PADDING = 1;

  buttonKeybind(
    keyCode,
    button.x1,
    button.y1,
    button.x2,
    button.y2 - BOTTOM_PADDING
  );
}
void buttonKeybind(
  const unsigned int keyCode,
  const Button3D& button,
  const int refreshFlag
) {
  /* 7kaa buttons have extra pixels around. */
  constexpr auto LEFT_MARGIN = 0;
  constexpr auto BOTTOM_MARGIN = 8;

  constexpr auto LEFT_PADDING = 1;
  constexpr auto BOTTOM_PADDING = 0;

  buttonKeybind(
    keyCode,
    button.x1 + LEFT_MARGIN + LEFT_PADDING,
    button.y1,
    button.x2,
    button.y2 - BOTTOM_MARGIN - BOTTOM_PADDING,
    refreshFlag
  );
}
void buttonKeybind(
  const unsigned int keyCode,
  const ButtonCustom& button
) {
  constexpr auto LEFT_PADDING = 2;
  constexpr auto BOTTOM_PADDING = 2;

  buttonKeybind(
    keyCode,
    button.x1 + LEFT_PADDING,
    button.y1,
    button.x2,
    button.y2 - BOTTOM_PADDING
  );
}

short calculateAnimatedLinePhase(
  const short _7kaaCalculation,
  const int animatedFlag,
  const int lineProgress
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateAnimatedLinePhase(
    animatedFlag,
    lineProgress
  );
}

int calculateBuildMarkerColour(
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  if (_7kaaCalculation == V_BLACK) {
    return _7kaaCalculation;
  }

  constexpr auto COLOUR_COUNT = 6;
  constexpr unsigned char COLOURS[COLOUR_COUNT] = {
    0xA4,
    0xA5,
    0xA6,
    0x98,
    0xA6,
    0xA5,
  };
  constexpr auto FRAME_RATE = 8;
  constexpr auto MILLISECONDS_PER_COLOUR = 1000 / FRAME_RATE;

  return COLOURS[(SDL_GetTicks64() / MILLISECONDS_PER_COLOUR) % COLOUR_COUNT];
}

int calculateDoneButtonWidth(
  const int _7kaaCalculation,
  const Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  // The harbour UI is a different size to the War Factory (for now).
  if (dynamic_cast<const FirmHarbor*>(firm)) {
    return 84;
  }

  return 80;
}

FirmBitmap* calculateFirmBitmap(
  FirmBitmap* _7kaaCalculation,
  Firm* firm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateFirmBitmap(_7kaaCalculation, firm);
}

int calculateHitbarBaseColour(
  const int _7kaaCalculation,
  const double maximumHitpoints
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateHitbarBaseColour(maximumHitpoints);
}

int calculateHitbarWidth(
  const int availableWidth,
  const double maximumHitpoints
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return availableWidth;
  }

  return Ambition::calculateHitbarWidth(availableWidth, maximumHitpoints);
}

short calculateRainSpeed(
  const short _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateRainSpeed(_7kaaCalculation);
}

char calculateRockRemainingDelay(
  const char _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateRockRemainingDelay(_7kaaCalculation);
}

char* calculateTerrainBitmap(
  char* _7kaaCalculation,
  const short terrainId,
  const int x,
  const int y
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateTerrainBitmap(terrainId, x, y);
}

char calculateTownFlagNumber(
  const char _7kaaCalculation,
  const int townRecordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateTownFlagNumber(townRecordNumber);
}

int calculateUnitHitbarWidth(
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateUnitHitbarWidth();
}

int calculateUnitIconY(
  const int _7kaaCalculation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateUnitIconY(_7kaaCalculation);
}

int calculateWorkerPortraitLeft(
  const int _7kaaCalculation,
  const int workerIndex
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return _7kaaCalculation;
  }

  return Ambition::calculateWorkerPortraitX(workerIndex);
}

void caravanCloneButton(
  const int refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (refreshFlag != INFO_REPAINT) {
    return;
  }

  Ambition::Trade::drawCaravanCloneButton();
}

int centreHitbar(
  const int left,
  const int maximumWidth,
  const int currentWidth
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return left;
  }

  return Ambition::centreHitbar(left, maximumWidth, currentWidth);
}

void discordButton(
) {
  const auto buttonBitmap = image_button.get_ptr("DISCORD");

  vga_front.put_bitmap_trans(
    Ambition::UserInterface::DISCORD_BUTTON.start.left,
    Ambition::UserInterface::DISCORD_BUTTON.start.top,
    buttonBitmap
  );
}

void doneButtonText(
  const ButtonCustom& button
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const Ambition::UserInterface::Rectangle buttonArea = {
    .start = {
      .left = button.x1,
      .top = button.y1,
    },
    .end = {
      .left = button.x2,
      .top = button.y2,
    },
  };
  Ambition::UserInterface::printText(
    font_bible,
    "Done",
    buttonArea.inner(7, 0),
    Ambition::UserInterface::Clear::EntireArea,
    Ambition::UserInterface::HorizontalAlignment::Right,
    Ambition::UserInterface::VerticalAlignment::Centre
  );
}

void feedbackLink(
) {
  Ambition::drawFeedbackLink();
}

void hitbarOutline(
  const bool isOwn,
  const int x,
  const int y,
  const int width,
  const int height
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawHitbarOutline(isOwn, x, y, width, height);
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

  return Ambition::initialiseSnowLayer(layer, level, animationSpeed, slideSpeed);
}

void innGuestCount(
  const short count,
  const int refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawInnGuestCount(count, refreshFlag);
}

void innGuestDetailsLeavingInformation(
  const InnUnit* _7kaaInnGuest,
  const int refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::displayInnGuestDetailsStayingInformation(_7kaaInnGuest, refreshFlag);
}

void innGuestLeavingSoonMark(
  const InnUnit* _7kaaInnGuest,
  const int top
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::displayInnGuestLeavingSoonMark(_7kaaInnGuest, top);
}

void loadMenuDeleteButton(
  Button3D& button,
  const int left,
  const int top
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::drawLoadMenuDeleteButton(button, left, top);
}

void militaryReportCostHeadings(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::printMilitaryReportCostHeadings();
}

void militaryReportCostLine(
  const int top,
  const int rowHeight,
  const int unitID,
  const int unitCount
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto rowArea = Ambition::UserInterface::Rectangle {
    .start = {
      .left = Ambition::UserInterface::REPORT_AREA.start.left,
      .top = top,
    },
    .end = {
      .left = Ambition::UserInterface::REPORT_AREA.end.left,
      .top = top + rowHeight,
    },
  };

  Ambition::printMilitaryReportCostLine(rowArea, unitID, unitCount);
}

void militaryReportTotalCost(
  const int top,
  const int rowHeight
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto rowArea = Ambition::UserInterface::Rectangle {
    .start = {
      .left = Ambition::UserInterface::REPORT_AREA.start.left,
      .top = top,
    },
    .end = {
      .left = Ambition::UserInterface::REPORT_AREA.end.left,
      .top = top + rowHeight,
    },
  };

  Ambition::printMilitaryReportTotalCost(rowArea);
}

bool minimapViewBoundsBox(
  const MapMatrix* _7kaaMapMatrix
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  return Ambition::Minimap::drawViewBoundsBox(_7kaaMapMatrix);
}

void modeInformation(
) {
  Ambition::drawModeInformation();
}

void outsideLeadershipIcon(
  Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (!_7kaaUnit->is_own() && !config.show_ai_info) {
    return;
  }

  if (Ambition::Unit::canReceiveLeadershipBonus(_7kaaUnit)
      && !Ambition::Unit::isReceivingLeadershipBonus(_7kaaUnit)
  ) {
    Ambition::drawOutsideLeadershipIcon(_7kaaUnit);
  }
}

void printBribeEstimate(
  const ::Spy* _7kaaSpy,
  Firm* _7kaaFirm,
  const char selectedWorkerId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::printBribeEstimate(_7kaaSpy, _7kaaFirm, selectedWorkerId);
}

void printGameSpeed(
  const int speed
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::displayGameSpeed(speed);
}

bool printLeadershipStatus(
  Unit* _7kaaUnit,
  const int top,
  const int refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  if (!_7kaaUnit->is_own() && !config.show_ai_info) {
    return false;
  }

  if (!Ambition::Unit::canReceiveLeadershipBonus(_7kaaUnit)) {
    return false;
  }

  if (_7kaaUnit->is_own_spy()) {
    return false;
  }

  Ambition::printLeadershipStatus(_7kaaUnit, top, refreshFlag);
  return true;
}


bool printSaveFileDate(
  const std::string string,
  const int left,
  const int top
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  const auto slotArea = Ambition::UserInterface::Rectangle::fromPoint(
    {
      .left = left - 60,
      .top = top
    },
    Ambition::UserInterface::SAVE_SLOT_SIZE
  );
  const auto slotContentsArea = slotArea.inner(10);

  const auto textArea = slotContentsArea.internal(
    Ambition::UserInterface::SAVE_INFORMATION_TEXT_AREA_SIZE,
    Ambition::UserInterface::HorizontalAlignment::Right,
    Ambition::UserInterface::VerticalAlignment::Bottom
  );

  Ambition::UserInterface::printText(
    font_small,
    string,
    textArea,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right
  );
  return true;
}

bool printSaveFilename(
  const std::string string,
  const int left,
  const int top
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  const auto slotArea = Ambition::UserInterface::Rectangle::fromPoint(
    {
      .left = left - 60,
      .top = top
    },
    Ambition::UserInterface::SAVE_SLOT_SIZE
  );
  const auto slotContentsArea = slotArea.inner(10);

  const auto textArea
    = slotContentsArea
    .inner(0, 0, 0, Ambition::UserInterface::SAVE_SLOT_SIZE.height)
    .internal(
      Ambition::UserInterface::SAVE_INFORMATION_TEXT_AREA_SIZE,
      Ambition::UserInterface::HorizontalAlignment::Right,
      Ambition::UserInterface::VerticalAlignment::Bottom
    );

  Ambition::UserInterface::printText(
    font_small,
    string,
    textArea,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right
  );
  return true;
}

void printSyncError(
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::printSyncError();
}

void printTownQualityOfLife(
  Town* town,
  const int refreshFlag,
  const int displayTop
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::displayTownQualityOfLife(town, refreshFlag, displayTop);
}

void printUnitContribution(
  ::Unit* unit,
  const int left,
  const int top
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::displayUnitContribution(unit, left, top);
}

void printWarMachineInBuildingInformation(
  const Worker* _7kaaWorker
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::printWarMachineInBuildingInformation(_7kaaWorker);
}

int printWarMachineInformation(
  const ::Unit* _7kaaUnit,
  const int refreshFlag
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return 0;
  }

  if (!Ambition::Unit::isWarMachine(_7kaaUnit)) {
    return 0;
  }

  return Ambition::printWarMachineInformation(_7kaaUnit, refreshFlag);
}

void queueCount(
  const FirmHarbor* _7kaaHarbour
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto building = Ambition::Building::findBy7kaaFirm(_7kaaHarbour);

  const auto productionCount
    = (building ? building->enqueuedProductionCount(-1) : 0)
    + (_7kaaHarbour->build_unit_id ? 1 : 0)
    + _7kaaHarbour->build_queue_count;

  if (!productionCount) {
    return;
  }

  Ambition::drawButtonOverlay(
    Ambition::UserInterface::Rectangle::fromPoint(
      {
        .left = 585,
        .top = 520,
      },
      Ambition::UserInterface::BUTTON_SIZE
    ),
    format("%'d", productionCount)
  );
}
void queueCount(
  const FirmWar* _7kaaWarFactory
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto building = Ambition::Building::findBy7kaaFirm(_7kaaWarFactory);

  const auto productionCount
    = (building ? building->enqueuedProductionCount(-1) : 0)
    + (_7kaaWarFactory->build_unit_id ? 1 : 0)
    + _7kaaWarFactory->build_queue_count;

  if (!productionCount) {
    return;
  }

  Ambition::drawButtonOverlay(
    Ambition::UserInterface::Rectangle::fromPoint(
      {
        .left = 585,
        .top = 500,
      },
      Ambition::UserInterface::BUTTON_SIZE
    ),
    format("%'d", productionCount)
  );
}
void queueCount(
  const Town* _7kaaTown
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto building = Ambition::Building::findBy7kaaTown(_7kaaTown);

  const auto productionCount
    = (building ? building->enqueuedProductionCount(-1) : 0)
    + (_7kaaTown->train_unit_recno ? 1 : 0)
    + _7kaaTown->train_queue_count;

  if (!productionCount) {
    return;
  }

  Ambition::drawButtonOverlay(
    Ambition::UserInterface::Rectangle::fromPoint(
      {
        .left = 637,
        .top = 471,
      },
      Ambition::UserInterface::BUTTON_SIZE
    ),
    format("%'d", productionCount)
  );
}

void tradeReportcaravanCloneButton(
  VBrowseIF& caravanBrowser
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::Trade::drawReportCaravanCloneButton(caravanBrowser);
}

void trainingSkillIcon(
  const char _7kaaSkillId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  constexpr Ambition::UserInterface::Size TRAINING_ICON_SIZE = {
    .width = 24,
    .height = 20,
  };

  const auto TRAINING_PANEL_AREA = Ambition::UserInterface::INFO_PANE_CONTENTS.internal(
    {
      .width = Ambition::UserInterface::INFO_PANE_CONTENTS.width(),
      .height = 30,
    },
    Ambition::UserInterface::HorizontalAlignment::Centre,
    Ambition::UserInterface::VerticalAlignment::Bottom
  );
  const auto TRAINING_ICON_PANEL_AREA = TRAINING_PANEL_AREA.inner(4, 4, 4, 2);
  const auto TRAINING_ICON_AREA
    = TRAINING_ICON_PANEL_AREA
    .inner(2, 2, 2, 1)
    .internal(TRAINING_ICON_SIZE);

  std::string iconKey = "U_";
  iconKey += _7kaaSkillId ? Skill::skill_code_array[_7kaaSkillId - 1] : "SPY";
  const auto bitmap = image_icon.get_ptr(iconKey.c_str());

  const auto iconSize = Ambition::UserInterface::bitmapSize(bitmap);
  const auto iconArea
    = TRAINING_ICON_AREA
    .inner(2)
    .internal(
      iconSize,
      Ambition::UserInterface::HorizontalAlignment::Right,
      Ambition::UserInterface::VerticalAlignment::Top
    );

  vga_front.put_bitmap(iconArea.start.left, iconArea.start.top, bitmap);
}

void unitWaypointsOnWorld(
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->drawWaypointsOnWorld();
  }
}

void unitWaypointsOnMinimap(
  const ::Unit* _7kaaUnit
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto unit
    = Ambition::Unit::findBy7kaaSpriteRecordNumber(_7kaaUnit->sprite_recno);
  if (unit) {
    unit->drawWaypointsOnMinimap();
  }
}

void warMachineTechnologyLevel(
  int* techLevel,
  const int left,
  const int top,
  const char* portraitBitmap
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto portraitArea = Ambition::UserInterface::Rectangle::fromPoint(
    {
      .left = left,
      .top = top,
    },
    Ambition::UserInterface::bitmapSize(portraitBitmap)
    // The bible font seems to have probelms with its sizing, with its height
    // being 4 too much, and its width being at least 2 too little.
  ).outer(0, 4).inner(2, 0);

  Ambition::UserInterface::printText(
    font_bible,
    misc.roman_number(*techLevel),
    portraitArea.inner(2),
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Right,
    Ambition::UserInterface::VerticalAlignment::Top
  );

  *techLevel = -1;
}

void whatsNewButton(
) {
  Button3D button;
  button.paint(770, 550, "REPAIRU", "REPAIRD");
}

void workerSpyIcon(
  const char** spyIconName,
  const int left,
  const int top,
  const char* portraitBitmap
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (!*spyIconName) {
    return;
  }

  const auto iconBitmap = image_icon.get_ptr(*spyIconName);
  const auto portraitArea = Ambition::UserInterface::Rectangle::fromPoint(
    {
      .left = left,
      .top = top,
    },
    Ambition::UserInterface::bitmapSize(portraitBitmap)
  );
  const auto iconArea = portraitArea.inner(2).internal(
    Ambition::UserInterface::bitmapSize(iconBitmap),
    Ambition::UserInterface::HorizontalAlignment::Right,
    Ambition::UserInterface::VerticalAlignment::Top
  );

  vga_front.put_bitmap(iconArea.start.left, iconArea.start.top, iconBitmap);
  *spyIconName = nullptr;
}

} // namespace _7kaaAmbitionInterface::Draw
