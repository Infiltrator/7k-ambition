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
 * Header file for a C++11 version of std::format.
 *
 * Code taken from https://stackoverflow.com/a/26221725.
 */

#pragma once

#include <memory>
#include <stdexcept>
#include <string>


template<typename... Args>
std::string format(
  const std::string& formatString,
  Args... args
) {
  // Get the size of the formatted output string with an extra space for the
  // null terminator.
  const auto size_s
    = std::snprintf(nullptr, 0, formatString.c_str(), args...) + 1;
  if (size_s <= 0) {
    // TODO: It's not ideal to be throwing here, as we want to use it for
    // formatting exception messages.  However, when would this actually fail?
    throw std::runtime_error("Error during formatting.");
  }
  const auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, formatString.c_str(), args...);
  // We don't want the null terminator inside the std::string.
  return std::string(buf.get(), buf.get() + size - 1);
}
