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
#include <filesystem>
#include <SDL2/SDL_misc.h>

#include "gettext.h"
#include "OBOX.h"
#include "OSYS.h"

#include "Ambition_config.hh"
#include "Ambition_repository.hh"


namespace Ambition {

void requestFeedback(
) {
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

void migrateLocalDataDirectories(
) {
  constexpr std::array CONFIG_FILES = {
    "CONFIG.DAT",
    "config.txt",
    "cookies.txt",
    "HALLFAME.DAT",
    "last-displayed-news-version",
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

} // namespace Ambition::Control

} // namespace Ambition
