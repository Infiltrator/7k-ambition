/*
 * Seven Kingdoms: Ambition
 *
 * Copyright 2025–2026 Tim Sviridov
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
 * Header file for Ambition::Serialisation.
 */

#pragma once

#include <boost/serialization/version.hpp>
#include <string>

#include "Ambition_error_handling.hh"

class GameFile;


namespace Ambition {

namespace Serialisation {

template <typename Object>
inline void enforceVersion(
  Object* object,
  std::string_view typeName,
  const unsigned int archiveVersion
) {
  const auto currentVersion
    = boost::serialization::version<std::remove_cvref_t<decltype(*object)>>
      ::value;
  if (archiveVersion > currentVersion) {
    throw ErrorHandling::Exceptions::newer_type_version(
      typeName,
      archiveVersion,
      currentVersion
    );
  }
}

} // namespace Ambition::Serialisation


struct SavefileInformation {
  template<class Archive>
  void serialize(
    Archive& archive,
    const unsigned int version
  ) {
    Serialisation::enforceVersion(this, "SavefileInformation", version);
  }
};


void read(
  const std::string filename,
  const long startingPosition
);

void write(
  const std::string filename
);

namespace Serialisation {

std::string calculateFileDateString(
  const std::string _7kaaCalculation,
  const GameFile* saveFile
);

} // namespace Ambition::Serialisation

} // namespace Ambition

BOOST_CLASS_VERSION(Ambition::SavefileInformation, 0)
