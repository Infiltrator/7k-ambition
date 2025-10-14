#include "Ambition_news.hh"

#include "gettext.h"


namespace Ambition::News {

const auto HEADING_PREAMBLE = _("Preamble");
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
          _("Automatically detect the language on Windows."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Base the display of the building guide on the Help setting.  Not shown when set to None; only the grid lines when set to Brief; and both highlighting and grid lines when set to Detailed."),
        },
      },
    },
  },
  {
    .identifier = _("v3.2.6 -- 2025-10-08"),
    .sections = {
      {
        .heading = HEADING_PREAMBLE,
        .items = {
          _("The behaviour of the newly added information panels and the building hitbars (see the sections below) is controlled (for now*) by the Help level setting in the Options menu.  You are encouraged to tweak this setting to your liking."),
          _("When set to None, they will never be displayed."),
          _("When set to Brief, they will be displayed for the currently selected building and any building that is hovered over with the mouse."),
          _("When set to Detailed, they will be displayed for all buildings all the time."),
          _("* Ambition will have its own Settings menu with more fine-grained control over features in the future."),
        },
      },
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Show an information panel for Towns, with the number of peasants, total population, number of races, and loyalty or resistance."),
          _("Show an information panel for Forts, with the general or king leadership level, number of soldiers, and the approximate total strength, including the effects of the leader."),
          _("Show information panels for other buildings, with information relevant to them, such as worker and good counts."),
          _("Show an information panel for Fryhtan Lairs with their species name and level.  The level is an approximate relative reflection of the species' strength and the gold reward for killing Ordos.  Note that this reflects only the species, not the current unit count and combat level, which can only be seen with a Phoenix."),
          _("Added a progress bar for Seats of Power."),
          _("Greater Beings will now go to the rally points of their Seats of Power when they are summoned."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed a crash that sometimes happened after an earthquake, where automatic caravan cloning would try to both clone and retire an idle caravan at the same time, resulting in a segfault."),
          _("Fixed the build Fort keybind (F) not working because it was conflicting with selecting a Fort.  The default keybind for selecting a fort has been changed to `N`.  You can change by binding the `KEYEVENT_ _BUILD_ _CAMP` and `KEYEVENT_ _GOTO_ _CAMP` events to different keys in the config.txt file."),
          _("Fixed spy Steal Reports option not working in Towns."),
          _("Fixed translations not working on Windows and always showing English.  Now, the game will respect the config.txt `locale` setting on Windows."),
          _("Fixed cloned caravans sometimes appearing on another island or in an otherwise inaccessible location and not being able to follow its route."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Changed when building hitbars are displayed based on the help setting in the options menu.  When it is set to None, building hitbars will never be displayed.  When it is set to Brief, they will be displayed for the currently selected building and any building hovered over with the mouse.  When it is set to Detailed, they will be displayed as per the old behaviour--when the building is at less than 100% hitpoints."),
          _("Town and Firm information panels will show the number of spies you have inside of them instead of just \"(Spy)\".  If the general is also your spy, it will say \"G+\" the number of other spies you have."),
        },
      },
    },
  },
  {
    .identifier = _("v3.2.5 -- 2025-09-09"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added scrolling with the mouse wheel, up/down, and home/end keys to the Harbour ship list."),
          _("Added ship count and capacity display to Harbours on the info pane."),
          _("Added $100,000 throne purchase option."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed building menus (Town training, Harbour/War Factory build, spy menus, etc.) exiting back to the main menu when production of a new unit starts."),
          _("Fixed a crash when looking at the ship list on the trade report."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Start the kingdom score comparison with your own kingdom selected by default.  You can deselect it or select another kingdom by clicking the button."),
          _("Show the per-unit and total costs when choosing how to respond to food purchase requests."),
          _("Use thousands separators for throne purchase amounts options."),
        },
      },
    },
  },
  {
    .identifier = _("v3.2.4 -- 2025-09-03"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added kingdom-to-kingdom score comparison to the Rank Report, allowing you to see the scores of two kingdoms side-by-side."),
          _("Independent towns will show that their resistance is dropping to 0 when they are working for you."),
          _("Added scrolling with the mouse wheel, up/down, home/end, and pg up/down keys to the scenario selection list and description."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed the estimated assassination success chance not properly taking into account other defenders present, showing too high when other units were present."),
          _("Fixed the estimated assassination success chance against a king being too low."),
          _("Fixed the estimated assassination success chance being too low based on the spy skill."),
          _("Fixed the production queue sizes showing the wrong number when training a unit finishes."),
          _("Fixed the production queue size display bouncing down and up in multiplayer when a unit starts production."),
          _("Fixed the build Seat of Power keybind (P) not working because it was conflicting with toggling opaque report mode.  The default keybind for toggling opaque report mode has been cleared.  You can re-enable the keybind with the `KEYEVENT_ _REPORT_ _OPAQUE_ _TOGGLE` event, with a different key, in the config.txt file."),
          _("Fixed the war machine type and level identifiers inside forts causing artefacts when they moved around in the list."),
          _("Fixed possible crashes when pasting text containing characters unprintable by the game."),
          _("Fixed a bug where loading and unloading a scenario would cause multiplayer to stop working."),
          _("Fixed a crash where loading and unloading a scenario and then loading and unloading a scenario again would crash the game."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Shifted the bitmaps of Firms to better align with the squares they actually take up so that they look more on-centre, especially the Seats of Power."),
          _("Shifted the bitmaps of Fryhtan Lairs to better align with the squares they actually take up so that they look more on-centre, especially the Wyrm and Ick ones."),
          _("Changed the graphics of Standard tileset terrain that looked like it should block building and movement but didn't, and vice versa, so that it's now obvious by just looking at it."),
          _("Adjusted the graphics of Standard tileset terrain that looked like it should allow building and movement around its corners and edges but didn't, so that it's now obvious by just looking that it's not."),
          _("Reworked the assassination chance estimate calculation to better take into account the possibility or lack of enemy spies."),
          _("Disabled the caravan clone button (and remove it in the trade report) for idle caravans, since trying to clone them does nothing anyway."),
          _("Changed default keybind for train soldier to `F` from `W`, to make it more consistent with the other fort-related keybinds (build fort)."),
          _("Swapped the default keybinds for same kingdom/any kingdom object switching keybinds (`,`/`.` and `<`/`>`), to make it easier to do the more common task (switching between units of the same kingdom) without holding they shift key."),
          _("Adjusted the page scrolling on the save game list to keep one item on the page, so that it's evident that it's scrolling only one page at a time."),
          _("Break up the 7K:AA changelog entries into categories, thanks to a poster on Discord, to make it easier to read."),
        },
      },
    },
  },
  {
    .identifier = _("v3.2.3 -- 2025-08-24"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Show a Spy-suggested bribe amount on the bribe menu.  This amount varies in accuracy over time and based on the spy's skill.  But keep in mind that your spies do not know about enemy spies."),
          _("Added an assassination confirmation screen with a Spy-estimated success chance.  This estimate varies in accuracy over time and based on the spy's skill."),
          _("Added a steal report confirmation screen with a Spy-estimated escape chance.  This estimate varies in accuracy over time and based on the spy's skill."),
          _("Show the surplus (or deficit) number of peasants that you have on the Villages Report.  This is how many extra peasants you currently have over the amount you need to exactly balance food production and consumption."),
          _("Increased bribe choices to include $2500 and $3500."),
          _("Added more food purchasing quantity and price options."),
          _("Added more aid and tribute amount options."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed build guidelines not being clearly visible on Snow and Badlands tilesets."),
          _("Fixed some build guidelines appearing a different colour on top of highlighting."),
          _("Fixed the Villages Report yearly Firm costs, Military Report yearly Unit costs, and War Machine yearly costs showing up 10% higher than actual, now that the day accounting bug has been fixed and reduced these costs again."),
          _("Fixed save file names and dates running off on the right side of the load screen, creating artefacts.  This was especially problematic in some non-English languages where the text is wider or longer."),
          _("Fixed a crash when selling or demolishing an empty Fort which has a rally point."),
          _("Fixed Spy steal report escape chances not actually depending on the type of report stolen.  Now, different report types will have different chances of escape after stealing them."),
          _("Fixed save game list scrolling further than expected."),
          _("Fixed a bug where a ghost guest in an Inn could cause a segmentation fault crash."),
          _("Fixed dying units always showing the outside-leadership-range icon."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Made the build guidelines on the Standard tileset slightly brighter, to make them more visible."),
          _("Hide icons for units that are dying."),
          _("Lined up the villager and peasant numbers on the Villages Report."),
          _("Show the per-unit and total costs of food purchase requests."),
          _("Increased the game speed of the unlimited speed by not drawing every frame, but only 60 FPS."),
          _("Use thousands separators for food purchase quantities and aid and tribute amounts."),
        },
      },
    },
  },
  {
    .identifier = _("v3.2.2 -- 2025-08-18"),
    .sections = {
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("No game changes.  This just has packaging changes to work with Gentoo."),
        },
      },
    },
  },
  {
    .identifier = _("v3.2.1 -- 2025-08-12"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added keybinds and keybind display to Town training menu buttons.  The events for the config.txt file are: `KEYEVENT_ _TRAIN_ _BUILDER`, `KEYEVENT_ _TRAIN_ _SOLDIER`, `KEYEVENT_ _TRAIN_ _MINER`, `KEYEVENT_ _TRAIN_ _ARTISAN`, `KEYEVENT_ _TRAIN_ _RESEARCHER`, and `KEYEVENT_ _TRAIN_ _SPY`."),
          _("Added keybinds and keybind display to Spy menu buttons.  The events for the config.txt file are: `KEYEVENT_ _UNIT_ _REWARD`, `KEYEVENT_ _SPY_ _ASSASSINATE`, `KEYEVENT_ _SPY_ _BRIBE`, `KEYEVENT_ _SPY_ _CAPTURE`, `KEYEVENT_ _SPY_ _CHANGE_ _MISSION`, `KEYEVENT_ _SPY_ _MOBILISE`, `KEYEVENT_ _SPY_ _MENU`, and `KEYEVENT_ _SPY_ _STEAL_ _REPORTS`."),
          _("Added a keybind for mobilising all workers in a building.  This uses the existing event of `KEYEVENT_ _FIRM_ _PATROL` (defaults to `R`)."),
          _("Load 7kaa's config.txt file as well as Ambition's one, to reduce the need for duplication.  Settings in Ambition's config.txt override those is 7kaa's one."),
          _("Added Discord and feedback links to the main menu, in the bottom-left corner."),
          _("Added keybinds for starting a new single-player game (`KEYEVENT_ _CONFIRM`) and for going back (`KEYEVENT_ _CANCEL`)."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed leadership status artefacts on the info pane when the status changes."),
          _("Fixed spies seemingly always getting caught after stealing reports."),
          _("Fixed spies having a 100% escape chance when stealing reports."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Made the keybind displays appear transparent over their buttons, to obscure less of the underlying buttons."),
          _("Enabled default hotkeys for the build menu and disabled default `Q`, `W`, and `E` for selecting map modes.  The new default `E` now cycles through all of the map modes; and `W` is the default hotkey for building a War Factory.  This can all be overridden in the config.txt file."),
          _("Changed the default hotkeys for next/previous object to `.`, `,`, `shift+.`, and `shift+,`, so that they do not conflict with scrolling."),
          _("Reduced the brightness of the building guide lines from bright white, so that they're less jarring."),
          _("Only ask for feedback about once a month, instead of every time."),
          _("Treat config.txt lines which begin with `#ambition` as actual settings (ignoring the `#ambition` part and using the rest of the line) instead of comments.  This allows sharing of configs with 7kaa without worrying about Ambition settings not being recognised by 7kaa."),
          _("Split local data into multiple directories, to make finding what you need easier: `config`, `screenshot`, `save/singleplayer`, and `save/multiplayer`.  Existing files in the base directory will be migrated to their appropriate location."),
          _("Start playing music straight away on the What's New screen instead of only on the main menu, to give the right atmosphere.  The usual settings for music and volume are respected."),
          _("Tweak the wording of the feedback request pop-up buttons."),
        },
      },
    },
  },
  {
    .identifier = _("v3.2.0 -- 2025-08-10"),
    .sections = {
      {
        .heading = HEADING_PREAMBLE,
        .items = {
          _("This is a 7kaa-compatibility release.  7kaa recently released a new version (2.15.7), and this merges in those changes, and adds a few fixes."),
          _("See the below sections for more details."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed the oversized Town training buttons and the artefacts they caused on the info pane."),
          _("Fixed the file time display of old save game files."),
          _("Fixed incorrect and misplaced field labels for Workers in the info pane."),
          _("Fixed Inn scrolling not showing lower guests when there are more than 4 guests."),
          _("Fixed config.txt settings for `KEYEVENT_ _CANCEL` and `KEYEVENT_ _CONFIRM` not working."),
          _("Removed the clone button for ships on the Trade Report, which causes a crash and was not meant to be put in yet.  (Cloning ships is a planned featured to be added later.)"),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Updated 7kaa compatibility to the recently-released 7kaa 2.15.7 (see below for more details about the update).")
        },
      },
    }
  },
  {
    .identifier = _("7K:AA [2.15.7] -- 2025-07-25"),
    .sections = {
      {
        .heading = _("Improvements from Ambition"),
        .items = {
          _("Allow comments in config.txt (lines starting with `#`)."),
          _("Fixed requested builder sometimes being unable to reach destination."),
          _("Correct day counting."),
          _("Fixed high-potential soldiers gaining less leadership."),
          _("Fixed unit skill potential to persist, and not continually be randomised"),
          _("Fixed issues with skills being reset when entering buildings"),
          _("Improved some graphic drawing"),
        },
      },
      {
        .heading = _("Added (UI)"),
        .items = {
          _("Added support for a hotkey to center on the selected unit"),
          _("Added support to make numeric keys configurable"),
          _("Added advanced options for news notifications"),
          _("Added support for ctrl button in hotkey mapping"),
          _("Allow screenshots of in-game menu, to easily record map seed"),
          _("Added hotkey support to cancel build or settle (typically right mouse button)"),
        },
      },
      {
        .heading = _("Added (General)"),
        .items = {
          _("Extended game file and synchronization code to support extensions to the game"),
          _("Allowed the scenario settings to persist across save and load."),
        },
      },
      {
        .heading = _("Fixed (UI)"),
        .items = {
          _("Fixed mouse sticky/scrolling issues."),
          _("Fixed an issue selecting invisible units."),
        },
      },
      {
        .heading = _("Fixed (AI)"),
        .items = {
          _("Fixed poor ai logic that cause the ai to always accept a trade treaty"),
          _("Fixed poor ai logic concerning building weapons"),
          _("Fixed poor ai logic concerning hiring spies"),
          _("Fixed an error when the ai attacking an unit that enters a building"),
          _("Corrected utilization of base towns, typically an ai concept"),
          _("Improved ai to avoid more cases of accepting treaties of large rivals"),
          _("Fixed an issue that caused the ai to duplicate fort build actions"),
          _("Fixed more sea based ai actions"),
          _("Corrected more instances where the ai incorrect targets itself"),
          _("Fixed some ai capture code"),
          _("Fixed sea-based AI actions and pathing to board ships."),
        },
      },
      {
        .heading = _("Fixed (General)"),
        .items = {
          _("Fixed some in-game statistics"),
          _("Fixed some game file bugs"),
          _("Removed a bug causing a fake earthquake"),
          _("Removed a possible accidental disclosure of the presence of spies"),
          _("Fixed an error occuring sometimes when a town turns independent"),
          _("Fixed an error occuring sometimes on recruiting the last pop of a town"),
          _("Fixed a pathing issue"),
          _("Fixed issue that allowed resigning ships with units in ships"),
          _("Fixed memory issue in multiplayer"),
          _("Fixed difficulty calculation"),
          _("Fixed crash when clicking on a player market with version 1 scenarios"),
        },
      },
      {
        .heading = _("Changed (UI)"),
        .items = {
          _("Improved mouse wheel scrolling"),
          _("Increased max screenshot file limit."),
        },
      },
      {
        .heading = _("Changed (AI)"),
        .items = {
          _("Improved detection of the likely target nation of an attack"),
          _("Improved ai management of firms"),
          _("Improved ai in managing town capture operations"),
          _("Ensured AI checks worker skills in management actions."),
          _("Adjusted AI to avoid accepting treaties from large rivals."),
          _("Prioritized higher-skilled generals and combat-level soldiers in management."),
        },
      },
      {
        .heading = _("Changed (General)"),
        .items = {
          _("Allowed the scenario settings to persist across save and load"),
          _("Improved caravan/caraval trade stop logic"),
          _("Improved manual and added a cover."),
          _("Improved multi-language support"),
          _("Increased the speed of the 9th speed level"),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.10 -- 2025-07-05"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added save/load game list scrolling with the mouse wheel or touchpad, up/down keys, pg up/pg down keys, left/right keys, and home/end keys."),
          _("Added news scrolling with the mouse wheel or touchpad, up/down keys, pg up/pg down keys, left/right keys, and home/end keys."),
          _("Added scrolling with the mouse wheel or touchpad, up/down keys, pg up/pg down keys, left/right keys, and home/end keys to the Kingdoms, Villages, Economy, Trade, Military, Technology, Espionage, and Raking reports."),
          _("Added Inn guest list scrolling with the mouse wheel or touchpad and up/down and home/end keys."),
          _("Added spy list scrolling with the mouse wheel or touchpad, up/down keys, pg up/pg down keys, left/right keys, and home/end keys."),
          _("Added ctrl+click priority builder request to request the nearest builder, ignoring all optimisations and what the nearest builder is doing (except constructing a new building).  Even if the builder request icon is not visible, holding Control will make it appear if there is at least one builder who could walk there."),
          _("Show war machine yearly upkeep and attack range in the info pane."),
          _("Show the Inn guests that will be leaving soon (within 20 days)."),
          _("Added ctrl+click to mobilise workers from a building before selling or demolishing it."),
          _("Added Town race list scrolling with the mouse wheel or touchpad, up/down keys, pg up/pg down keys, left/right keys, and home/end keys."),
          _("Added scroll up and scroll down prompts to the What's New screen."),
          _("Added scroll hints to the What's New screen."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed crash when requesting a builder while training one in a Town."),
          _("Fixed a desync on selling a fort with soldiers or a general in it in multiplayer."),
          _("Fixed Inn selected guest changing when another one leaves."),
          _("Fixed the build marker flashing too fast."),
          _("Fixed items sometimes showing up under the wrong heading on the What's New screen."),
          _("Fixed the building guide lines not reaching the top and right edges of the viewport."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Display the contribution value in the info pane exactly as it relates to total reward and loyalty (the true effect on loyalty is the newly displayed contribution value minus the reward value)."),
          _("Prevent accidentally hiring the wrong Inn guest if the currently selected guest leaves the Inn by disabling the hire button until another selection is made."),
          _("Made the no leader assigned message more obvious by adding \"assigned\"."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.9 -- 2025-06-25"),
    .sections = {
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed save game filenames being all underscores."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.8 -- 2025-06-23"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("Added a clone button to Caravans.  If there is an idle Caravan, it is used as the clone.  Otherwise, a new Caravan spawns at the Market closest to the Caravan's first stop."),
          _("Added a caravan clone button to the trade report, next to the paste route button."),
          _("Automatically \"heal\" caravans by replacing damaged ones using the above caravan cloning feature.  Caravans heading to a Market will be replaced if they have any damage, and caravans which do not have any Market stops will be replaced if they fall to 80% or below."),
          _("Show the skill icon of the unit being trained in Towns under the progress bar on the world map and in the info pane."),
          _("Show the skill level of builders inside buildings on the hammer icon."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed the Town training icon in the info pane not appearing until clicking on the Town again and disappearing after one unit finishes training."),
          _("Fixed high CPU usage on the \"Enter the game's address\" dialog."),
          _("Fixed graphics corruption when hiring units from inns in multiplayer."),
          _("Fixed graphics corruption with disabled buttons."),
          _("Fixed memory violation with building guides."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Preferentially use idle builders outside of buildings further away instead of from closer buildings."),
          _("Moved the \"Done\" text to the right in the War Factory and Harbour build menus, so that they are not covered up by the \"Esc\" keybind display."),
          _("Use lower case for default save filenames, based on the king's name."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.7 -- 2025-06-13"),
    .sections = {
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed a bug where opening the military report before setting a global research target could cause a crash when a Tower of Science was built afterwards."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.6 -- 2025-05-28"),
    .sections = {
      {
        .heading = _(HEADING_ADDED),
        .items = {
          _("When selecting where to place a building, show grid rules of the building, and grid rules one tile out, for a walkable perimeter, to allow for easier placement and planning of buildings."),
          _("When selecting where to place a building, highlight valid locations, darken invalid locations, and shade un-walkable locations red, to create a guide for placing and planning buildings.  This is especially helpful with Harbours."),
          _("Highlight possible Harbour locations brighter when they can link to other buildings, to allow for easier placement of buildings."),
          _("Highlight possible Seat of Power locations brighter when they are away from Towns, to allow for planning so that they do not take up space that can be used by buildings that do link to Towns."),
          _("Highlight possible Inn locations brighter when they are away from Towns and other Inns, to allow for planning so that they do not take up space that can be used by buildings that do link to Towns and so that Inns do not get the proximity penalty."),
          _("Highlight possible locations for other buildings brighter when they can link to other buildings, to allow for easier placement and planning of buildings."),
          _("Show the per-unit and total costs, along with the percentages of the fixed expense, for units on the Military Report."),
          _("Show a total for the unit costs, along with the percentage of fixed expense, on the Military Report."),
          _("Added a keybind for confirming in saving and loading menus (defaults to Enter), settable in config.txt with `bindkey = KEYEVENT_ _CONFIRM,Return`."),
          _("Added the KEYEVENT_ _CONFIRM keybind to mode selection screen for confirming the selection using the Enter key (by default)."),
        },
      },
      {
        .heading = _(HEADING_FIXED),
        .items = {
          _("Fixed the building expense amounts on the Villages report being lower than the real expense."),
          _("Fixed spy icon getting duplicated when workers are re-arranged inside buildings."),
          _("Fixed raw material (and other) icons showing with a white background."),
          _("Fixed potential incorrect placement and crash with user interface bounds."),
        },
      },
      {
        .heading = _(HEADING_CHANGED),
        .items = {
          _("Sortie troops from Forts before selling or destroying them, so that the troops still count as being under the general's leadership."),
          _("When a new builder enters a building under construction, the knocked-out builder will get sent to the building's rally point.  The same applies to builders ejected by pressing the hammer button."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.5 -- 2025-05-03"),
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
          _("Added thousands separators to treasury and food store amounts, to make reading them easier.  (Currently, this uses groups of three with a comma as the separator for everyone.  Proper localisation will be added later.)"),
          _("Show the war machine technology level on the portrait, instead of as part of the name, on the War Factory build list.  This frees up some room for more information."),
        },
      },
    },
  },
  {
    .identifier = _("v3.1.4 -- 2025-04-28"),
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
    .identifier = _("v3.1.3 -- 2025-04-27"),
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
