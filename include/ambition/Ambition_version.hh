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
 * @file Ambition_version.hh
 *
 * Version information for Seven Kingdoms: Ambition.
 */

#ifndef Ambition_version_hh_
#define Ambition_version_hh_

#include <string>

namespace Ambition {

constexpr auto MAJOR_VERSION = 2;
constexpr auto MINOR_VERSION = 9156;
constexpr auto PATCH_VERSION = 0;

std::string versionString();

} // namespace Ambition

#endif
