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
 * Header file for Ambition::Control.
 */

#pragma once


namespace Ambition {

void requestFeedback(
);

void resetGameState(
);


namespace Control {

void finishReplay(
);

/**
 * Migrate files from the everything-in-one config directory to the separate
 * ones.
 *
 * 7kaa has everything together in the base config directory, which can get
 * confusing, especially when asking users to locate specific files.  Ambition
 * stores separate data types in separate config, single-player saves,
 * multi-player saves, and screenshot directories.
 *
 * As Ambition used to have the same behaviour as 7kaa, this function migrates
 * any existing files that were created in the past into the new directories.
 */
void migrateLocalDataDirectories(
);

void openDiscord(
);

void openFeedback(
);

void startMusic(
);

} // namespace Ambition::Control

} // namespace Ambition
