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
 * Header file for Ambition::Unit.
 */

#pragma once

class Firm;


namespace Ambition {

/**
 * Send an available builder to a Firm.
 *
 * A builder is considered to be available if he is idle or repairing a building
 * that is above a certain hitpoint percentage.
 *
 * @param firm The Firm to send a builder to.
 * @return Whether the Ambition code took effect and so the rest of the 7kaa
 * code should be skipped.
 */
bool sendAvailableBuilderToFirm(
  const Firm* firm
);

} // namespace Ambition
