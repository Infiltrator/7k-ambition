/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2026 Tim Sviridov
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
 * Header file for Ambition::ErrorHandling.
 */

#pragma once

#include <format>
#include <stdexcept>
#include <string_view>


namespace Ambition::ErrorHandling {

namespace Exceptions {

class newer_type_version : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;

  newer_type_version(
    std::string_view typeName,
    const int archiveVersion,
    const int currentVersion
  )
    : std::runtime_error::runtime_error(
      std::format(
        "{} archive version {} is newer than the current version {}.",
        typeName,
        archiveVersion,
        currentVersion
      )
    )
  { }
};

} // namespace Ambition::ErrorHandling::Exceptions

} // namespace Ambition::ErrorHandling
