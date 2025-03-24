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
 * Implementation file for Ambition::Repository.
 */

#include "Ambition_repository.hh"

#include <limits>
#include <stdexcept>

#include "OSYS.h"

#include "Ambition_entity.hh"
#include "format.hh"


namespace Ambition {

Repository entityRepository;

void Repository::reset(
) {
  records.clear();
  nextRecordNumber = STARTING_RECORD_NUMBER;
}

unsigned long long int Repository::takeRecordNumber(
) {
  if (nextRecordNumber == std::numeric_limits<unsigned long long int>::max()) {
    throw std::range_error("(Somehow) ran out of record numbers.");
  }

  return nextRecordNumber++;
}


/* Private functions. */
std::shared_ptr<Entity> Repository::_get(
  unsigned long long int recordNumber
) const {
  auto result = records.find(recordNumber);

  if (result == records.end()) {
    throw std::out_of_range(
      format("Entity with given key %d does not exist.", recordNumber)
    );
  }

  auto record = result->second;
  if (record.state != Record::State::Loaded) {
    return nullptr;
  }

  record.accessTimestamp = timestamp();
  return record.entity;
}

void Repository::_insert(
  std::shared_ptr<Entity> entity
) {
  if (entity->recordNumber >= nextRecordNumber) {
    nextRecordNumber = entity->recordNumber + 1;
  }

  const auto result = records.insert(
    std::make_pair(
      entity->recordNumber,
      Record {
        .accessTimestamp = timestamp(),
        .entity = entity,
        .recordNumber = entity->recordNumber,
        .state = Record::State::Loaded,
      }
    )
  );

  if (!result.second) {
    throw std::runtime_error(
      format(
        "Unable to add entity with record number %d to repository.",
        entity->recordNumber
      )
    );
  }
}

unsigned long long int Repository::timestamp(
) const {
  // TODO: This will overflow, so we need to use a proper timestamp.
  return sys.frame_count;
}

} // namespace Ambition
