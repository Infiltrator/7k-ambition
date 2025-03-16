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
 * Implementation file for _7kaaAmbitionInterface::Config.
 */

#define _AMBITION_IMPLEMENTATION
#include "config.hh"

#include "Ambition_config.hh"
#include "Ambition_version.hh"


namespace _7kaaAmbitionInterface::Config {

bool compatibleWith7kaa(
) {
  return Ambition::config.compatibleWith7kaa();
}

bool enhancementsAvailable(
) {
  return Ambition::config.enhancementsAvailable();
}

std::string modeString(
) {
  return Ambition::config.modeString();
}

void set7kaaConfigOption(
  char* key,
  char* value,
  const int lineNumber
) {
  Ambition::set7kaaConfigOption(key, value, lineNumber);
}

void report7kaaConfigLoadingErrors(
  const char* filename
) {
  Ambition::report7kaaConfigLoadingErrors(filename);
}

std::string versionString(
) {
  return Ambition::versionString();
}

} // namespace _7kaaAmbitionInterface::Config
