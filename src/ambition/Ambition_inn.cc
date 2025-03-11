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
 * Implementation file for Ambition::Inn.
 */

#include "Ambition_inn.hh"

#include <stdint.h>

#include "OBUTT3D.h"
#include "OF_INN.h"
#include "OVBROWIF.h"

#include "Ambition_config.hh"


namespace Ambition {

int getInnSelectedRecordNumber(
  const FirmInn* inn,
  const int browserRecordNumber
) {
  if (!config.enhancementsAvailable()) {
    return browserRecordNumber;
  }

  return inn->inn_unit_count - browserRecordNumber + 1;
}

void refreshInnBrowser(
  const short firmRecordNumber,
  FirmInn* inn,
  VBrowseIF& browser,
  const int removedUnitIndex,
  Button3D& hireButton
) {
  if (!config.enhancementsAvailable()) {
    return;
  }

  if (
    firmRecordNumber != firm_array.selected_recno
    || !inn->should_show_info()
  ) {
    return;
  }

  if (removedUnitIndex > inn->inn_unit_count) {
    browser.refresh(browser.recno() + 1, inn->inn_unit_count);
  } else {
    if (browser.recno() == inn->inn_unit_count - removedUnitIndex + 1) {
      browser.refresh(1, inn->inn_unit_count);
      hireButton.disable();
    } else if (
      browser.recno() > inn->inn_unit_count - removedUnitIndex
      && browser.recno() > 1
    ) {
      browser.refresh(browser.recno() - 1, inn->inn_unit_count);
    } else {
      browser.refresh(browser.recno(), inn->inn_unit_count);
    }
  }
}

} // namespace Ambition
