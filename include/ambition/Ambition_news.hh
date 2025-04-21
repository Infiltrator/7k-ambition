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
 * Header file for Ambition::News.
 */

#pragma once

#include <numeric>
#include <string>
#include <vector>


namespace Ambition::News {

struct VersionUpdates {
  std::string identifier;

  struct Section {
    std::string heading;
    std::vector<std::string> items;
  };
  std::vector<Section> sections;

  auto totalItemCount(
  ) const {
    return std::accumulate(
      sections.begin(),
      sections.end(),
      0,
      [](const int accumulator, const Section& section) {
        return accumulator + section.items.size();
      }
    );
  }
};


void display(
);

std::string lastDisplayedNewsVersion(
);

void saveDisplayedNewsVersion(
);


extern std::vector<VersionUpdates> versionDetails;

} // namespace Ambition::News
