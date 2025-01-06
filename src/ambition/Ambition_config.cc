/**
 * \file Ambition_config.cc
 *
 * Implementation file for Ambition::Config.
 */

#include "Ambition_config.hh"

#include <stdexcept>

namespace Ambition {

Config ambitionConfig;

std::string
Config::modeString(
) const {
  switch (_mode) {
  case Mode::Classic:
	 return "classic";
  case Mode::Enhanced:
	 return "enhanced";
  case Mode::Advanced:
	 return "advanced";
  case Mode::Ambition:
	 return "ambition";
  }
}

bool
Config::compatibleWith7kaa(
) const {
  return _mode <= Mode::Enhanced;
};

bool
Config::enhancementsAvailable(
) const {
  return _mode >= Mode::Enhanced;
}

void Config::setMode(
  const Mode mode
) {
  // if(!currentState.allowsModeTransitions()) {
  // 	 throw std::domain_error("The current state does not allow mode transitions.");
  // }

  _mode = mode;
}

} // namespace Ambition
