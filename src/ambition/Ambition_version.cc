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
 * Version information for Seven Kingdoms: Ambition.
 */

#include "Ambition_version.hh"

#include "git_string.h"

namespace Ambition {

std::string versionString() {
  return std::to_string(MAJOR_VERSION)
	 + "." + std::to_string(MINOR_VERSION)
	 + "." + std::to_string(PATCH_VERSION)
    + (RELEASE_CANDIDATE_NUMBER > 0
       ? "-rc" + std::to_string(RELEASE_CANDIDATE_NUMBER)
       : "")
	 + GIT_STRING;
}

} // namespace Ambition
