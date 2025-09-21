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

#include <ranges>

#define _AMBITION_IMPLEMENTATION
#include "KEY.h"
#include "OMOUSE.h"
#include "ONATIONA.h"
#include "OSYS.h"
#include "OVGA.h"
#include "OWORLD.h"
#include "vga_util.h"

#include "Ambition_coordinates.hh"
#include "Ambition_vga.hh"


namespace Ambition::UserInterface {

constexpr auto PIXELS_PER_DISTANCE = 32.0 / Coordinates::SCALING_FACTOR;


int rankReportComparison7kaaNationRecordNumber = 0;

BuildingMenu buildingMenu = BuildingMenu::_7kaa;
int reportType = -1;
short selected7kaaFirmOrTownRecordNumber = 0;


char _7kaaJustification(
  const HorizontalAlignment horizontalAlignment
);


Rectangle Rectangle::fromWorldRectangle(
  const Coordinates::Rectangle& coordinateRectangle
) {
  return fromPoint(
    fromWorldPoint(coordinateRectangle.topLeft()),
    {
      .width = static_cast<int>(
        coordinateRectangle.width() * PIXELS_PER_DISTANCE
      ),
      .height = static_cast<int>(
        coordinateRectangle.height() * PIXELS_PER_DISTANCE
      ),
    }
  );
}


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
      .left = start.left - marginLeft,
      .top = start.top - marginTop,
    },
    .end = {
      .left = end.left + marginRight,
      .top = end.top + marginBottom,
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

bool Rectangle::intersects(
  const Rectangle& with
) const {
  return (
    (((start.left >= with.start.left && start.left <= with.end.left)
       || (end.left >= with.start.left && end.left <= with.end.left))
      && ((start.top >= with.start.top && start.top <= with.end.top)
        || (end.top >= with.start.top && end.top <= with.end.top)))
    || (((with.start.left >= start.left && with.start.left <= end.left)
        || (with.end.left >= start.left && with.end.left <= end.left))
      && ((with.start.top >= start.top && with.start.top <= end.top)
        || (with.end.top >= start.top && with.end.top <= end.top)))
  );
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

bool drawInformationPanel(
  const Rectangle& bounds,
  const Rectangle& panelArea,
  const std::vector<std::string>& lines,
  const HorizontalAlignment alignment,
  Font& font,
  const int lineSpacing
) {
  if (!VIEWPORT.intersects(panelArea)) {
    return false;
  }

  const auto totalTextHeight
    = lines.size() * font.font_height
    + (lines.size() - 1) * lineSpacing;

  const auto textArea = panelArea.internal(
    { .width = panelArea.width(), .height = static_cast<int>(totalTextHeight) },
    HorizontalAlignment::Centre,
    VerticalAlignment::Centre
  );

  /* The panel interferes with the panels drawn by the reports. */
  if (sys.view_mode == MODE_NORMAL) {
    drawPanel(VIEWPORT.intersection(panelArea));
  }

  const auto restoreVgaFront = !vga.use_back_buf;
  vga.use_back();

  for (const auto i : std::views::iota(0U, lines.size())) {
    const auto lineArea = textArea.inner(
      0,
      i * (font.font_height + lineSpacing),
      0,
      0
    );

    printText(
      font,
      lines[i],
      lineArea,
      Clear::None,
      alignment,
      VerticalAlignment::Top,
      VIEWPORT
    );
  }

  if (restoreVgaFront) {
    vga.use_front();
  }

  return true;
}

void drawRectangle(
  const Rectangle& rectangle,
  const int colourIndex
) {
  vga.active_buf->bar(
    rectangle.start.left,
    rectangle.start.top,
    rectangle.end.left,
    rectangle.end.top,
    colourIndex
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
  if (keyCode == KEY_RETURN) {
    return "Enter";
  }

  return "UNKNOWN";
}

Point fromWorldPoint(
  Ambition::Coordinates::Point worldPoint,
  Ambition::Coordinates::Rectangle viewport
) {
  const auto relative = (worldPoint - viewport.topLeft()) * PIXELS_PER_DISTANCE;

  return {
    .left = static_cast<int>(VIEWPORT.start.left + relative.x),
    .top = static_cast<int>(VIEWPORT.start.top - relative.y),
  };
}

bool mouseCursorInArea(
  const Rectangle area
) {
  return mouse.in_area(
    area.start.left,
    area.start.top,
    area.end.left,
    area.end.top
  );
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
  const VerticalAlignment verticalAlignment,
  const Rectangle& bounds
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
    const auto clearArea
      = (clear == Clear::EntireArea ? area : textArea)
      .intersection(bounds);
    vga_util.blt_buf(
      clearArea.start.left,
      clearArea.start.top,
      clearArea.end.left,
      clearArea.end.top,
      0
    );
  }

  const auto drawArea = textArea.outer(
    0,
    0,
    0,
    font.max_font_height - font.font_height
  );
  const auto withinBounds = bounds.contains(drawArea);
  const auto drawBuffer
    = withinBounds
    ? vga.active_buf->buf_ptr(drawArea.start.left, drawArea.start.top)
    : Vga::prepareBitmapBuffer(sys.common_data_buf, drawArea.size());
  const auto bufferPitch
    = withinBounds ? vga.active_buf->buf_pitch() : drawArea.width();

  const auto printArea = Rectangle::fromPoint(
    { .left = 0, .top = 0 },
    drawArea.size()
  );

  font.put_to_buffer(
    drawBuffer,
    bufferPitch,
    printArea.start.left,
    printArea.start.top,
    text.c_str()
  );

  if (!withinBounds) {
    const auto inside = bounds.intersection(drawArea);
    const auto sourceArea = Rectangle::fromPoint(
      {
        .left = inside.start.left - drawArea.start.left,
        .top = inside.start.top - drawArea.start.top,
      },
      inside.size()
    );

    vga.active_buf->put_bitmap_area_trans(
      drawArea.start.left,
      drawArea.start.top,
      sys.common_data_buf,
      sourceArea.start.left,
      sourceArea.start.top,
      sourceArea.end.left,
      sourceArea.end.top
    );
  }
}

void resetState(
) {
  rankReportComparison7kaaNationRecordNumber = nation_array.player_recno;

  buildingMenu = BuildingMenu::_7kaa;
  reportType = -1;
  selected7kaaFirmOrTownRecordNumber = 0;
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
