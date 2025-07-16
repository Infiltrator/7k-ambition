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

#include <SDL2/SDL_misc.h>

#include "gettext.h"
#include "OBOX.h"
#include "OSYS.h"

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
