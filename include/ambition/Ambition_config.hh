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
 * Header file for Ambition::Config.
 */

#pragma once

#include <string>

namespace Ambition {

/**
 * The high-level configuration information for Seven Kingdoms: Ambition.
 */
struct Config {
public:
  /**
	* A string representation of the current running mode.
	*/
  std::string modeString() const;

  /**
	* Whether the current running mode is compatible with 7kaa multiplayer.
	*/
  bool compatibleWith7kaa() const;
  /**
	* Whether the current running mode has enhancements available.
	*/
  bool enhancementsAvailable() const;

  enum class Mode {
    Classic,  /// Run everything the exact same as 7Kfans' 7kaa.
    Enhanced, /// Add enhancements that do not affect gameplay.
    Advanced, /// Add enhancements and bugfixes that may affect gameplay.
    /** Add all of the ambitious new features.  Arguably a different game to
	  * 7K:AA. */
	 Ambition,
  };

  /**
	* Set the running mode.
	*
	* \throws An error if unable to change modes due to the current state
	* (e.g. in the middle of a match.)
	*/
  void setMode(
	 const Mode mode
  );

  /**
   * The current running mode.
   */
  Mode currentMode() const;

private:
  Mode _mode {Mode::Enhanced};
};

/** The global Ambition Config. */
extern Config config;


/**
 * Draw the current running mode onto the top-right of the screen.
 */
void drawModeInformation();

} // namespace Ambition
