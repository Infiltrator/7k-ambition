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
 * Implementation file for Ambition::Input.
 */

#include "Ambition_input.hh"

#include <algorithm>

#define _AMBITION_IMPLEMENTATION
#include "OMOUSE.h"
#include "OVBROWIF.h"

#include "Ambition_user_interface.hh"


namespace Ambition::Input {

bool detectReportScroll(
  VBrowseIF& browser,
  short& selectedRecordNumber,
  const UserInterface::Rectangle& area
) {
  return detectScroll(
    true,
    area,
    EXTENDED_ACTIVATIONS,
    {
      {
        ScrollOrientation::Vertical,
        [&browser, &selectedRecordNumber] (
          const int amount
        ) {
          browser.top_rec_no = std::clamp(
            browser.top_rec_no + amount,
            1,
            browser.total_rec()
          );
          selectedRecordNumber = std::clamp(
            selectedRecordNumber + amount,
            1,
            browser.total_rec()
          );
        }
      },
    },
    [browser]() { return browser.y_max_rec - 1; },
    [browser]() { return browser.total_rec_num; }
  );
}

bool detectScroll(
  const bool enableMouse,
  const UserInterface::Rectangle& area,
  const std::vector<Activation>& activations,
  const std::map<ScrollOrientation, Action>& actions,
  const PageSize verticalPageSize,
  const PageSize verticalMaximum,
  const PageSize horizontalPageSize,
  const PageSize horizontalMaximum
) {
  if (enableMouse
    && !area.contains({.left = mouse.cur_x, .top = mouse.cur_y,})
  ) {
    return false;
  }

  int scrollHorizontal = 0;
  int scrollVertical = 0;

  if (mouse.get_scroll(&scrollHorizontal, &scrollVertical)) {
  } else {
    for (const auto& activation : activations) {
      int& scroll
        = activation.orientation == ScrollOrientation::Horizontal
        ? scrollHorizontal
        : scrollVertical;
      auto step = 1;
      if (activation.distance == ScrollStep::Page) {
        step
          = activation.orientation == ScrollOrientation::Horizontal
          ? horizontalPageSize()
          : verticalPageSize();
      }
      if (activation.distance == ScrollStep::End) {
        step
          = activation.orientation == ScrollOrientation::Horizontal
          ? horizontalMaximum()
          : verticalMaximum();
      }
      const auto amount
        = (activation.direction == ScrollDirection::Forward ? 1 : -1)
        * step;

      for (const auto& _7kaaDetect : activation._7kaaButtonDetects) {
        if (_7kaaDetect()) {
          scroll = amount;
        }
      }
      for (const auto& keyCode : activation.keyCodes) {
        if (mouse.unique_key_code == keyCode) {
          scroll = amount;
        }
      }
    }
  }

  bool actionExecuted = false;
  if (scrollHorizontal != 0) {
    const auto action = actions.find(ScrollOrientation::Horizontal);
    if (action != actions.end()) {
      action->second(scrollHorizontal);
      actionExecuted = true;
    }
  }
  if (scrollVertical != 0) {
    const auto action = actions.find(ScrollOrientation::Vertical);
    if (action != actions.end()) {
      action->second(scrollVertical);
      actionExecuted = true;
    }
  }

  return actionExecuted;
}

} // namespace Ambition::Input
