#include "Ambition_news.hh"

#include "gettext.h"


namespace Ambition::News {

constexpr auto HEADING_ADDED = "Added";
constexpr auto HEADING_FIXED = "Fixed";
constexpr auto HEADING_CHANGED = "Changed";
constexpr auto HEADING_REMOVED = "Removed";
constexpr auto IDENTIFIER_PREVIEW = "Preview Version -- not yet released";


std::vector<VersionUpdates> versionDetails = {
  {
    .identifier = _(IDENTIFIER_PREVIEW),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
        },
      },
    },
  },
  {
    .identifier = _("3.1.5 -- 2025-05-03"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Show building construction costs on buttons."),
          _("Show war machine and ship construction costs on War Factory and Harbour build menus."),
          _("Show total production queue count and cost for War Factories and Harbours."),
          _("Show keybinds at the bottom-left of buttons that have them."),
          _("Add a keybind for cancelling menus (defaults to Esc), settable in config.txt with `bindkey = KEYEVENT_ _CANCEL,Escape`."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Show Spy combat (in Forts) or primary skill (in other Firms) level in the worker list by moving the spy icon to the portrait."),
          _("Added thousands seperators to treasury and food store amounts, to make reading them easier.  (Currently, this uses groups of three with a comma as the seperator for everyone.  Proper localisation will be added later.)"),
          _("Show the war machine technology level on the portrait, instead of as part of the name, on the War Factory build list.  This frees up some room for more information."),
        },
      },
    },
  },
  {
    .identifier = _("3.1.4 -- 2025-04-28"),
    .sections = {
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed spy change nation colours not working properly and corruption with buttons on info pane.  (Temporarly removed the leadership panel until a better solution is implemented.)"),
        },
      },
    },
  },
  {
    .identifier = _("3.1.3 -- 2025-04-27"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added clipboard keys (Ctrl+C, Ctrl+X, Ctrl+V) to input fields, such as the address of the \"Enter Address\" multiplayer option, map ID, and name fields."),
          _("Allow prepending to (adding to to the start of) production queues with Ctrl+click, allowing quickly producing a unit without clearing the entire queue."),
          _("Automatically copy the map ID to the clipboard when opening the in-game menu, allowing for easy copy-pasting of the map ID."),
          _("Automatically paste into the join address field, allowing for easy joining of games when using the \"Enter Address\" option."),
          _("Added a What's New screen, shown on the first startup of each new version."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed setting rally point from a Town using shift also pulling peasants."),
          _("Fixed pixel artefacts on the bottom of credits and results screens."),
          _("Fixed pixel artefact on text update."),
          _("Fixed Alt+right-click not pulling peasants from a Town in Classic mode."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.2 -- 2025-04-20"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Show an icon for units which are outside of their general's command range and so do not receive combat bonuses and are losing loyalty."),
          _("Display unit leadership effect status in the info pane."),
          _("Make War Factory production queue unlimited, up from 20."),
          _("Make Harbour production queue unlimited, up from 10."),
          _("Show the production queue size for Towns, Harbours, and War Factories over the production button, in a manner similar to the auto-tax."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed desync when cancelling training in a Town."),
          _("Fixed alignment of town goods satisfaction, inn guest count, and worker list."),
          _("Fixed Town training queue counts not showing correctly after loading a game with a 7kaa training queue."),
          _("Fixed not being able to cancel production after loading a 7kaa save game with a unit already in production."),
          _("Fixed not being able to cancel units in the queue after loading a 7kaa save game with an existing production queue."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Display the type and level of war machines inside forts, for easier identification."),
          _("Harbour production queue persists even when the harbour is full (4 ships) instead of clearing the queue."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.1 -- 2025-04-12"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Rally points can be toggled by setting the rally point to the same place again, in addition to alt-right-clicking on the building again."),
          _("Waypoints (and rally points) can now be set (and unset) with shift-right-click in addition to alt-right-click, to be more intuitive to people."),
          _("Added a rally point button on the info pane for buildings, which shows when a rally point is active.  Clicking on it goes to the rally location and right-clicking on it clears the rally point."),
          _("Added T (actually the same as the unit settle key) as a key to clear the rally point of a building."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed positioning of the colour box on the info pane for buildings and units."),
          _("Fixed porcupines and other not-attacking units not following waypoints."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.0 -- 2025-04-06"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added rally points for War Factory war machines."),
          _("Added rally points for Town peasants and trained units."),
          _("Added rally points for Inn hired units."),
          _("Added rally points for Camp units when sortieing."),
          _("Added rally points for all firms when mobilising workers."),
          _("Added rally points for Harbours when sailing ships."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed a crash on requesting a builder when one is on a ship."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Make Town training queue unlimited, up from 10."),
          _("Use global research target for all new and newly idle Towers of Science, not just the ones that were present when first clicking."),
          _("Waypoints can now be set to attack and enter buildings instead of just moving to a destination."),
          _("Waypoints now no longer get cleared if the unit cannot reach one destination, allowing for exploration without interruption."),
          _("Rally points can also be set to attack and enter buildings instead of just moving to a destination, the same as waypoints."),
          _("Rally points are now visible over unexplored blackness."),
          _("Setting a rally point on a building will not pull peasants to it."),
        },
      },
    },
  },
  {
    .identifier = _("v3.0.4 -- 2025-03-16"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added delete button to load game menu."),
          _("Show a progress bar on the map for town training."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed requested builder not being able to reach the destination building if there is no walkable path."),
          _("Fixed Inn hire button being enabled/disabled incorrectly."),
          _("Fixed hitbars having extra pixels on the right when a worker enters or leaves the building."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Continue loading config.txt even when there are invalid lines, so that the valid ones still apply.  Then, once finished loading, display all the line numbers which contained errors, allowing fixing them."),
        },
      },
    },
  },
  {
    .identifier = _("v3.0.3 -- 2025-03-07"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Show a progress bar for Harbours.  (They were missed when Towers of Science and War Factories were done.)"),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed the town recruitment progress bar overlapping the buttons when a spy is present and the spy button is showing."),
        },
      },
    },
  },
  {
    .identifier = _("v3.0.2 -- 2025-03-04"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Show town goods demand satisfaction (quality of life) to know how much goods are demanded and how much are being supplied--and thus the level of the bonus."),
          _("Show a progress bar for Towers of Science and War Factories."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed the unit path line (and building connection lines) animation speed breaking on game speed unlimited (9).  This should also fix the issue some users were having where moving the mouse would speed up the line animation."),
          _("Fixed the building hitbars looking kind of off and tilted."),
          _("Fix high CPU usage on showing help on hover."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Scale unit hitbar widths with the (square root of) their maximum hitpoints, to give a better idea of units' toughness at a glance."),
          _("Add more hitbar colours (dark green, orange, red) to make judging unit tougness easier."),
          _("Draw town/building link lines consistently in the same direction, regardless of which side is selected.  Since some links make sense only as bi-directional flow, these links are drawn alternating in both direction."),
          _("Darkened the bottom-left pixel of the hitbar outline to make it look more in place."),
        },
      },
    },
  },
  {
    .identifier = _("v3.0.1 -- 2025-02-25"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added game speed display to top-right of the map and \"PAUSED\" in the middle."),
          _("Show the guest count in an inn, to make it easy to know whether scrolling is even required."),
          _("Show the amount that non-spy units have been rewarded, so that it's easier to tell the effects of the contribution on loyalty."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed high CPU usage when moving the mouse cursor in-game.  This should also fix the unit path line looking like it's moving when the mouse cursor is moved."),
          _("Fixed minimap bounding box colours."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Display inn guest list from newest at the top to oldest at the bottom, so that it's quicker to check inns for new mercenaries."),
          _("Preferentially use builders from undamaged buildings further away instead of from closer buildings that are still not fully repaired."),
          _("Use all of the available space in the building info pane to show wider unit hitbars, so that they're easier to comprehend."),
          _("Show the missing portion of unit hitbars in the building info pane, so that it's easier to tell when a unit is missing hitpoints."),
          _("Draw a hitbar for the general/king in a camp, so that you can see his health."),
          _("Properly centre units in the building info pane, so that the hitbars are clearly discernible."),
          _("Use a four-slot screen for the mode selection.  This gives more space to expand the description text."),
          _("Use roman numerals for numbering modes on the mode selection screen.  This aesthetic is more appropriate."),
          _("Make mode selection screen translatable.  No translations have been completed yet, though."),
        },
      },
    },
  },
  {
    .identifier = _("v3.0.0 -- 2025-02-21"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Display the current running mode in top-right of the main menu."),
          _("Added a rudimentary running mode switcher accessible from a button at the top-right of the main menu."),
          _("Made the game look smoother by running at at a graphical 60 FPS."),
          _("Added cyan highlight to selected units, making for easier identification."),
          _("Added coloured outline to hit bars, making it easier to distinguish friendly units' hit bars from enemies'."),
          _("Show a hitbar for firms when they are at less than 100% HP."),
          _("Added builder icon to buildings with builders in them, allowing easy at-a-glance identification of them."),
          _("Made the game look smoother by constantly animating buildings at a fixed rate."),
          _("Made the game look smoother by constantly animating water and other terrain at a fixed rate."),
          _("Made the game look smoother by having the water be more sloshy and run approximately into the south-western direction as the waves imply."),
          _("Made the game look smoother by continuing rain and snow while game is paused."),
          _("Display more details when a multiplayer sync error occurs to help hunt down and fix the cause."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed the 100% CPU usage all the time problem."),
          _("Fixed scrolling to the right and down not working properly."),
          _("Fixed the unit path line flow direction to go from the unit to its destination instead of backwards."),
          _("Centred the info pane (the one below the minimap) so that it's no longer off-centre."),
          _("Fixed a memory leak when loading a saved game."),
          _("Fixed a memory leak with unit waypoints."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Use yellow colour for highlighting units on hover instead of cyan."),
          _("Made it easier to see when units are low on hitpoints by making the unit hitbars wider."),
          _("Change right and bottom cursor boundaries to allow the cursor to go further."),
          _("Made development slightly easier."),
        },
      },
    },
  },
};

} // namespace Ambition::News
