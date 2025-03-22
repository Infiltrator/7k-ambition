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
 * Implementation file for _7kaaAmbitionInterface::Polity.
 */

#define _AMBITION_IMPLEMENTATION
#include "polity.hh"

#include "ONATION.h"
#include "ONATIONB.h"

#include "Ambition_config.hh"
#include "Ambition_polity.hh"


namespace _7kaaAmbitionInterface::Polity {

void dissolve(
  const NationBase* _7kaaNation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto _7kaaRecordNumber = _7kaaNation->nation_recno;
  auto polity = Ambition::Polity::findBy7kaaRecordNumber(_7kaaRecordNumber);
  if (polity) {
    polity->dissolve(Ambition::Time::now());
  }
}

void setResearchTarget(
  const Nation* _7kaaNation,
  const int technologyId
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return;
  }

  const auto _7kaaRecordNumber = _7kaaNation->nation_recno;
  auto polity = Ambition::Polity::getBy7kaaRecordNumber(_7kaaRecordNumber);
  if (polity) {
    polity->setResearchTarget(technologyId);
  }
}

} // namespace _7kaaAmbitionInterface::Polity
