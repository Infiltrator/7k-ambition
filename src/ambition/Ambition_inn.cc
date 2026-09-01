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
 * Implementation file for Ambition::Inn.
 */

#include "Ambition_inn.hh"

#define _AMBITION_IMPLEMENTATION
#include "pragma_silence_7kaa_warnings.hh"
#include "OBUTT3D.h"
#include "OF_INN.h"
#include "OVBROWIF.h"
#pragma GCC diagnostic pop


namespace Ambition {

namespace Inn {

int competingInnCount(
  const FirmInn* _7kaaInn
) {
  auto count = 0;
  for (auto i = 0; i < _7kaaInn->linked_firm_count; i++) {
    if (firm_array[_7kaaInn->linked_firm_array[i]]->firm_id == FIRM_INN) {
      count++;
    }
  }
  return count;
}

} // namespace Ambition::Inn

int getInnSelectedRecordNumber(
  const FirmInn* inn,
  const int browserRecordNumber
) {
  // TODO: Log and/or dig into why this condition occurs.
  if (browserRecordNumber < 1
    || browserRecordNumber > inn->inn_unit_count
  ) {
    return 1;
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
  if (
    firmRecordNumber != firm_array.selected_recno
    || !inn->should_show_info()
  ) {
    return;
  }

  if (removedUnitIndex < 0) {
    browser.refresh(browser.recno() + 1, inn->inn_unit_count);
  } else {
    /* inn->inn_unit_count has already been decremented by the removed guest, so
     * we need to add 1 back whenever we're using it as if it were the old guest
     * list. */
    if (browser.recno() == (inn->inn_unit_count + 1) - removedUnitIndex + 1) {
      browser.refresh(1, inn->inn_unit_count);
      hireButton.disable();
    } else if (browser.recno()
        > (inn->inn_unit_count + 1) - removedUnitIndex + 1
    ) {
      browser.refresh(browser.recno() - 1, inn->inn_unit_count);
    } else {
      browser.refresh(browser.recno(), inn->inn_unit_count);
    }
  }
}

} // namespace Ambition
