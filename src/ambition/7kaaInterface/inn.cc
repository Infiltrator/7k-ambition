/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2025–26 Tim Sviridov
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
 * Implementation file for _7kaaAmbitionInterface::Inn.
 */

#define _AMBITION_IMPLEMENTATION
#include "inn.hh"

#include <cassert>

#include "pragma_silence_7kaa_warnings.hh"
#include "gettext.h"
#include "OBUTT3D.h"
#include "OF_INN.h"
#include "OMOUSE.h"
#include "ONATIONA.h"
#include "OVBROWIF.h"
#pragma GCC diagnostic pop

#include "Ambition_config.hh"
#include "Ambition_inn.hh"


namespace _7kaaAmbitionInterface::Inn {

bool guestSelectionValid = false;


std::string crowdingMessage(
  const Firm* _7kaaFirm
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return "";
  }

  if (_7kaaFirm->nation_recno != nation_array.player_recno
    || _7kaaFirm->firm_id != FIRM_INN
  ) {
    return "";
  }

  const auto _7kaaInn = dynamic_cast<const FirmInn*>(_7kaaFirm);
  assert(_7kaaInn);
  if (Ambition::Inn::competingInnCount(_7kaaInn)) {
    /* TRANSLATORS: affix to Inn's name to indicate that this Inn is too close
     * to another Inn, and so will have fewer mercenaries. */
    return std::string(" ") + _("(too close)");
  }

  return "";
}

void detectBrowserClick(
  FirmInn* inn,
  const VBrowseIF& guestBrowser
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (mouse.any_click(
      guestBrowser.ix1,
      guestBrowser.iy1,
      guestBrowser.ix2,
      guestBrowser.iy2
    )
  ) {
    const auto recordNumber = getSelectedRecordNumber(
      inn,
      (guestBrowser.top_rec_no
        + ((mouse.click_y() - guestBrowser.iy1)
           / (guestBrowser.rec_height + guestBrowser.rec_y_space)
           * guestBrowser.x_max_rec))
    );
    guestSelected(inn, recordNumber);
  }
}

int getSelectedRecordNumber(
  const FirmInn* inn,
  const int browserRecordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return browserRecordNumber;
  }

  return Ambition::getInnSelectedRecordNumber(inn, browserRecordNumber);
}

void guestLeaving(
  const FirmInn* inn,
  const VBrowseIF& guestBrowser,
  const int recordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (inn->firm_recno != firm_array.selected_recno || !inn->should_show_info()) {
    return;
  }

  if (Ambition::getInnSelectedRecordNumber(inn, guestBrowser.rec_no)
      == recordNumber
  ) {
    guestSelectionValid = false;
  }
}

void guestSelected(
  FirmInn* inn,
  const int recordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (inn->firm_recno != firm_array.selected_recno || !inn->should_show_info()) {
    return;
  }

  guestSelectionValid = true;
}

void overrideHireButtonStatus(
  Button3D& hireButton
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (!guestSelectionValid) {
    hireButton.disable();
  }
}

bool refreshBrowser(
  const short firmRecordNumber,
  FirmInn* inn,
  VBrowseIF& browser,
  const int removedUnitIndex,
  Button3D& hireButton
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  Ambition::refreshInnBrowser(
    firmRecordNumber,
    inn,
    browser,
    removedUnitIndex,
    hireButton
  );

  return true;
}

} // namespace _7kaaAmbitionInterface::Inn
