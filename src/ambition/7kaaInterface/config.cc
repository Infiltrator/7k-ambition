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

#include <SDL2/SDL_filesystem.h>

#include "ConfigAdv.h"
#include "OSYS.h"

#include "Ambition_config.hh"
#include "Ambition_version.hh"


namespace _7kaaAmbitionInterface::Config {

bool compatibleWith7kaa(
) {
  return Ambition::config.compatibleWith7kaa();
}

std::string configDirectoryPath(
) {
  return Ambition::DirectoryPath::config().string();
}

bool enhancementsAvailable(
) {
  return Ambition::config.enhancementsAvailable();
}

void load7kaaConfig(
) {
  constexpr auto FILE_NAME = "config.txt";

  const auto _7kaaConfigDirectory = SDL_GetPrefPath("7kfans.com", "7kaa");
  strcpy(sys.dir_config, _7kaaConfigDirectory);
  SDL_free(_7kaaConfigDirectory);

  config_adv.load(FILE_NAME);

  sys.set_config_dir();
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

std::string screenshotDirectoryPath(
) {
  return Ambition::DirectoryPath::screenshot().string();
}

std::string saveDirectoryPath(
  const std::string filename
) {
  return Ambition::DirectoryPath::save(filename).string();
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
