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
 * Header file for Ambition::Repository.
 */

#pragma once

#include <map>
#include <memory>
#include <type_traits>


namespace Ambition {

class Entity;

class Repository {
public:
  template <typename T, typename Func>
  std::shared_ptr<
    typename std::enable_if<std::is_base_of<Entity, T>::value, T>::type
  >
  findEntityBy(
    Func match
  ) {
    for (auto mapElement : records) {
      auto record = mapElement.second;
      auto entity = std::dynamic_pointer_cast<T>(record.entity);
      if (entity && match(entity)) {
        record.accessTimestamp = timestamp();
        return entity;
      }
    }

    return nullptr;
  }

  template <typename T>
  std::shared_ptr<
    typename std::enable_if<std::is_base_of<Entity, T>::value, T>::type
  >
  get(
    unsigned long long int recordNumber
  ) const {
    return std::dynamic_pointer_cast<T>(_get(recordNumber));
  }

  template <typename T>
  std::shared_ptr<
    typename std::enable_if<std::is_base_of<Entity, T>::value, T>::type
  >
  insert(
    std::shared_ptr<T> entity
  ) {
    _insert(entity);
    return entity;
  }

  void reset(
  );

  unsigned long long int takeRecordNumber(
  );

private:
  static const auto STARTING_RECORD_NUMBER = 1;

  friend void write(
    const std::string filename
  );

  struct Record;

  std::map<unsigned long long int, Record> records;
  unsigned long long int nextRecordNumber {STARTING_RECORD_NUMBER};

  std::shared_ptr<Entity> _get(
    unsigned long long int recordNumber
  ) const;

  void _insert(
    std::shared_ptr<Entity> entity
  );

  unsigned long long int timestamp(
  ) const;

  struct Record {
    unsigned long long int accessTimestamp;
    std::shared_ptr<Entity> entity;
    const unsigned long long int recordNumber;

    enum class State {
      Nonexistent,
      Loading,
      Loaded,
      Offloaded,
    };
    State state;
  };
};

extern Repository entityRepository;

} // namespace Ambition
