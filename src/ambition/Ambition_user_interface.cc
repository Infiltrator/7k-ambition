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
#include "KEY.h"
#include "OFONT.h"
#include "OMOUSE.h"
#include "OWORLD.h"
#include "vga_util.h"

#include "Ambition_coordinates.hh"


namespace Ambition::UserInterface {

char _7kaaJustification(
  const HorizontalAlignment horizontalAlignment
);


bool Rectangle::contains(
  const Point& point
) const {
  return (
    point.left >= start.left && point.left <= end.left
    && point.top >= start.top && point.top <= end.top
  );
}
bool Rectangle::contains(
  const Rectangle& rectangle
) const {
  return intersection(rectangle) == rectangle;
}

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

Rectangle Rectangle::intersection(
  const Rectangle with
) const {
  return {
    .start = {
      .left = std::max(start.left, with.start.left),
      .top = std::max(start.top, with.start.top),
    },
    .end = {
      .left = std::min(end.left, with.end.left),
      .top = std::min(end.top, with.end.top),
    },
  };
}


Size bitmapSize(
  const char* bitmap
) {
  const auto bitmapHeader = reinterpret_cast<const unsigned char*>(bitmap);
  return {
    .width = bitmapHeader[0] + (bitmapHeader[1] << 8),
    .height = bitmapHeader[2] + (bitmapHeader[3] << 8),
  };
}

bool detectMouseClick(
  const Rectangle area
) {
  return mouse.single_click(
    area.start.left,
    area.start.top,
    area.end.left,
    area.end.top
  );
}

void drawPanel(
  const Rectangle rectangle
) {
  vga_util.d3_panel_up(
    rectangle.start.left,
    rectangle.start.top,
    rectangle.end.left,
    rectangle.end.top
  );
}

std::string keyString(
  const unsigned int keyCode
) {
  if (keyCode >= 'a' && keyCode <= 'z') {
    // Use upper-case versions of letters.
    return { static_cast<char>(keyCode - 'a' + 'A') };
  }
  if (keyCode >= '!' && keyCode <= '~') { // Standard printable range.
    return { static_cast<char>(keyCode) };
  }

  if (keyCode == ' ') {
    return "Space";
  }
  if (keyCode == KEY_ESC) {
    return "Esc";
  }

  return "UNKNOWN";
}

Point fromWorldPoint(
  Ambition::Coordinates::Point worldPoint,
  Ambition::Coordinates::Rectangle viewport
) {
  constexpr auto PIXELS_PER_DISTANCE = 32.0 / Coordinates::SCALING_FACTOR;

  const auto relative = (worldPoint - viewport.topLeft()) * PIXELS_PER_DISTANCE;

  return {
    .left = static_cast<int>(VIEWPORT.start.left + relative.x),
    .top = static_cast<int>(VIEWPORT.start.top - relative.y),
  };
}

void printParagraph(
  Font& font,
  const std::string text,
  const Rectangle area,
  const int lineSpacing,
  const Clear clear,
  const HorizontalAlignment horizontalAlignment,
  const VerticalAlignment verticalAlignment
) {
  const auto justification = _7kaaJustification(horizontalAlignment);

  font.put_paragraph(
    area.start.left,
    area.start.top,
    area.end.left,
    area.end.top,
    text.c_str(),
    lineSpacing,
    1,
    0,
    justification
  );
  const auto textHeight
    = font.next_text_y - font.font_height - lineSpacing - area.start.top;

  const auto textArea = area.internal(
    {
      .width = area.width(),
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

  font.put_paragraph(
    textArea.start.left,
    textArea.start.top,
    textArea.end.left,
    textArea.end.top,
    text.c_str(),
    lineSpacing,
    1,
    1,
    justification
  );
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


/* Private functions. */

char _7kaaJustification(
  const HorizontalAlignment horizontalAlignment
) {
  if (horizontalAlignment == HorizontalAlignment::Left) {
    return Font::LEFT_JUSTIFY;
  }
  if (horizontalAlignment == HorizontalAlignment::Centre) {
    return Font::CENTER_JUSTIFY;
  }
  if (horizontalAlignment == HorizontalAlignment::Right) {
    return Font::RIGHT_JUSTIFY;
  }

  return Font::AUTO_JUSTIFY;
}

} // namespace Ambition::UserInterface
