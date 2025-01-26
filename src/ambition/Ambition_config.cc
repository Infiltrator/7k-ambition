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
 * Implementation file for Ambition::Config.
 */

#include "Ambition_config.hh"

#include <stdexcept>

#include "ConfigAdv.h"
#include "OFONT.h"

#include "ambition/Ambition_vga.hh"

namespace Ambition {

/** The global Ambition Config. */
Config config;

std::string
Config::modeString(
) const {
  switch (_mode) {
  case Mode::Classic:
	 return "classic";
  case Mode::Enhanced:
	 return "enhanced";
  case Mode::Advanced:
	 return "advanced";
  case Mode::Ambition:
	 return "ambition";
  }

  return "ERROR:UNKNOWN";
}

bool
Config::compatibleWith7kaa(
) const {
  return _mode <= Mode::Enhanced;
};

bool
Config::enhancementsAvailable(
) const {
  return _mode >= Mode::Enhanced;
}

void Config::setMode(
  const Mode mode
) {
  // if(!currentState.allowsModeTransitions()) {
  // 	 throw std::domain_error("The current state does not allow mode transitions.");
  // }

  _mode = mode;

  if (!compatibleWith7kaa()) {
    config_adv.flags |= ConfigAdv::FLAG_NOT_7KAA_COMPATIBLE;
  } else {
    config_adv.flags &= ~ConfigAdv::FLAG_NOT_7KAA_COMPATIBLE;
  }
}

Config::Mode Config::currentMode(
) const {
  return _mode;
}


constexpr auto MODE_INFORMATION_TOP = 6;
constexpr auto MODE_INFORMATION_RIGHT = 794;

void drawModeInformation(
) {
  font_news.right_put(
    MODE_INFORMATION_RIGHT,
    MODE_INFORMATION_TOP,
    (char*)"Current mode:"
  );
  font_news.right_put(
    MODE_INFORMATION_RIGHT,
    MODE_INFORMATION_TOP + 12,
    (char*)(config.modeString()).c_str()
  );
}

} // namespace Ambition
