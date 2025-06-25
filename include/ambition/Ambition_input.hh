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
 * Header file for Ambition::Input.
 */

#pragma once

#include <functional>
#include <map>
#include <vector>


namespace Ambition {

namespace UserInterface {
struct Rectangle;
}


void calculateScroll(
  int& x,
  int& y
);

namespace Input {

enum class ScrollOrientation {
  Horizontal,
  Vertical,
};
enum class ScrollDirection {
  Forward,
  Backward,
};
enum class ScrollStep {
  Single,
  Page,
  End,
};

using _7kaaButtonDetect = std::function<int()>;

struct Activation {
  ScrollOrientation orientation;
  ScrollDirection direction;
  ScrollStep distance;
  std::vector<unsigned int> keyCodes;
  std::vector<_7kaaButtonDetect> _7kaaButtonDetects;
};

using Action = std::function<void(int distance)>;
using PageSize = std::function<int()>;


bool detectScroll(
  const bool enableMouse,
  const UserInterface::Rectangle& area,
  const std::vector<Activation>& activations,
  const std::map<ScrollOrientation, Action>& actions,
  const PageSize verticalPageSize,
  const PageSize verticalMaximum = []() { return 1000000; },
  const PageSize horizontalPageSize = []() { return 0; },
  const PageSize horizontalMaximum = []() { return 1000000; }
);

} // namespace Ambition::Input

} // namespace Ambition
