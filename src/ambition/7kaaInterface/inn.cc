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
 * Implementation file for _7kaaAmbitionInterface::Inn.
 */

#define _AMBITION_IMPLEMENTATION
#include "inn.hh"

#include "OBUTT3D.h"
#include "OF_INN.h"
#include "OMOUSE.h"
#include "OVBROWIF.h"

#include "Ambition_config.hh"
#include "Ambition_inn.hh"


namespace _7kaaAmbitionInterface::Inn {

bool guestSelectionValid = false;


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

void guestLeft(
  FirmInn* inn,
  const VBrowseIF& guestBrowser,
  const int recordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (inn->firm_recno != firm_array.selected_recno || !inn->should_show_info()) {
    return;
  }

  if (guestBrowser.rec_no == recordNumber) {
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

void refreshBrowser(
  const short firmRecordNumber,
  FirmInn* inn,
  VBrowseIF& browser,
  const int removedUnitIndex,
  Button3D& hireButton
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  Ambition::refreshInnBrowser(
    firmRecordNumber,
    inn,
    browser,
    removedUnitIndex,
    hireButton
  );
}

} // namespace _7kaaAmbitionInterface::Inn
