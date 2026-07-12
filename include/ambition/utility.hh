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
 * Header file for C++20 versions of std functions and/or std functions that
 * should exist.
 *
 * Largely adapted from https://en.cppreference.com/w/cpp.html.
 */

#pragma once

#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <istream>
#include <ranges>
#include <string>


namespace Utility {

template<class Range, class Mapper>
using map_result_t = std::invoke_result_t<
  Mapper&,
  std::ranges::range_reference_t<std::remove_reference_t<Range>>
>;

template<
  template<class...> class Sequence,
  class Range,
  class Mapper,
  class Element = std::remove_cvref_t<map_result_t<Range, Mapper>>
>
concept MappableInto = std::ranges::output_range<
  Sequence<Element>,
  std::invoke_result_t<
    Mapper&,
    std::ranges::range_reference_t<std::remove_reference_t<Range>>
  >
>;

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
  Range&& range,
  const Element& value,
  Projection projection = {}
) {
  return std::ranges::find(
    std::ranges::begin(range),
    std::ranges::end(range),
    value,
    projection
  ) != std::ranges::end(range);
}

template<
  typename _CharT,
  typename _Traits
>
inline std::string
getline(
  std::basic_istream<_CharT, _Traits>& istream
) {
  std::string line;
  std::getline(istream, line);
  return line;
}

template <
  template<class...> class Sequence,
  std::ranges::viewable_range Range,
  class Mapper = std::identity
>
requires
  std::default_initializable<
    Sequence<std::remove_cvref_t<map_result_t<Range, Mapper>>>
  > && MappableInto<Sequence, Range, Mapper>
constexpr auto
map_to(
  Range&& range,
  Mapper mapper = {}
) {
  using Element = std::remove_cvref_t<map_result_t<Range, Mapper>>;
  Sequence<Element> result;

  if constexpr (std::ranges::sized_range<std::remove_reference_t<Range>>
    && requires(Sequence<Element>& r, decltype(std::ranges::size(range)) n) {
      r.reserve(n);
    }
  ) {
    result.reserve(std::ranges::size(range));
  }

  std::ranges::transform(
    std::forward<Range>(range),
    std::back_inserter(result),
    mapper
  );
  return result;
}

template<
  typename InputRangeT,
  typename DelimiterT
>
inline std::vector<InputRangeT>
split(
  const InputRangeT& input,
  const DelimiterT& delimiter,
  const boost::algorithm::token_compress_mode_type eCompress
    = boost::algorithm::token_compress_mode_type::token_compress_off
) {
  std::vector<InputRangeT> result;
  boost::split(
    result,
    input,
    [&delimiter](auto& e) {
      return e == delimiter;
    },
    eCompress
  );
  return result;
}

template<typename SequenceT>
inline SequenceT
trim(
  const SequenceT& input,
  const std::locale& locale = std::locale()
) {
  SequenceT copy(input);
  boost::trim(copy);
  return copy;
}

} // namespace Utility
