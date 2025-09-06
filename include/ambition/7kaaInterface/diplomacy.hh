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
 * Header file for _7kaaAmbitionInterface::Diplomacy.
 */

#pragma once

#include "OSTR.h"

class Nation;
class TalkMsg;
class TalkRes;


namespace _7kaaAmbitionInterface::Diplomacy {

String& getFoodPurchasePriceDescription(
  String& _7kaaCalculation,
  const TalkMsg* _7kaaDiplomaticMessage
);

bool setFoodPurchaseQuantities(
  TalkRes* _7kaaDiplomaticMessageResource,
  const Nation* source7kaaNation
);

bool setFoodPurchasePrices(
  TalkRes* _7kaaDiplomaticMessageResource,
  const Nation* source7kaaNation,
  const int quantity
);

bool setTributeQuantities(
  TalkRes* _7kaaDiplomaticMessageResource,
  const Nation* source7kaaNation
);

} // namespace _7kaaAmbitionInterface::Diplomacy

#ifndef _AMBITION_IMPLEMENTATION
/** Allow 7kaa to call using Ambition::*. */
namespace Ambition = _7kaaAmbitionInterface;
#endif
