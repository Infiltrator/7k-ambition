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
 * Implementation file for Ambition::Trade.
 */

#include "Ambition_trade.hh"

#include <cstdint>

#define _AMBITION_IMPLEMENTATION
#include "OBUTT3D.h"
#include "OF_FACT.h"
#include "OF_MINE.h"
#include "OMOUSE.h"
#include "ONATIONA.h"
#include "OU_CARA.h"
#include "OVBROWIF.h"
#include "vga_util.h"

#include "7kaaInterface/draw.hh"
#include "Ambition_polity.hh"
#include "Ambition_user_interface.hh"


namespace Ambition::Trade {

Button3D caravanCloneButton;
Button reportCloneButton;


void checkCaravanForReplacement(
  UnitCaravan* _7kaaCaravan
) {
  if (_7kaaCaravan->hit_points == _7kaaCaravan->max_hit_points) {
    return;
  }

  /**
   * The hit point threshold for a caravan to be replaced if it has no Market
   * stops.
   *
   * Since the new caravan would need to travel some distance from the nearest
   * Market to the first stop, we do not want to do this if the caravan has
   * taken only a small amount of damage.
   */
  constexpr auto NO_MARKET_STOPS_HIT_POINT_REPLACEMENT_THRESHOLD = 0.8;

  auto hasMarketStops = false;
  for (auto i = 0; i < MAX_STOP_FOR_CARAVAN; i++) {
    const auto& tradeStop = _7kaaCaravan->stop_array[i];
    if (tradeStop.firm_id == FIRM_MARKET) {
      hasMarketStops = true;
      break;
    }
  }

  if (hasMarketStops) {
    const auto& currentDestination
      = _7kaaCaravan->stop_array[_7kaaCaravan->dest_stop_id];
    if (currentDestination.firm_id != FIRM_MARKET) {
      return;
    }
  } else {
    if ((_7kaaCaravan->hit_points / _7kaaCaravan->max_hit_points)
        > NO_MARKET_STOPS_HIT_POINT_REPLACEMENT_THRESHOLD
    ) {
      return;
    }
  }

  const auto polity = Polity::getBy7kaaRecordNumber(_7kaaCaravan->nation_recno);
  polity->replaceCaravan(_7kaaCaravan);
}

void detectCaravanCloneButton(
  const UnitCaravan* _7kaaCaravan
) {
  if (!caravanCloneButton.detect(GETKEY(KEYEVENT_FIRM_PATROL))) {
    return;
  }

  const auto polity = Polity::getBy7kaaRecordNumber(_7kaaCaravan->nation_recno);
  polity->cloneCaravan(_7kaaCaravan);
}

bool detectReportCaravanCloneButton(
  const UnitCaravan* _7kaaCaravan
) {
  if (!reportCloneButton.detect()) {
    return false;
  }

  const auto polity = Polity::getBy7kaaRecordNumber(_7kaaCaravan->nation_recno);
  polity->cloneCaravan(_7kaaCaravan);
  return true;
}

void drawCaravanCloneButton(
) {
  caravanCloneButton.paint(
    UserInterface::BUTTON_ROW_LOWER.start.left,
    UserInterface::BUTTON_ROW_LOWER.start.top,
    'A',
    "HIRECARA"
  );

  _7kaaAmbitionInterface::Draw::buttonKeybind(
    GETKEY(KEYEVENT_FIRM_PATROL),
    caravanCloneButton
  );

  const auto polity = Polity::getBy7kaaRecordNumber(nation_array.player_recno);
  if (!polity->idleCaravans().empty() || polity->canHireCaravan()) {
    caravanCloneButton.enable();
  } else {
    caravanCloneButton.disable();
  }

  /* Remove the extra lines that the button bitmap has at the bottom. */
  vga_util.blt_buf(
    Ambition::UserInterface::BUTTON_ROW_LOWER.start.left,
    Ambition::UserInterface::BUTTON_ROW_LOWER.end.top + 1,
    Ambition::UserInterface::BUTTON_ROW_LOWER.end.left,
    Ambition::UserInterface::BUTTON_ROW_LOWER.end.top + 2
  );
}

void drawReportCaravanCloneButton(
  VBrowseIF& caravanBrowser
) {
  int x1, y1, x2, y2;
  if (caravanBrowser.mouse_over(&x1, &y1, &x2, &y2)) {
    reportCloneButton.paint_text(
      x2 - 42,
      y1 - 4,
      "C",
      1,
      reportCloneButton.button_wait > 0
    );
    reportCloneButton.enable_flag = 1;
  } else {
    reportCloneButton.enable_flag = 0;
  }
}

bool isCaravanIdle(
  UnitCaravan* _7kaaCaravan
) {
  auto dropOffCount = 0;
  auto pickUpCount = 0;

  for (auto i = 0; i < MAX_STOP_FOR_CARAVAN; i++) {
    const auto& tradeStop = _7kaaCaravan->stop_array[i];

    if (!tradeStop.firm_recno || firm_array.is_deleted(tradeStop.firm_recno)) {
      continue;
    }

    const auto _7kaaFirm = firm_array[tradeStop.firm_recno];

    if (_7kaaFirm->firm_id == FIRM_MARKET) {
      dropOffCount++;

      if (tradeStop.pick_up_type == NO_PICK_UP) {
        continue;
      }

      const auto _7kaaMarket = dynamic_cast<FirmMarket*>(_7kaaFirm);
      for (auto j = 0; j < MAX_MARKET_GOODS; j++) {
        auto marketGoods = _7kaaMarket->market_goods_array[j];
        if (!marketGoods.stock_qty && !marketGoods.sales_365days()) {
          continue;
        }

        if (marketGoods.raw_id
          && (tradeStop.pick_up_array[marketGoods.raw_id - 1]
            || tradeStop.pick_up_type == AUTO_PICK_UP)
        ) {
          pickUpCount++;
          break;
        } else if (marketGoods.product_raw_id
          && (tradeStop.pick_up_array[marketGoods.product_raw_id - 1 + MAX_RAW]
            || tradeStop.pick_up_type==AUTO_PICK_UP)
        ) {
          pickUpCount++;
          break;
        }
      }
    } else if (_7kaaFirm->firm_id == FIRM_FACTORY) {
      const auto firmFactory = _7kaaFirm->cast_to_FirmFactory();

      if (_7kaaCaravan->carrying_qty(firmFactory->product_raw_id) > 0) {
        dropOffCount++;
      }

      if (tradeStop.pick_up_type == NO_PICK_UP) {
        continue;
      }

      if (!tradeStop.pick_up_array[firmFactory->product_raw_id - 1 + MAX_RAW]
        || (!firmFactory->stock_qty
          && !firmFactory->production_30days())
      ) {
        continue;
      }

      pickUpCount++;
    } else if (_7kaaFirm->firm_id == FIRM_MINE) {
      if (tradeStop.pick_up_type == NO_PICK_UP) {
        continue;
      }

      const auto firmMine = _7kaaFirm->cast_to_FirmMine();

      if (!tradeStop.pick_up_array[firmMine->raw_id - 1]
        || (!firmMine->stock_qty
          && !firmMine->production_30days())
      ) {
        continue;
      }

      pickUpCount++;
    }
  }

  auto carriedGoodCount = 0;
  for (int i = PICK_UP_RAW_FIRST; i < MAX_PICK_UP_GOODS; i++) {
    carriedGoodCount += _7kaaCaravan->carrying_qty(i);
  }

  if (pickUpCount == 0) {
    if (carriedGoodCount > 0) {
      return false;
    }
    if (dropOffCount == 0) {
      return true;
    }
  }
  if (_7kaaCaravan->stop_defined_num == 1
    && carriedGoodCount == 0) {
    return true;
  }
  return false;
}

} // namespace Ambition::Trade
