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
 * Implementation file for Ambition::Control.
 */

#include "Ambition_control.hh"

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <SDL2/SDL_misc.h>

#define _AMBITION_IMPLEMENTATION
#include "gettext.h"
#include "OBOX.h"
#include "OCONFIG.h"
#include "OMUSIC.h"
#include "ONATION.h"
#include "OSYS.h"

#include "Ambition_config.hh"
#include "Ambition_repository.hh"
#include "Ambition_user_interface.hh"


namespace Ambition {

namespace Control {

std::chrono::time_point<std::chrono::system_clock> lastFeedbackRequestTime(
);

void saveFeedbackRequestTime(
);

} // namespace Ambition::Control


void requestFeedback(
) {
  const auto daysSinceLastRequest = std::chrono::duration_cast<std::chrono::days>(
    std::chrono::system_clock::now() - Control::lastFeedbackRequestTime()
  );
  if (daysSinceLastRequest < std::chrono::days(30)) {
    return;
  }

  Control::saveFeedbackRequestTime();

  const auto saveSignalExitFlag = sys.signal_exit_flag;
  sys.signal_exit_flag = 0;

  if (box.ask(
      _("We hope that you have enjoyed playing Seven Kingdoms: Ambition!"
        "\nWe would appreciate your feedback on the game and to hear what else"
        " you would like to see in the game."
        "\nWould you be willing to share your feedback now?  (Will open in a"
        " web browser.)"
      ),
      _("I want to be heard!"),
      _("Later")
    )
  ) {
    Control::openFeedback();
  }

  sys.signal_exit_flag = saveSignalExitFlag;
}

void resetGameState(
) {
  entityRepository.reset();
}


namespace Control {

constexpr auto LAST_FEEDBACK_REQUEST_FILENAME = "last-feedback-request";


void migrateLocalDataDirectories(
) {
  constexpr std::array CONFIG_FILES = {
    "CONFIG.DAT",
    "config.txt",
    "cookies.txt",
    "HALLFAME.DAT",
    "last-displayed-news-version",
    "PLAYSTAT.DAT",
    "sdl.txt",
  };

  std::filesystem::create_directories(DirectoryPath::config());
  std::filesystem::create_directories(DirectoryPath::multiplayerSave());
  std::filesystem::create_directories(DirectoryPath::screenshot());
  std::filesystem::create_directories(DirectoryPath::singleplayerSave());

  const std::filesystem::path basePath = sys.dir_config;

  for (const auto filename : CONFIG_FILES) {
    const auto path = basePath / filename;
    if (std::filesystem::exists(path)) {
      std::filesystem::rename(path, DirectoryPath::config() / filename);
    }
  }

  for (const auto file : std::filesystem::directory_iterator(basePath)) {
    if (!file.is_regular_file()) {
      continue;
    }

    const auto extension = file.path().extension();

    if (extension == ".SAV") {
      std::filesystem::rename(
        file,
        DirectoryPath::singleplayerSave() / file.path().filename()
      );
    } else if (extension == ".RPL" || extension == ".SVM") {
      std::filesystem::rename(
        file,
        DirectoryPath::multiplayerSave() / file.path().filename()
      );
    } else if (extension == ".BMP") {
      std::filesystem::rename(
        file,
        DirectoryPath::screenshot() / file.path().filename()
      );
    }
  }
}

void openDiscord(
) {
  constexpr auto DISCORD_INVITATION_URL = "https://discord.gg/xJs99xK38G";

  const auto sdlReturnCode = SDL_OpenURL(DISCORD_INVITATION_URL);
}

void openFeedback(
) {
  constexpr auto FEEDBACK_URL
    = "https://sourceforge.net/p/seven-kingdoms-ambition/wiki/Post-game%20Feedback/";

  const auto sdlReturnCode = SDL_OpenURL(FEEDBACK_URL);
}

void startMusic(
) {
  if (::config.music_flag) {
    if (!music.is_playing(1)) {
      music.play(1, sys.cdrom_drive ? MUSIC_CD_THEN_WAV : 0 );
    }
  }
}


/* Private functions. */

std::chrono::time_point<std::chrono::system_clock> lastFeedbackRequestTime(
) {
  std::ifstream file(
    DirectoryPath::config() / LAST_FEEDBACK_REQUEST_FILENAME,
    std::ios::binary
  );
  if (!file.good()) {
    return std::chrono::time_point<std::chrono::system_clock>(
      std::chrono::system_clock::duration { 0 }
    );
  }

  std::chrono::time_point<std::chrono::system_clock>::rep storedValue;
  if (!file.read(reinterpret_cast<char*>(&storedValue), sizeof(storedValue))) {
    return std::chrono::time_point<std::chrono::system_clock>(
      std::chrono::system_clock::duration { 0 }
    );
  }

  return std::chrono::time_point<std::chrono::system_clock>(
    std::chrono::system_clock::duration { storedValue }
  );
}

void saveFeedbackRequestTime(
) {
  std::ofstream file(
    DirectoryPath::config() / LAST_FEEDBACK_REQUEST_FILENAME,
    std::ios::binary
  );
  if (!file.good()) {
    return;
  }

  const auto now = std::chrono::system_clock::now().time_since_epoch().count();
  file.write(reinterpret_cast<const char*>(&now), sizeof(now));
}

} // namespace Ambition::Control

} // namespace Ambition
