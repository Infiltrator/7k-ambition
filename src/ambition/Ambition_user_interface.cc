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
 * Implementation file for Ambition::UserInterface.
 */

#include "Ambition_user_interface.hh"

#define _AMBITION_IMPLEMENTATION
#include "OFONT.h"
#include "OWORLD.h"
#include "vga_util.h"


namespace Ambition::UserInterface {

Rectangle Rectangle::inner(
  int paddingLeft,
  int paddingTop,
  int paddingRight,
  int paddingBottom
) const {
  if (paddingTop < 0) {
    paddingTop = paddingLeft;
  }
  if (paddingRight < 0) {
    paddingRight = paddingLeft;
  }
  if (paddingBottom < 0) {
    paddingBottom = paddingTop;
  }

  if (paddingLeft + paddingRight >= width()) {
    paddingLeft = std::min(paddingLeft, (width() - 1) / 2);
    paddingRight = std::min(paddingRight, (width() - 1) / 2);
  }
  if (paddingTop + paddingBottom >= height()) {
    paddingTop = std::min(paddingTop, (height() - 1) / 2);
    paddingBottom = std::min(paddingBottom, (height() - 1) / 2);
  }

  return {
    .start = {
      .left = start.left + paddingLeft,
      .top = start.top + paddingTop,
    },
    .end = {
      .left = end.left - paddingRight,
      .top = end.top - paddingBottom,
    },
  };
}

Rectangle Rectangle::outer(
  const int marginLeft,
  int marginTop,
  int marginRight,
  int marginBottom
) const {
  if (marginTop < 0) {
    marginTop = marginLeft;
  }
  if (marginRight < 0) {
    marginRight = marginLeft;
  }
  if (marginBottom < 0) {
    marginBottom = marginTop;
  }

  return {
    .start = {
      .left = std::max(start.left - marginLeft, BOUNDS.start.left),
      .top = std::max(start.top - marginTop, BOUNDS.start.top),
    },
    .end = {
      .left = std::min(end.left + marginRight, BOUNDS.end.left),
      .top = std::min(end.top + marginBottom, BOUNDS.end.top),
    },
  };
}

Rectangle Rectangle::internal(
  const Size size,
  const HorizontalAlignment horizontalAlignment,
  const VerticalAlignment verticalAlignment
) const {
  const auto horizontalSpace = width() - size.width;
  const auto verticalSpace = height() - size.height;

  int paddingLeft;
  int paddingTop;
  int paddingRight;
  int paddingBottom;

  if (horizontalAlignment == HorizontalAlignment::Left) {
    paddingLeft = 0;
    paddingRight = horizontalSpace;
  } else if (horizontalAlignment == HorizontalAlignment::Centre) {
    paddingLeft = horizontalSpace / 2;
    paddingRight = horizontalSpace / 2;
  } else if (horizontalAlignment == HorizontalAlignment::Right) {
    paddingLeft = horizontalSpace;
    paddingRight = 0;
  }

  if (verticalAlignment == VerticalAlignment::Top) {
    paddingTop = 0;
    paddingBottom = verticalSpace;
  } else if (verticalAlignment == VerticalAlignment::Centre) {
    paddingTop = verticalSpace / 2;
    paddingBottom = verticalSpace / 2;
  } else if (verticalAlignment == VerticalAlignment::Bottom) {
    paddingTop = verticalSpace;
    paddingBottom = 0;
  }

  return inner(paddingLeft, paddingTop, paddingRight, paddingBottom);
}


Point fromWorldPoint(
  Ambition::Coordinates::Point worldPoint,
  Ambition::Coordinates::Rectangle viewport
) {
  const auto relative = (worldPoint - viewport.topLeft()).asCoordinates();

  return {
    .left = static_cast<int>(ZOOM_X1 + ZOOM_LOC_WIDTH / 2 + (relative.x * 2)),
    .top = static_cast<int>(ZOOM_Y1 + ZOOM_LOC_HEIGHT / 2 - (relative.y * 2)),
  };
}

void printText(
  Font& font,
  const std::string text,
  const UserInterface::Rectangle area,
  const Clear clear,
  const HorizontalAlignment horizontalAlignment,
  const VerticalAlignment verticalAlignment
) {
  const auto textWidth = font.text_width(text.c_str());
  const auto textHeight = font.text_height();

  const auto textArea = area.internal(
    {
      .width = textWidth + 1,
      .height = textHeight + 1,
    },
    horizontalAlignment,
    verticalAlignment
  );

  if (clear != Clear::None) {
    const auto clearArea = clear == Clear::EntireArea ? area : textArea;
    vga_util.blt_buf(
      clearArea.start.left,
      clearArea.start.top,
      clearArea.end.left,
      clearArea.end.top,
      0
    );
  }

  font.put(
    textArea.start.left,
    textArea.start.top,
    text.c_str(),
    0,
    textArea.end.left
  );
}

} // namespace Ambition::UserInterface
