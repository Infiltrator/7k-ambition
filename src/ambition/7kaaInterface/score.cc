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
 * Implementation file for _7kaaAmbitionInterface::Score.
 */

#define _AMBITION_IMPLEMENTATION
#include "score.hh"

#include "pragma_silence_7kaa_warnings.hh"
#include "gettext.h"
#include "OBUTTON.h"
#include "OFONT.h"
#include "ONATION.h"
#pragma GCC diagnostic pop

#include "Ambition_config.hh"
#include "Ambition_user_interface.hh"
#include "format.hh"


namespace _7kaaAmbitionInterface::Score {

Button comparisonButton;


void detectComparisonButton(
  const int compared7kaaNationRecordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  if (comparisonButton.detect()) {
    if (Ambition::UserInterface::rankReportComparison7kaaNationRecordNumber
      == compared7kaaNationRecordNumber
    ) {
      Ambition::UserInterface::rankReportComparison7kaaNationRecordNumber = 0;
    } else {
      Ambition::UserInterface::rankReportComparison7kaaNationRecordNumber
        = compared7kaaNationRecordNumber;
    }
  }
}

void printComparisonScores(
  const int compared7kaaNationRecordNumber
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto comparisonActive
    = Ambition::UserInterface::rankReportComparison7kaaNationRecordNumber > 0
    && !nation_array.is_deleted(
      Ambition::UserInterface::rankReportComparison7kaaNationRecordNumber
    );

  using namespace Ambition::UserInterface::Report::Rank::Scores;
  comparisonButton.paint_text(
    COMPARISON_BUTTON.start.left,
    COMPARISON_BUTTON.start.top,
    COMPARISON_BUTTON.end.left,
    COMPARISON_BUTTON.end.top,
    // Button does not know how to do multi-line text, so we will manually
    // printParagraph() the text on top of the empty button.
    "",
    1,
    comparisonActive
  );

  const auto _7kaaNation = nation_array[
    comparisonActive
      ? Ambition::UserInterface::rankReportComparison7kaaNationRecordNumber
      : compared7kaaNationRecordNumber
  ];

  const auto colourCode = format(
    " @COL%d",
    _7kaaNation->color_scheme_id
  );

  const auto string = format(
    comparisonActive
      ? _("Comparing %s's Kingdom%s")
      : _("Compare %s's Kingdom%s"),
    _7kaaNation->king_name(1),
    colourCode.c_str()
  );

  Ambition::UserInterface::printParagraph(
    font_san,
    string,
    COMPARISON_BUTTON.inner(4),
    2,
    Ambition::UserInterface::Clear::None,
    Ambition::UserInterface::HorizontalAlignment::Centre,
    Ambition::UserInterface::VerticalAlignment::Centre
  );

  if (comparisonActive
    && Ambition::UserInterface::rankReportComparison7kaaNationRecordNumber
      != compared7kaaNationRecordNumber
  ) {
    constexpr auto TEXT_LINE_HEIGHT = 16;

    auto totalScore = 0;
    auto comparedTotalScore = 0;

    for (auto i = 0 ; i < MAX_RANK_TYPE; i++) {
      const auto row = COMPARISON_TEXT_AREA.inner(0, i * TEXT_LINE_HEIGHT, 0, 0);

      const auto score = info.get_rank_score(
        i + 1,
        Ambition::UserInterface::rankReportComparison7kaaNationRecordNumber
      );
      totalScore += score;

      Ambition::UserInterface::printText(
        font_san,
        format("%d", score),
        row.intersection(COMPARISON_SCORE_COLUMN),
        Ambition::UserInterface::Clear::None,
        Ambition::UserInterface::HorizontalAlignment::Right
      );

      const auto comparedScore = info.get_rank_score(
        i + 1,
        compared7kaaNationRecordNumber
      );
      comparedTotalScore += comparedScore;

      Ambition::UserInterface::printText(
        font_san,
        format("(%+d)", score - comparedScore),
        row.intersection(COMPARISON_SCORE_DIFFERENCE_COLUMN)
      );
    }

    constexpr auto TOTAL_ROW_LINE_HEIGHT = 2;
    constexpr auto TOTAL_ROW_LINE_SPACING = 4;
    const auto lineArea
      = COMPARISON_TEXT_AREA
      .inner(0, MAX_RANK_TYPE * TEXT_LINE_HEIGHT, 0, 0)
      .internal(
        {
          .width
            = COMPARISON_SCORE_COLUMN.width()
              + COLUMN_GAP
              + COMPARISON_SCORE_DIFFERENCE_COLUMN.width(),
          .height = TOTAL_ROW_LINE_HEIGHT,
        }
      );
    Ambition::UserInterface::drawRectangle(lineArea);

    const auto totalRow = COMPARISON_TEXT_AREA.inner(
      0,
      MAX_RANK_TYPE * TEXT_LINE_HEIGHT
        + TOTAL_ROW_LINE_HEIGHT
        + TOTAL_ROW_LINE_SPACING,
      0,
      0
    );

    Ambition::UserInterface::printText(
      font_san,
      format("%d", totalScore),
      totalRow.intersection(COMPARISON_SCORE_COLUMN),
      Ambition::UserInterface::Clear::None,
      Ambition::UserInterface::HorizontalAlignment::Right
    );
    Ambition::UserInterface::printText(
      font_san,
      format("(%+d)", totalScore - comparedTotalScore),
      totalRow.intersection(COMPARISON_SCORE_DIFFERENCE_COLUMN)
    );
  }
}

} // namespace _7kaaAmbitionInterface::Score
