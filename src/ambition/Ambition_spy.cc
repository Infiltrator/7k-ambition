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
 * Implementation file for Ambition::Spy.
 */

#include "Ambition_spy.hh"

#include <algorithm>
#include <cmath>
#include <cstdint>

#define _AMBITION_IMPLEMENTATION
#include "OBUTT3D.h"
#include "OFIRM.h"
#include "ONATIONA.h"
#include "OSPY.h"
#include "OSYS.h"
#include "OUNIT.h"


namespace Ambition::Spy {

Button3D assassinationButton;
Button3D cancelButton;
Button3D stealReportsButton;


double confidenceFactor(
  const ::Spy* _7kaaSpy
) {
  const auto t = _7kaaSpy->spy_recno + sys.frame_count / 337.0;
  const auto wave
    = 0.6 * std::sin(t)
    + 0.25 * std::sin(t * 4)
    + 0.15 * std::sin(t * 16);
  const auto skillAdjusted = wave * (0.55 - 0.5 * _7kaaSpy->spy_skill / 100.0);
  return (
    skillAdjusted < 0
    ? 1 / (1 + skillAdjusted)
    : 1 - skillAdjusted
  );
}

int bribeAmountEstimate(
  const ::Spy* _7kaaSpy,
  const int aimForChance,
  const int targetLoyalty,
  const int targetRaceId,
  const int targetCommandPower
) {
  /* Required bribe for chance = (chance + commandPower + loyaltyEffect -
   * reputation - spySkill - cultureEffect) / 200 * MAX_BRIBE_AMOUNT. */

  if (targetLoyalty == 100) {
    return std::numeric_limits<int>::max();
  }

  const auto _7kaaNation = nation_array[_7kaaSpy->true_nation_recno];
  const auto cultureEffect
    = 10 * (_7kaaSpy->race_id == targetRaceId)
    + 10 * (_7kaaNation->race_id == targetRaceId);

  const auto loyaltyEffect
    = targetLoyalty
    + std::max(0, targetLoyalty - 60)
    + std::max(0, targetLoyalty - 70)
    + std::max(0, targetLoyalty - 80)
    + std::max(0, targetLoyalty - 90);

  const auto bribeAmount = std::ceil(
    (aimForChance
      + targetCommandPower
      + loyaltyEffect
      - std::floor(_7kaaNation->reputation)
      - _7kaaSpy->spy_skill
      - cultureEffect
    ) / 200 * MAX_BRIBE_AMOUNT
  );

  return bribeAmount / confidenceFactor(_7kaaSpy);
}


double assassinationChanceEstimate(
  const ::Spy* _7kaaSpy,
  const Firm* _7kaaFirm,
  ::Unit* target
) {
  auto attackRating = 0;
  int apparentDefenceRating = 60 + target->hit_points / 2;
  auto possibleExtraDefenceRating = 0;
  auto defenderCount = 0;
  for (auto i = 0; i < _7kaaFirm->worker_count; i++) {
    const auto _7kaaWorker = _7kaaFirm->worker_array[i];
    if (_7kaaWorker.spy_recno == _7kaaSpy->spy_recno) {
      attackRating += _7kaaSpy->spy_skill;
      attackRating += _7kaaSpy->spy_skill / 4;
      attackRating += _7kaaWorker.hit_points / 2;
    } else if (_7kaaWorker.spy_recno
      && spy_array[_7kaaWorker.spy_recno]->true_nation_recno
        == _7kaaSpy->true_nation_recno
    ) {
      attackRating += spy_array[_7kaaWorker.spy_recno]->spy_skill / 4;
    } else {
      defenderCount++;
      apparentDefenceRating += 4 + _7kaaWorker.hit_points / 30;
      possibleExtraDefenceRating += 50 - (4 + _7kaaWorker.hit_points / 30);
    }
  }

  if (target->rank_id == RANK_KING) {
    apparentDefenceRating += 50;
  } else {
    // Target can be a spy and have up to 100 spy skill.
    possibleExtraDefenceRating += 100;
  }

  /* The spy always want to not get caught. */
  if (defenderCount > 0) {
    apparentDefenceRating += 80;
  }

  const auto clearance = attackRating - apparentDefenceRating;

  auto chance = 0.0;
  if (clearance < -30) {
    chance = 0;//return 0;
  } else {
    chance = 0.7 * std::min(1.0, (clearance + 30) / 30.0);
  }

  if (clearance > 0) {
    const auto estimatedDefence
      = apparentDefenceRating
      + possibleExtraDefenceRating * 2 / (1 + confidenceFactor(_7kaaSpy));
    chance
      += 0.3 * std::sqrt(clearance / (estimatedDefence - apparentDefenceRating));
  }

  return chance;
}

int bribeAmountEstimate(
  const ::Spy* _7kaaSpy,
  Worker& target
) {
  constexpr auto AIM_FOR_CHANCE = 100;

  return bribeAmountEstimate(
    _7kaaSpy,
    AIM_FOR_CHANCE,
    target.loyalty(),
    target.race_id,
    0
  );
}
int bribeAmountEstimate(
  const ::Spy* _7kaaSpy,
  ::Unit* target
) {
  constexpr auto AIM_FOR_CHANCE = 100;

  return bribeAmountEstimate(
    _7kaaSpy,
    AIM_FOR_CHANCE,
    target->loyalty,
    target->race_id,
    target->commander_power()
  );
}

double stealReportEspaceChanceEstimate(
  const ::Spy* _7kaaSpy,
  const char report
) {
  constexpr int REPORT_REQUIRED_SKILLS[] = { 40, 20, 30, 30, 50, 40, 90 };

  const auto skillMargin
    = _7kaaSpy->spy_skill - REPORT_REQUIRED_SKILLS[report];

  if (skillMargin < 30) {
    return 0;
  }

  const auto estimatedEscapeChance
    = 1.0
    - (1.0
      / (floor(skillMargin / 15) * confidenceFactor(_7kaaSpy))
    );

  return std::clamp(estimatedEscapeChance, 0.0, 1.0);
}

} // namespace Ambition::Spy
