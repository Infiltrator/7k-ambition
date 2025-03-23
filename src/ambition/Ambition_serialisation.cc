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
 * Implementation file for Ambition::Serialisation.
 */

#include "Ambition_serialisation.hh"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>

#include "gettext.h"
#include "OBOX.h"

#include "Ambition_building.hh"
#include "Ambition_entity.hh"
#include "Ambition_repository.hh"
#include "Ambition_version.hh"
#include "format.hh"


namespace Ambition {

constexpr auto BOOKMARK = "0xFAB0";
constexpr auto HEADER_START = "[Ambition_header]";
constexpr auto SAVEFILE_VERSION = 0;

enum HeaderFlags : uint64_t {
  Compressed = 1 << 0,
  BoostXml = 1 << 1,
};


void read(
  const std::string filename,
  const long startingPosition
) {
  std::ifstream saveFile(filename);
  assert(saveFile.good());

  saveFile.seekg(startingPosition);

  std::string rollingBuffer;
  auto bookmarkPosition = std::string::npos;

  /* 7kaa either does not read to the end of the file, or else does not report
   * the correct position when it has read to the end.  Either way, we need to
   * seek forward from where 7kaa says. */
  constexpr auto WINDOW_SIZE = 6;

  do {
    const auto substringPosition
      = std::max(0, int(rollingBuffer.size()) - WINDOW_SIZE);
    rollingBuffer = rollingBuffer.substr(substringPosition);

    std::string buffer;
    saveFile >> std::setw(WINDOW_SIZE) >> buffer;
    rollingBuffer.append(buffer);

    bookmarkPosition = rollingBuffer.find(BOOKMARK);
  } while (!saveFile.eof() && bookmarkPosition == std::string::npos);

  /* Seek backwards if we have overshot. */
  if (bookmarkPosition != std::string::npos) {
    saveFile.seekg(
      bookmarkPosition + WINDOW_SIZE - rollingBuffer.size(),
      std::ios_base::cur
    );
  }

  saveFile >> rollingBuffer;
  if (rollingBuffer != HEADER_START) {
    return;
  }

  uint64_t flags = 0;
  saveFile >> flags;

  auto savefileVersion = SAVEFILE_VERSION;
  saveFile >> savefileVersion;

  saveFile >> rollingBuffer;
  const auto lastSavingVersion = rollingBuffer;

  if (savefileVersion > SAVEFILE_VERSION) {
    box.msg(
      format(
        _("Save game version is too new."
          "\nThe game will load, but all Ambition features will be reset."
          "\nSaved by: v%s."
          "\nCurrent: v%s."
        ),
        lastSavingVersion.c_str(),
        versionString().c_str()
      ).c_str(),
      0
    );
    return;
  }

  boost::archive::xml_iarchive archive(saveFile);
  archive.register_type<SavefileInformation>();
  archive.register_type<Entity>();
  archive.register_type<Building>();

  SavefileInformation savefileInformation;
  archive >> BOOST_SERIALIZATION_NVP(savefileInformation);

  size_t recordCount;
  archive >> BOOST_SERIALIZATION_NVP(recordCount);
  entityRepository.reset();

  for (auto i = 0; i < recordCount; i++) {
    Entity* entity;
    archive >> BOOST_SERIALIZATION_NVP(entity);
    entityRepository.insert(std::shared_ptr<Entity>(entity));
  }
}

void write(
  const std::string filename
) {
  uint64_t flags = 0;
  flags |= HeaderFlags::BoostXml;

  std::ofstream saveFile(filename, std::ios::app);
  assert(saveFile.good());

  saveFile << BOOKMARK << std::endl;
  saveFile << HEADER_START << std::endl;
  saveFile << flags << std::endl;
  saveFile << SAVEFILE_VERSION << std::endl;
  saveFile << versionString() << std::endl;

  boost::archive::xml_oarchive archive(saveFile);
  archive.register_type<SavefileInformation>();
  archive.register_type<Entity>();
  archive.register_type<Building>();

  SavefileInformation savefileInformation;
  archive << BOOST_SERIALIZATION_NVP(savefileInformation);

  const auto recordCount = entityRepository.records.size();
  archive << BOOST_SERIALIZATION_NVP(recordCount);

  for (const auto mapPair : entityRepository.records) {
    const auto entity = mapPair.second.entity.get();
    archive << BOOST_SERIALIZATION_NVP(entity);
  }
}

} // namespace Ambition
