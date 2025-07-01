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

#include "KEY.h"

class VBrowseIF;


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

using Action = std::function<void(const int distance)>;
using PageSize = std::function<int()>;

const Activation ACTIVATION_KEY_UP = {
  .orientation = Ambition::Input::ScrollOrientation::Vertical,
  .direction = Ambition::Input::ScrollDirection::Backward,
  .distance = Ambition::Input::ScrollStep::Single,
  .keyCodes = { KEY_UP },
};
const Activation ACTIVATION_KEY_DOWN = {
  .orientation = Ambition::Input::ScrollOrientation::Vertical,
  .direction = Ambition::Input::ScrollDirection::Forward,
  .distance = Ambition::Input::ScrollStep::Single,
  .keyCodes = {KEY_DOWN},
};
const Activation ACTIVATION_KEY_PAGE_UP = {
  .orientation = Ambition::Input::ScrollOrientation::Vertical,
  .direction = Ambition::Input::ScrollDirection::Backward,
  .distance = Ambition::Input::ScrollStep::Page,
  .keyCodes = {KEY_PGUP},
};
const Activation ACTIVATION_KEY_PAGE_DOWN = {
  .orientation = Ambition::Input::ScrollOrientation::Vertical,
  .direction = Ambition::Input::ScrollDirection::Forward,
  .distance = Ambition::Input::ScrollStep::Page,
  .keyCodes = { KEY_PGDN },
};
const Activation ACTIVATION_KEY_LEFT_PAGE_UP = {
  .orientation = Ambition::Input::ScrollOrientation::Vertical,
  .direction = Ambition::Input::ScrollDirection::Backward,
  .distance = Ambition::Input::ScrollStep::Page,
  .keyCodes = { KEY_LEFT },
};
const Activation ACTIVATION_KEY_RIGHT_PAGE_DOWN = {
  .orientation = Ambition::Input::ScrollOrientation::Vertical,
  .direction = Ambition::Input::ScrollDirection::Forward,
  .distance = Ambition::Input::ScrollStep::Page,
  .keyCodes = { KEY_RIGHT },
};
const Activation ACTIVATION_KEY_HOME = {
  .orientation = Ambition::Input::ScrollOrientation::Vertical,
  .direction = Ambition::Input::ScrollDirection::Backward,
  .distance = Ambition::Input::ScrollStep::End,
  .keyCodes = { KEY_HOME },
};
const Activation ACTIVATION_KEY_END = {
  .orientation = Ambition::Input::ScrollOrientation::Vertical,
  .direction = Ambition::Input::ScrollDirection::Forward,
  .distance = Ambition::Input::ScrollStep::End,
  .keyCodes = { KEY_END },
};

const std::vector<Activation> BASE_ACTIVATIONS = {
  ACTIVATION_KEY_UP,
  ACTIVATION_KEY_DOWN,
  ACTIVATION_KEY_HOME,
  ACTIVATION_KEY_END,
};
const std::vector<Activation> STANDARD_ACTIVATIONS = {
  ACTIVATION_KEY_UP,
  ACTIVATION_KEY_DOWN,
  ACTIVATION_KEY_PAGE_UP,
  ACTIVATION_KEY_PAGE_DOWN,
  ACTIVATION_KEY_HOME,
  ACTIVATION_KEY_END,
};
const std::vector<Activation> EXTENDED_ACTIVATIONS = {
  ACTIVATION_KEY_UP,
  ACTIVATION_KEY_DOWN,
  ACTIVATION_KEY_PAGE_UP,
  ACTIVATION_KEY_PAGE_DOWN,
  ACTIVATION_KEY_HOME,
  ACTIVATION_KEY_END,
  ACTIVATION_KEY_DOWN,
  ACTIVATION_KEY_LEFT_PAGE_UP,
  ACTIVATION_KEY_RIGHT_PAGE_DOWN,
};


bool detectReportScroll(
  VBrowseIF& browser,
  short& selectedRecordNumber,
  const UserInterface::Rectangle& area
);

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
