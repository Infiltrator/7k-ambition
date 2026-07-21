/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2025–26 Tim Sviridov
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
#include "pragma_silence_7kaa_warnings.hh"
#include "COLCODE.h"
#include "KEY.h"
#include "OBUTT3D.h"
#include "OIMGRES.h"
#include "OMOUSE.h"
#include "ONATIONA.h"
#include "OSLIDCUS.h"
#include "OSYS.h"
#include "OVGA.h"
#include "OWORLD.h"
#include "vga_util.h"
#pragma GCC diagnostic pop

#include "Ambition_coordinates.hh"
#include "Ambition_vga.hh"


namespace Ambition::UserInterface {

constexpr auto PIXELS_PER_DISTANCE = 32.0 / Coordinates::SCALING_FACTOR;


struct Section {
  std::string text;
  char* bitmap;
};


int rankReportComparison7kaaNationRecordNumber = 0;

BuildingMenu buildingMenu = BuildingMenu::_7kaa;
int reportType = -1;
short selected7kaaFirmOrTownRecordNumber = 0;


constexpr UserInterface::Size LOCALE_BUTTON_SIZE = {
  .width = 34,
  .height = 40,
};
const Rectangle LOCALE_BUTTON
  = UserInterface::BOUNDS
  .inner(10)
  .internal(LOCALE_BUTTON_SIZE);
constexpr auto ICON_CLICK_OUTSIDE_CLEARANCE = 4;
const Rectangle LOCALE_BUTTON_CLICK_AREA = LOCALE_BUTTON.outer(
  ICON_CLICK_OUTSIDE_CLEARANCE
);

constexpr auto BORDER_WIDTH = 1;

const Size SCROLL_BUTTON_SIZE = { .width = 14, .height = 17 };

namespace SelectionListScreen {

namespace Heading {

const Rectangle SECTION = BOUNDS.internal({ .height = 172 });

const Rectangle TEXT
  = SECTION
  .inner({ left: 90, right: 90, top: 32, bottom: 84 })
  .inner(TEXT_BOX_PADDING);

} // namespace SelectionListScreen::Heading

namespace ShortHeading {

const Rectangle SECTION = BOUNDS.internal({ .height = 101 });

const Rectangle TEXT
  = SECTION
  .inner({ left: 90, right: 90, top: 32, bottom: 13 })
  .inner(TEXT_BOX_PADDING);

} // namespace SelectionListScreen::ShortHeading

namespace Description {

extern const Rectangle SECTION
  = BOUNDS
  .inner({ top: static_cast<unsigned int>(Heading::SECTION.height()) })
  .internal({ .height = 148 });

const Rectangle TEXT_BOX
  = SECTION.inner({ left: 30, right: 29, top: 15, bottom: 13 });
const Rectangle TEXT
  = TEXT_BOX
  .inner(
    { right: static_cast<unsigned int>(SCROLL_BUTTON_SIZE.width + BORDER_WIDTH) }
  ).inner(TEXT_BOX_PADDING);

const Rectangle SCROLLBAR = TEXT_BOX.internal(
  { .width = SCROLL_BUTTON_SIZE.width },
  HorizontalAlignment::Right
);

} // namespace SelectionListScreen::Description

namespace List {

extern const Rectangle SECTION
  = BOUNDS
  .inner(
    {
      top: static_cast<unsigned int>(
        Heading::SECTION.height() + Description::SECTION.height()
      )
    }
  ).internal({ .height = 280 });

const Rectangle SLOT_AREA
  = SECTION.inner({ left: 30, right: 29, top: 15, bottom: 89 });
const Rectangle SLOTS = SLOT_AREA.inner(
  { right: static_cast<unsigned int>(SCROLL_BUTTON_SIZE.width + BORDER_WIDTH) }
);

const Rectangle SCROLLBAR = SLOT_AREA.internal(
  { .width = SCROLL_BUTTON_SIZE.width },
  HorizontalAlignment::Right
);

} // namespace SelectionListScreen::List

namespace LongList {

const Rectangle SECTION
  = BOUNDS.inner({ top: static_cast<unsigned int>(ShortHeading::SECTION.height()) });

const Rectangle SLOT_AREA
  = SECTION.inner({ left: 30, right: 29, top: 15, bottom: 89 });
const Rectangle SLOTS = SLOT_AREA.inner(
  { right: static_cast<unsigned int>(SCROLL_BUTTON_SIZE.width + BORDER_WIDTH) }
);

const Rectangle SCROLLBAR = SLOT_AREA.internal(
  { .width = SCROLL_BUTTON_SIZE.width },
  HorizontalAlignment::Right
);

} // namespace SelectionListScreen::LongList

namespace Buttons {

const Rectangle SECTION = BOUNDS.internal(
  { height: 85 },
  HorizontalAlignment::Centre,
  VerticalAlignment::Bottom
);

const Size BUTTON_SIZE = { .width = 166, .height = 46 };

const Rectangle BUTTON_ROW = SECTION.inner({ bottom: 25 });
const Rectangle START_BUTTON
  = BUTTON_ROW
  .inner({ left: 170 })
  .internal(BUTTON_SIZE, HorizontalAlignment::Left, VerticalAlignment::Bottom);
const Rectangle BACK_BUTTON
  = BUTTON_ROW
  .inner({ right: 170 })
  .internal(BUTTON_SIZE, HorizontalAlignment::Right, VerticalAlignment::Bottom);

} // namespace SelectionListScreen::Buttons

} // namespace SelectionListScreen


char _7kaaJustification(
  const HorizontalAlignment horizontalAlignment
);

std::vector<Section> processCodes(
  const std::string text
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
  const Space& padding
) const {
  return inner(padding.left, padding.top, padding.right, padding.bottom);
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
  const Space& margin
) const {
  return outer(margin.left, margin.top, margin.right, margin.bottom);
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
  const auto horizontalSpace = size.width == 0 ? 0 : width() - size.width;
  const auto verticalSpace = size.height == 0 ? 0 : height() - size.height;

  int paddingLeft {};
  int paddingTop {};
  int paddingRight {};
  int paddingBottom {};

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

void draw7kaaScrollbar(
  SlideVBar *scrollbar,
  const int _UNUSED
) {
  /* Refresh the entire trough from the back buffer. */
  vga_util.blt_buf(
    scrollbar->scrn_x1,
    scrollbar->scrn_y1,
    scrollbar->scrn_x2,
    scrollbar->scrn_y2
  );

  /* Draw the thumb. */
  const auto thumbTop = scrollbar->rect_top();
  const auto thumbBottom = scrollbar->rect_bottom();
  vga_front.bar(
    scrollbar->scrn_x1,
    thumbTop,
    scrollbar->scrn_x2,
    thumbBottom,
    VGA_YELLOW + 1
  );
  if (thumbBottom - thumbTop > 6) {
    constexpr auto BORDER_THICKNESS = 2;
    constexpr auto DO_NOT_FILL = 0;
    vga_front.d3_panel_up(
      scrollbar->scrn_x1,
      thumbTop,
      scrollbar->scrn_x2,
      thumbBottom,
      BORDER_THICKNESS,
      DO_NOT_FILL
    );
  }
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

void initScrollButton(
  Button3D& scrollButton,
  const UserInterface::Rectangle& scrollButtonLocation,
  const ScrollButtonDirection direction
) {
  constexpr auto UP_BUTTON_NORMAL_CODE = "SV-UP-U";
  constexpr auto UP_BUTTON_PUSHED_CODE = "SV-UP-D";
  constexpr auto DOWN_BUTTON_NORMAL_CODE = "SV-DW-U";
  constexpr auto DOWN_BUTTON_PUSHED_CODE = "SV-DW-D";

  constexpr auto ELASTIC = 1;
  constexpr auto IS_NOT_PUSHED = 0;

  scrollButton.create(
    scrollButtonLocation.start.left,
    scrollButtonLocation.start.top,
    direction == Up ? UP_BUTTON_NORMAL_CODE : DOWN_BUTTON_NORMAL_CODE,
    direction == Up ? UP_BUTTON_PUSHED_CODE : DOWN_BUTTON_PUSHED_CODE,
    ELASTIC,
    IS_NOT_PUSHED
  );
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
  const VerticalAlignment verticalAlignment,
  const int linesToSkip
) {
  const auto justification = _7kaaJustification(horizontalAlignment);

  font.put_paragraph(
    area.start.left,
    area.start.top,
    area.end.left,
    area.end.top,
    text.c_str(),
    lineSpacing,
    linesToSkip + 1,
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
    linesToSkip + 1,
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
  const auto sections = processCodes(text);

  constexpr auto BITMAP_MARGIN_LEFT = 1;
  constexpr auto BITMAP_MARGIN_RIGHT = 2;
  constexpr auto TOTAL_BITMAP_MARGIN = BITMAP_MARGIN_LEFT + BITMAP_MARGIN_RIGHT;

  int textWidth = 0;
  int textHeight = 0;
  for (const auto& section : sections) {
    if (section.bitmap) {
      const auto iconSize = bitmapSize(section.bitmap);
      textWidth += iconSize.width + TOTAL_BITMAP_MARGIN;
      textHeight = std::max(textHeight, iconSize.height);
    } else {
      textWidth += font.text_width(section.text.c_str());
      textHeight = std::max(textHeight, font.text_height());
    }
  }

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

  auto printArea = Rectangle::fromPoint(
    { .left = 0, .top = 0 },
    drawArea.size()
  );

  for (const auto& section : sections) {
    if (section.bitmap) {
      const auto iconArea
        = printArea
        .inner(BITMAP_MARGIN_LEFT, 0, 0, font.max_font_height - font.font_height)
        .internal(
          bitmapSize(section.bitmap),
          HorizontalAlignment::Left,
          VerticalAlignment::Centre
        );
      IMGbltTrans(
        drawBuffer,
        bufferPitch,
        iconArea.start.left,
        iconArea.start.top,
        section.bitmap
      );
      printArea = printArea.inner(iconArea.width() + TOTAL_BITMAP_MARGIN, 0, 0);
    } else {
      font.put_to_buffer(
        drawBuffer,
        bufferPitch,
        printArea.start.left,
        printArea.start.top,
        section.text.c_str()
      );
      printArea = printArea.inner(font.text_width(section.text.c_str()), 0, 0);
    }
  }

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

Section processSection(
  const std::string text
) {
  constexpr auto ICON_CODE = "@ICN(";
  if (text.starts_with(ICON_CODE)) {
    const auto iconKey = text.substr(5);
    return { "@ERR", image_icon.read(iconKey.c_str()) };
  }

  return { text, nullptr };
}
std::vector<Section> processCodes(
  const std::string text
) {
  std::vector<Section> sections;

  std::string buffer;
  for (auto character : text) {
    if (buffer[0] == '@') {
      if (character == ')') {
        sections.push_back(processSection(buffer));
        buffer.clear();
        continue;
      }
    } else {
      if (character == '@') {
        sections.push_back(processSection(buffer));
        buffer.clear();
      }
    }

    buffer.push_back(character);
  }

  sections.push_back(processSection(buffer));

  return sections;
}

} // namespace Ambition::UserInterface
