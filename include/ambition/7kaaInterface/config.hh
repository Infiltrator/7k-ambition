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
 * Header file for _7kaaAmbitionInterface::Config.
 */

#pragma once

#include <string>


namespace _7kaaAmbitionInterface::Config {

/**
 * Whether the current running mode is multiplayer-compatible with 7K:AA.
 */
bool compatibleWith7kaa(
);

/**
 * Whether the current running mode has enhancements available.
 */
bool enhancementsAvailable(
);

/**
 * A string representation of the current running mode.
 */
std::string modeString(
);

/**
 * Set a 7K:AA (advanced) config option.  These are the ones in config.txt.
 *
 * @param key The key of the option to set.
 * @param value The value to set the key to.
 * @param lineNumber The line number of the config.txt file that is currently
 * being read to attempt to set an option.
 */
void set7kaaConfigOption(
  char* key,
  char* value,
  const int lineNumber
);

/**
 * Report all of the 7K:AA (advanced) config option file loading errors, if any.
 *
 * @param The filename of the config file.
 */
void report7kaaConfigLoadingErrors(
  const char* filename
);

std::string versionString(
);

} // namespace _7kaaAmbitionInterface::Config

#ifndef _AMBITION_IMPLEMENTATION
/** Allow 7kaa to call using Ambition::*. */
namespace Ambition = _7kaaAmbitionInterface;
#endif
