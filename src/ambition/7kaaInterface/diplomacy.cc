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
 * Implementation file for _7kaaAmbitionInterface::Diplomacy.
 */

#define _AMBITION_IMPLEMENTATION
#include "diplomacy.hh"

#include <array>
#include <cassert>

#include "ONATION.h"
#include "OTALKRES.h"

#include "Ambition_config.hh"
#include "format.hh"


namespace _7kaaAmbitionInterface::Diplomacy {

constexpr auto STRING_LENGTH = 128;
std::array<char[STRING_LENGTH], MAX_TALK_CHOICE> choicesBuffer;
std::size_t choicesBufferIndex = 0;

void add7kaaTalkChoice(
  TalkRes* _7kaaDiplomaticMessageResource,
  const std::string string,
  const short parameter
);


bool setFoodPurchaseQuantities(
  TalkRes* _7kaaDiplomaticMessageResource,
  const Nation* source7kaaNation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  constexpr auto MINIMUM_PURCHASE_PRICE = 0.1;

  constexpr auto AMOUNTS = std::array {
    100,
    250,
    500,
    750,
    1'000,
    1'500,
    2'000,
    3'000,
    4'000,
  };

  for (const auto& amount : AMOUNTS) {
    if (source7kaaNation->cash < amount * MINIMUM_PURCHASE_PRICE) {
      break;
    }

    add7kaaTalkChoice(
      _7kaaDiplomaticMessageResource,
      format("%'d.", amount),
      amount
    );
  }
  return true;
}

bool setFoodPurchasePrices(
  TalkRes* _7kaaDiplomaticMessageResource,
  const Nation* source7kaaNation,
  const int quantity
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  constexpr auto _7KAA_PRICE_FACTOR = 10;

  constexpr auto PRICES = std::array {
    0.1,
    0.2,
    0.3,
    0.4,
    0.5,
    1.0,
    1.5,
    2.0,
    5.0,
  };

  for (const auto& price : PRICES) {
    if (source7kaaNation->cash < quantity * price) {
      break;
    }

    add7kaaTalkChoice(
      _7kaaDiplomaticMessageResource,
      format("$%.0f.", price * _7KAA_PRICE_FACTOR),
      price * _7KAA_PRICE_FACTOR
    );
  }
  return true;
}

bool setTributeQuantities(
  TalkRes* _7kaaDiplomaticMessageResource,
  const Nation* source7kaaNation
) {
  if (!Ambition::config.enhancementsAvailable()) {
    return false;
  }

  constexpr auto AMOUNTS = std::array {
    500,
    1'000,
    2'000,
    3'000,
    4'000,
    5'000,
    10'000,
    20'000,
    32'000,
  };

  for (const auto& amount : AMOUNTS) {
    assert (amount <= std::numeric_limits<short>::max());

    if (_7kaaDiplomaticMessageResource->cur_talk_msg.talk_id != TALK_DEMAND_TRIBUTE
      && _7kaaDiplomaticMessageResource->cur_talk_msg.talk_id != TALK_DEMAND_AID
      && source7kaaNation->cash < amount
    ) {
      break;
    }

    add7kaaTalkChoice(
      _7kaaDiplomaticMessageResource,
      format("$%'d.", amount),
      amount
    );
  }
  return true;
}


/* Private functions. */

void add7kaaTalkChoice(
  TalkRes* _7kaaDiplomaticMessageResource,
  const std::string string,
  const short parameter
) {
  assert(_7kaaDiplomaticMessageResource->talk_choice_count < MAX_TALK_CHOICE);

  strncpy(choicesBuffer[choicesBufferIndex], string.c_str(), STRING_LENGTH);

  _7kaaDiplomaticMessageResource->talk_choice_array[
    _7kaaDiplomaticMessageResource->talk_choice_count
  ].str  = choicesBuffer[choicesBufferIndex];
  _7kaaDiplomaticMessageResource->talk_choice_array[
    _7kaaDiplomaticMessageResource->talk_choice_count
  ].para = parameter;

  _7kaaDiplomaticMessageResource->talk_choice_count++;
  choicesBufferIndex = (choicesBufferIndex + 1) % MAX_TALK_CHOICE;
}

} // namespace _7kaaAmbitionInterface::Diplomacy
