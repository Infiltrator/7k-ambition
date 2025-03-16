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
 * Implementation file for Ambition::Remote.
 */

#include "Ambition_remote.hh"

#include "OCRC_STO.h"
#include "OFONT.h"
#include "OWORLD.h"


namespace Ambition {

void printSyncError(
) {
  String errorMessage;
  errorMessage = "Sync error: ";
  errorMessage += crc_store.crc_error_string;

  font_news.disp(
    ZOOM_X1 + 10,
    ZOOM_Y1 + 42,
    errorMessage,
    MAP_X2
  );
}

} // namespace Ambition
