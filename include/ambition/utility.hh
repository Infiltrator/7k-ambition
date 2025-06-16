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
 * Header file for C++20 versions of std functions.
 *
 * Largely adapted from https://en.cppreference.com/w/cpp.html.
 */

#pragma once

#include <ranges>


namespace Utility {

template <
  std::ranges::input_range Range,
  class Projection = std::identity,
  class Element = std::projected<Range, Projection>
>
requires std::indirect_binary_predicate<
  std::ranges::equal_to,
  std::projected<std::ranges::iterator_t<Range>, Projection>,
  const Element*
>
constexpr bool
contains(
  Range range,
  const Element& value,
  Projection projection = {}
) {
  return std::ranges::find(
    std::move(std::ranges::begin(range)),
    std::ranges::end(range),
    value,
    projection
  ) != std::ranges::end(range);
}

} // namespace Utility
