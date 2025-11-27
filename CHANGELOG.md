# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project softly adheres to [Semantic
Versioning](https://semver.org/spec/v2.0.0.html) where the API is, roughly
speaking, considered to be the player's experience.


## [Unreleased]

### Added

- Added scrolling with the mouse wheel, touchpad, up/down, home/end, and pg
  up/down keys to the tutorial selection list.

### Fixed

- Fixed builders sometimes going to their buildings' rally points instead of to
  the building where they were requested by the request builder button.  The
  workaround for this means that builders will always ignore rally points, for
  the time being.
- Fixed a crash on hovering over the caravan list of the Trade Report when you
  have no caravans.
- Fixed replay desyncs caused by setting Caravan and trade ship stop pickup
  types or deleting trade stops.
- Fixed non-English tutorial descriptions getting cut off at the bottom, by
  adding a scroll bar and buttons, so that all the text can be scrolled and
  read.
- Fixed the config.txt settings not taking effect when there was only a 7kaa
  config.txt and not an Ambition one.

### Changed

- Changed the default setting for Help from Full to Brief, to avoid overwhelming
  new players with information panels.  This only applies to new installations.


## [3.2.7] — 2025-10-24

### Added

- Automatically detect the language on Windows.
- Added a Dutch translation, thanks to Jan Miedema.
- Show the average target resistance of independent towns on the information
  panel when you have workers.

### Fixed

- Fixed target resistance being shown incorrectly when you have workers in the
  independent town.
- Fixed the buttons of the Harbour interface overlapping with the production
  progress bar.
- Fixed the Harbour production count not being placed properly on the button.
- Fixed mines built outside of natural resources showing "null" on their
  information panels.
- Fixed not being able to play replays (R key) because the NONAME.RPL file was
  being saved to the wrong place.

### Changed

- Base the display of the building guide on the Help setting.  Not shown when
  set to None; only the grid lines when set to Brief; and both highlighting and
  grid lines when set to Detailed.
- Changed the Harbour ship count to include the currently-building ship, as it
  blocks other ships from entering, so counts towards the limit.
- Pause replays at the end instead of immediately exiting to the menu without
  warning.


## [3.2.6] — 2025-10-08

### Added

- Show an information panel for Towns, with the number of peasants, total
  population, number of races, and loyalty or resistance.
- Show an information panel for Forts, with the general or king leadership
  level, number of soldiers, and the approximate total strength, including the
  effects of the leader.
- Show information panels for other buildings, with information relevant to
  them, such as worker and good counts.
- Show an information panel for Fryhtan Lairs with their species name and level.
  The level is an approximate relative reflection of the species' strength and
  the gold reward for killing Ordos.  Note that this reflects only the species,
  not the current unit count and combat level, which can only be seen with a
  Phoenix.
- Added a progress bar for Seats of Power.
- Greater Beings will now go to the rally points of their Seats of Power when
  they are summoned.

### Fixed

- Fixed a crash that sometimes happened after an earthquake, where automatic
  caravan cloning would try to both clone and retire an idle caravan at the same
  time, resulting in a segfault.
- Fixed the build Fort keybind (F) not working because it was conflicting with
  selecting a Fort.  The default keybind for selecting a fort has been changed
  to `N`.  You can change by binding the `KEYEVENT_BUILD_CAMP` and
  `KEYEVENT_GOTO_CAMP` events to different keys in the config.txt file.
- Fixed spy Steal Reports option not working in Towns.
- Fixed translations not working on Windows and always showing English.  Now,
  the game will respect the config.txt `locale` setting on Windows.
- Fixed cloned caravans sometimes appearing on another island or in an otherwise
  inaccessible location and not being able to follow its route.

### Changed

- Changed when building hitbars are displayed based on the help setting in the
  options menu.  When it is set to None, building hitbars will never be
  displayed.  When it is set to Brief, they will be displayed for the currently
  selected building and any building hovered over with the mouse.  When it is
  set to Detailed, they will be displayed as per the old behaviour—when the
  building is at less than 100% hitpoints.
- Town and Firm information panels will show the number of spies you have inside
  of them instead of just "(Spy)".  If the general is also your spy, it will say
  "G+" the number of other spies you have.


## [3.2.5] — 2025-09-09

### Added

- Added scrolling with the mouse wheel, up/down, and home/end keys to the
  Harbour ship list.
- Added ship count and capacity display to Harbours on the info pane.
- Added $100,000 throne purchase option.

### Fixed

- Fixed building menus (Town training, Harbour/War Factory build, spy menus,
  etc.) exiting back to the main menu when production of a new unit starts.
- Fixed a crash when looking at the ship list on the trade report.

### Changed

- Start the kingdom score comparison with your own kingdom selected by default.
  You can deselect it or select another kingdom by clicking the button.
- Show the per-unit and total costs when choosing how to respond to food
  purchase requests.
- Use thousands separators for throne purchase amounts options.


## [3.2.4] — 2025-09-03

### Added

- Added kingdom-to-kingdom score comparison to the Rank Report, allowing you to
  see the scores of two kingdoms side-by-side.
- Independent towns will show that their resistance is dropping to 0 when they
  are working for you.
- Added scrolling with the mouse wheel, up/down, home/end, and pg up/down keys
  to the scenario selection list and description.

### Fixed

- Fixed the estimated assassination success chance not properly taking into
  account other defenders present, showing too high when other units were
  present.
- Fixed the estimated assassination success chance against a king being too low.
- Fixed the estimated assassination success chance being too low based on the
  spy skill.
- Fixed the production queue sizes showing the wrong number when training a unit
  finishes.
- Fixed the production queue size display bouncing down and up in multiplayer
  when a unit starts production.
- Fixed the build Seat of Power keybind (P) not working because it was
  conflicting with toggling opaque report mode.  The default keybind for
  toggling opaque report mode has been cleared.  You can re-enable the keybind
  with the `KEYEVENT_REPORT_OPAQUE_TOGGLE` event, with a different key, in the
  config.txt file.
- Fixed the war machine type and level identifiers inside forts causing
  artefacts when they moved around in the list.
- Fixed a bug where loading and unloading a scenario would cause multiplayer to
  stop working.
- Fixed a crash where loading and unloading a scenario and then loading and
  unloading a scenario again would crash the game.
- Fixed possible crashes when pasting text containing characters unprintable by
  the game.

### Changed

- Shifted the bitmaps of Firms to better align with the squares they actually
  take up so that they look more on-centre, especially the Seats of Power.
- Shifted the bitmaps of Fryhtan Lairs to better align with the squares they
  actually take up so that they look more on-centre, especially the Wyrm and Ick
  ones.
- Changed the graphics of Standard tileset terrain that looked like it should
  block building and movement but didn't, and vice versa, so that it's now
  obvious by just looking at it.
- Adjusted the graphics of Standard tileset terrain that looked like it should
  allow building and movement around its corners and edges but didn't, so that
  it's now obvious by just looking that it's not.
- Reworked the assassination chance estimate calculation to better take into
  account the possibility or lack of enemy spies.
- Disabled the caravan clone button (and remove it in the trade report) for idle
  caravans, since trying to clone them does nothing anyway.
- Changed default keybind for train soldier to `F` from `W`, to make it more
  consistent with the other fort-related keybinds (build fort).
- Swapped the default keybinds for same kingdom/any kingdom object switching
  keybinds (`,`/`.` and `<`/`>`), to make it easier to do the more common task
  (switching between units of the same kingdom) without holding they shift key.
- Adjusted the page scrolling on the save game list to keep one item on the
  page, so that it's evident that it's scrolling only one page at a time.
- Break up the 7K:AA changelog entries into categories, thanks to a poster on
  Discord, to make it easier to read.


## [3.2.3] — 2025-08-24

### Added

- Show a Spy-suggested bribe amount on the bribe menu.  This amount varies in
  accuracy over time and based on the spy's skill.  But keep in mind that your
  spies do not know about enemy spies.
- Added an assassination confirmation screen with a Spy-estimated success
  chance.  This estimate varies in accuracy over time and based on the spy's
  skill.
- Added a steal report confirmation screen with a Spy-estimated escape chance.
  This estimate varies in accuracy over time and based on the spy's skill.
- Show the surplus (or deficit) number of peasants that you have on the Villages
  Report.  This is how many extra peasants you currently have over the amount
  you need to exactly balance food production and consumption.
- Increased bribe choices to include $2500 and $3500.
- Added more food purchasing quantity and price options.
- Added more aid and tribute amount options.

### Fixed

- Fixed build guidelines not being clearly visible on Snow and Badlands
  tilesets.
- Fixed some build guidelines appearing a different colour on top of
  highlighting.
- Fixed the Villages Report yearly Firm costs, Military Report yearly Unit
  costs, and War Machine yearly costs showing up 10% higher than actual, now
  that the day accounting bug has been fixed and reduced these costs again.
- Fixed save file names and dates running off on the right side of the load
  screen, creating artefacts.  This was especially problematic in some
  non-English languages where the text is wider or longer.
- Fixed a crash when selling or demolishing an empty Fort which has a rally
  point.
- Fixed Spy steal report escape chances not actually depending on the type of
  report stolen.  Now, different report types will have different chances of
  escape after stealing them.
- Fixed save game list scrolling further than expected.
- Fixed a bug where a ghost guest in an Inn could cause a segmentation fault
  crash.
- Fixed dying units always showing the outside-leadership-range icon.

### Changed

- Made the build guidelines on the Standard tileset slightly brighter, to make
  them more visible.
- Hide icons for units that are dying.
- Lined up the villager and peasant numbers on the Villages Report.
- Show the per-unit and total costs of food purchase requests.
- Increased the game speed of the unlimited speed by not drawing every frame,
  but only 60 FPS.
- Use thousands separators for food purchase quantities and aid and tribute
  amounts.


## [3.2.2] — 2025-08-18

### Changed

- No game changes.  This just has packaging changes to work with Gentoo.


## [3.2.1] — 2025-08-12

### Added

- Added keybinds and keybind display to Town training menu buttons.  The events
  for the config.txt file are: `KEYEVENT_TRAIN_BUILDER`,
  `KEYEVENT_TRAIN_SOLDIER`, `KEYEVENT_TRAIN_MINER`, `KEYEVENT_TRAIN_ARTISAN`,
  `KEYEVENT_TRAIN_RESEARCHER`, and `KEYEVENT_TRAIN_SPY`.
- Added keybinds and keybind display to Spy menu buttons.  The events for the
  config.txt file are: `KEYEVENT_UNIT_REWARD`, `KEYEVENT_SPY_ASSASSINATE`,
  `KEYEVENT_SPY_BRIBE`, `KEYEVENT_SPY_CAPTURE`, `KEYEVENT_SPY_CHANGE_MISSION`,
  `KEYEVENT_SPY_MOBILISE`, `KEYEVENT_SPY_MENU`, and
  `KEYEVENT_SPY_STEAL_REPORTS`.
- Added a keybind for mobilising all workers in a building.  This uses the
  existing event of `KEYEVENT_FIRM_PATROL` (defaults to `R`).
- Load 7kaa's config.txt file as well as Ambition's one, to reduce the need for
  duplication.  Settings in Ambition's config.txt override those is 7kaa's one.
- Added Discord and feedback links to the main menu, in the bottom-left corner.
- Added keybinds for starting a new single-player game (`KEYEVENT_CONFIRM`) and
  for going back (`KEYEVENT_CANCEL`).

### Fixed

- Fixed leadership status artefacts on the info pane when the status changes.
- Fixed spies seemingly always getting caught after stealing reports.
- Fixed spies having a 100% escape chance when stealing reports.

### Changed

- Made the keybind displays appear transparent over their buttons, to obscure
  less of the underlying buttons.
- Enabled default hotkeys for the build menu and disabled default `Q`, `W`, and
  `E` for selecting map modes.  The new default `E` now cycles through all of
  the map modes; and `W` is the default hotkey for building a War Factory.  This
  can all be overridden in the config.txt file.
- Changed the default hotkeys for next/previous object to `.`, `,`, `shift+.`,
  and `shift+,`, so that they do not conflict with scrolling.
- Reduced the brightness of the building guide lines from bright white, so that
  they're less jarring.
- Only ask for feedback about once a month, instead of every time.
- Treat config.txt lines which begin with `#ambition` as actual settings
  (ignoring the `#ambition` part and using the rest of the line) instead of
  comments.  This allows sharing of configs with 7kaa without worrying about
  Ambition settings not being recognised by 7kaa.
- Split local data into multiple directories, to make finding what you need
  easier: `config`, `screenshot`, `save/singleplayer`, and `save/multiplayer`.
  Existing files in the base directory will be migrated to their appropriate
  location.
- Start playing music straight away on the What's New screen instead of only on
  the main menu, to give the right atmosphere.  The usual settings for music and
  volume are respected.
- Tweak the wording of the feedback request pop-up buttons.


## [3.2.0] — 2025-08-10

A 7kaa-compatibility release.  7kaa recently released a new version (2.15.7),
and this merges in those changes, and adds a few fixes.

See [the v3.2-rc1](#3-2-rc1), [the v3.2-rc2](#3-2-rc2), and [the 7kaa
2.15.7](#7kaa-2-15-7) sections for more details.


<a id="3-2-rc2"></a>
## [3.2-rc2] — 2025-08-07

### Fixed

- Fixed the oversized Town training buttons and the artefacts they caused on the
  info pane.
- Fixed the file time display of old save game files.


<a id="3-2-rc1"></a>
## [3.2-rc1] — 2025-08-03

### Fixed

- Fixed incorrect and misplaced field labels for Workers in the info pane.
- Fixed Inn scrolling not showing lower guests when there are more than 4
  guests.
- Fixed config.txt settings for `KEYEVENT_CANCEL` and `KEYEVENT_CONFIRM` not
  working.
- Removed the clone button for ships on the Trade Report, which causes a crash
  and was not meant to be put in yet.  (Cloning ships is a planned featured to
  be added later.)

### Changed

- Updated 7kaa compatibility to the recently-released 7kaa 2.15.7 (see below for
  more details about the update).


<a id="7kaa-2-15-7"></a>
## 7K:AA [2.15.7] — 2025-07-25

### Improvements from Ambition

- Allow comments in config.txt (lines starting with `#`).
- Fixed requested builder sometimes being unable to reach destination.
- Correct day counting.
- Fixed high-potential soldiers gaining less leadership.
- Fixed unit skill potential to persist, and not continually be randomised.
- Fixed issues with skills being reset when entering buildings.
- Improved some graphic drawing.

### Other Changes

#### Added

##### UI

- Added support for a hotkey to center on the selected unit
- Added support to make numeric keys configurable
- Added advanced options for news notifications
- Added support for ctrl button in hotkey mapping
- Allow screenshots of in-game menu, to easily record map seed
- Added hotkey support to cancel build or settle (typically right mouse button)

##### General

- Extended game file and synchronization code to support extensions to the game
- Allowed the scenario settings to persist across save and load.

#### Fixed

##### UI

- Fixed mouse sticky/scrolling issues.
- Fixed an issue selecting invisible units.

##### AI

- Fixed poor ai logic that cause the ai to always accept a trade treaty
- Fixed poor ai logic concerning building weapons
- Fixed poor ai logic concerning hiring spies
- Fixed an error when the ai attacking an unit that enters a building
- Corrected utilization of base towns, typically an ai concept
- Improved ai to avoid more cases of accepting treaties of large rivals
- Fixed an issue that caused the ai to duplicate fort build actions
- Fixed more sea based ai actions
- Corrected more instances where the ai incorrect targets itself
- Fixed some ai capture code
- Fixed sea-based AI actions and pathing to board ships.

##### General

- Fixed some in-game statistics
- Fixed some game file bugs
- Removed a bug causing a fake earthquake
- Removed a possible accidental disclosure of the presence of spies
- Fixed an error occuring sometimes when a town turns independent
- Fixed an error occuring sometimes on recruiting the last pop of a town
- Fixed a pathing issue
- Fixed issue that allowed resigning ships with units in ships
- Fixed memory issue in multiplayer
- Fixed difficulty calculation
- Fixed crash when clicking on a player market with version 1 scenarios

#### Changed

##### UI

- Improved mouse wheel scrolling
- Increased max screenshot file limit.

##### AI

- Improved detection of the likely target nation of an attack
- Improved ai management of firms
- Improved ai in managing town capture operations
- Ensured AI checks worker skills in management actions.
- Adjusted AI to avoid accepting treaties from large rivals.
- Prioritized higher-skilled generals and combat-level soldiers in management.

##### General

- Allowed the scenario settings to persist across save and load
- Improved caravan/caraval trade stop logic
- Improved manual and added a cover.
- Improved multi-language support
- Increased the speed of the 9th speed level


## [3.1.10] — 2025-07-05

### Added

- Added save/load game list scrolling with the mouse wheel or touchpad, up/down
  keys, pg up/pg down keys, left/right keys, and home/end keys.
- Added news scrolling with the mouse wheel or touchpad, up/down keys, pg up/pg
  down keys, left/right keys, and home/end keys.
- Added scrolling with the mouse wheel or touchpad, up/down keys, pg up/pg down
  keys, left/right keys, and home/end keys to the Kingdoms, Villages, Economy,
  Trade, Military, Technology, Espionage, and Raking reports.
- Added Inn guest list scrolling with the mouse wheel or touchpad and up/down
  and home/end keys.
- Added spy list scrolling with the mouse wheel or touchpad, up/down keys, pg
  up/pg down keys, left/right keys, and home/end keys.
- Added ctrl+click priority builder request to request the nearest builder,
  ignoring all optimisations and what the nearest builder is doing (except
  constructing a new building).  Even if the builder request icon is not
  visible, holding Control will make it appear if there is at least one builder
  who could walk there.
- Show war machine yearly upkeep and attack range in the info pane.
- Show the Inn guests that will be leaving soon (within 20 days).
- Added ctrl+click to mobilise workers from a building before selling or
  demolishing it.
- Added Town race list scrolling with the mouse wheel or touchpad, up/down keys,
  pg up/pg down keys, left/right keys, and home/end keys.
- Added scroll up and scroll down prompts to the What's New screen.
- Added scroll hints to the What's New screen.

### Fixed

- Fixed crash when requesting a builder while training one in a Town.
- Fixed a desync on selling a fort with soldiers or a general in it in
  multiplayer.
- Fixed Inn selected guest changing when another one leaves.
- Fixed the build marker flashing too fast.
- Fixed items sometimes showing up under the wrong heading on the What's New
  screen.
- Fixed the building guide lines not reaching the top and right edges of the
  viewport.

### Changed

- Display the contribution value in the info pane exactly as it relates to total
  reward and loyalty (the true effect on loyalty is the newly displayed
  contribution value minus the reward value).
- Prevent accidentally hiring the wrong Inn guest if the currently selected
  guest leaves the Inn by disabling the hire button until another selection is
  made.
- Made the no leader assigned message more obvious by adding "assigned".


## [3.1.9] — 2025-06-25

### Fixed

- Fixed save game filenames being all underscores.


## [3.1.8] — 2025-06-23

### Added

- Added a clone button to Caravans.  If there is an idle Caravan, it is used as
  the clone.  Otherwise, a new Caravan spawns at the Market closest to the
  Caravan's first stop.
- Added a caravan clone button to the trade report, next to the paste route
  button.
- Automatically "heal" caravans by replacing damaged ones using the above
  caravan cloning feature.  Caravans heading to a Market will be replaced if
  they have any damage, and caravans which do not have any Market stops will be
  replaced if they fall to 80% or below.
- Show the skill icon of the unit being trained in Towns under the progress bar
  on the world map and in the info pane.
- Show the skill level of builders inside buildings on the hammer icon.

### Fixed

- Fixed the Town training icon in the info pane not appearing until clicking on
  the Town again and disappearing after one unit finishes training.
- Fixed high CPU usage on the "Enter the game's address" dialog.
- Fixed graphics corruption when hiring units from inns in multiplayer.
- Fixed graphics corruption with disabled buttons.
- Fixed memory violation with building guides.

### Changed

- Preferentially use idle builders outside of buildings further away instead of
  from closer buildings.
- Moved the "Done" text to the right in the War Factory and Harbour build menus,
  so that they are not covered up by the "Esc" keybind display.
- Use lower case for default save filenames, based on the king's name.


## [3.1.7] — 2025-06-13

### Fixed

- Fixed a bug where opening the military report before setting a global research
  target could cause a crash when a Tower of Science was built afterwards.


## [3.1.6] — 2025-05-28

### Added

- When selecting where to place a building, show grid rules of the building, and
  grid rules one tile out, for a walkable perimeter, to allow for easier
  placement and planning of buildings.
- When selecting where to place a building, highlight valid locations, darken
  invalid locations, and shade un-walkable locations red, to create a guide for
  placing and planning buildings.  This is especially helpful with Harbours.
- Highlight possible Harbour locations brighter when they can link to other
  buildings, to allow for easier placement of buildings.
- Highlight possible Seat of Power locations brighter when they are away from
  Towns, to allow for planning so that they do not take up space that can be
  used by buildings that do link to Towns.
- Highlight possible Inn locations brighter when they are away from Towns and
  other Inns, to allow for planning so that they do not take up space that can
  be used by buildings that do link to Towns and so that Inns do not get the
  proximity penalty.
- Highlight possible locations for other buildings brighter when they can link
  to other buildings, to allow for easier placement and planning of buildings.
- Show the per-unit and total costs, along with the percentages of the fixed
  expense, for units on the Military Report.
- Show a total for the unit costs, along with the percentage of fixed expense,
  on the Military Report.
- Added a keybind for confirming in saving and loading menus (defaults to
  Enter), settable in config.txt with `bindkey = KEYEVENT_CONFIRM,Return`.
- Added the KEYEVENT_CONFIRM keybind to mode selection screen for confirming the
  selection using the Enter key (by default).

### Fixed

- Fixed the building expense amounts on the Villages report being lower than the
  real expense.
- Fixed spy icon getting duplicated when workers are re-arranged inside
  buildings.
- Fixed raw material (and other) icons showing with a white background.
- Fixed potential incorrect placement and crash with user interface bounds.

### Changed

- Sortie troops from Forts before selling or destroying them, so that the troops
  still count as being under the general's leadership.
- When a new builder enters a building under construction, the knocked-out
  builder will get sent to the building's rally point.  The same applies to
  builders ejected by pressing the hammer button.


## [3.1.5] — 2025-05-03

### Added

- Show building construction costs on buttons.
- Show war machine and ship construction costs on War Factory and Harbour build
  menus.
- Show total production queue count and cost for War Factories and Harbours.
- Show keybinds at the bottom-left of buttons that have them.
- Add a keybind for cancelling menus (defaults to Esc), settable in config.txt
  with `bindkey = KEYEVENT_CANCEL,Escape`.

### Changed

- Show Spy combat (in Forts) or primary skill (in other Firms) level in the
  worker list by moving the spy icon to the portrait.
- Added thousands separators to treasury and food store amounts, to make reading
  them easier.  (Currently, this uses groups of three with a comma as the
  separator for everyone.  Proper localisation will be added later.)
- Show the war machine technology level on the portrait, instead of as part of
  the name, on the War Factory build list.  This frees up some room for more
  information.


## [3.1.4] — 2025-04-28

### Fixed

- Fixed spy change nation colours not working properly and corruption with
  buttons on info pane.  (Temporarly removed the leadership panel until a better
  solution is implemented.)


## [3.1.3] — 2025-04-27

### Added

- Added clipboard keys (Ctrl+C, Ctrl+X, Ctrl+V) to input fields, such as the
  address of the "Enter Address" multiplayer option, map ID, and name fields.
- Allow prepending to (adding to to the start of) production queues with
  Ctrl+click, allowing quickly producing a unit without clearing the entire
  queue.
- Automatically copy the map ID to the clipboard when opening the in-game menu,
  allowing for easy copy-pasting of the map ID.
- Automatically paste into the join address field, allowing for easy joining of
  games when using the "Enter Address" option.
- Added a What's New screen, shown on the first startup of each new version.

### Fixed

- Fixed setting rally point from a Town using shift also pulling peasants.
- Fixed pixel artefacts on the bottom of credits and results screens.
- Fixed pixel artefact on text update.
- Fixed Alt+right-click not pulling peasants from a Town in Classic mode.


## [3.1.2] — 2025-04-20

### Added

- Show an icon for units which are outside of their general's command range and
  so do not receive combat bonuses and are losing loyalty.
- Display unit leadership effect status in the info pane.
- Make War Factory production queue unlimited, up from 20.
- Make Harbour production queue unlimited, up from 10.
- Show the production queue size for Towns, Harbours, and War Factories over the
  production button, in a manner similar to the auto-tax.

### Fixed

- Fixed desync when cancelling training in a Town.
- Fixed alignment of town goods satisfaction, inn guest count, and worker list.
- Fixed Town training queue counts not showing correctly after loading a game
  with a 7kaa training queue.
- Fixed not being able to cancel production after loading a 7kaa save game with
  a unit already in production.
- Fixed not being able to cancel units in the queue after loading a 7kaa save
  game with an existing production queue.

### Changed

- Display the type and level of war machines inside forts, for easier
  identification.
- Harbour production queue persists even when the harbour is full (4 ships)
  instead of clearing the queue.


## [3.1.1] — 2025-04-12

### Added

- Rally points can be toggled by setting the rally point to the same place
  again, in addition to alt-right-clicking on the building again.
- Waypoints (and rally points) can now be set (and unset) with shift-right-click
  in addition to alt-right-click, to be more intuitive to people.
- Added a rally point button on the info pane for buildings, which shows when a
  rally point is active.  Clicking on it goes to the rally location and
  right-clicking on it clears the rally point.
- Added T (actually the same as the unit settle key) as a key to clear the rally
  point of a building.

### Fixed

- Fixed positioning of the colour box on the info pane for buildings and units.
- Fixed porcupines and other not-attacking units not following waypoints.


## [3.1.0] — 2025-04-06

First big release.  See [the v3.1-rc1](#3-1-rc1) section for more details.

### Fixed

- Fixed a crash on requesting a builder when one is on a ship.


## [3.1-rc2] — 2025-03-31

### Fixed

- Fixed momentary multiplayer desync when a unit goes to next waypoint.
- Fixed waypoints disappearing in multiplayer.
- Fixed technologies researching beyond level 3.
- Fix crash when spy drops identity.
- Fixed a crash when a kingdom is destroyed or surrenders while still having
  spies.  This crash also happened on exit, which is also fixed.


<a id="3-1-rc1"></a>
## [3.1-rc1] — 2025-03-27

### Added

- Added rally points for War Factory war machines.
- Added rally points for Town peasants and trained units.
- Added rally points for Inn hired units.
- Added rally points for Camp units when sortieing.
- Added rally points for all firms when mobilising workers.
- Added rally points for Harbours when sailing ships.

### Changed

- Make Town training queue unlimited, up from 10.
- Use global research target for all new and newly idle Towers of Science, not
  just the ones that were present when first clicking.
- Waypoints can now be set to attack and enter buildings instead of just moving
  to a destination.
- Waypoints now no longer get cleared if the unit cannot reach one destination,
  allowing for exploration without interruption.
- Rally points can also be set to attack and enter buildings instead of just moving
  to a destination, the same as waypoints.
- Rally points are now visible over unexplored blackness.
- Setting a rally point on a building will not pull peasants to it.



## [3.0.4] — 2025-03-16

### Added

- Added delete button to load game menu.
- Show a progress bar on the map for town training.

### Fixed

- Fixed requested builder not being able to reach the destination building if
  there is no walkable path.
- Fixed Inn hire button being enabled/disabled incorrectly.
- Fixed hitbars having extra pixels on the right when a worker enters or
  leaves the building.

### Changed

- Continue loading config.txt even when there are invalid lines, so that the
  valid ones still apply.  Then, once finished loading, display all the line
  numbers which contained errors, allowing fixing them.


## [3.0.3] — 2025-03-07

### Added

- Show a progress bar for Harbours.  (They were missed when Towers of Science
  and War Factories were done.)

### Fixed

- Fixed the town recruitment progress bar overlapping the buttons when a spy is
  present and the spy button is showing.


## [3.0.2] — 2025-03-04

### Added

- Show town goods demand satisfaction (quality of life) to know how much goods
  are demanded and how much are being supplied—and thus the level of the bonus.
- Show a progress bar for Towers of Science and War Factories.

### Fixed

- Fixed the unit path line (and building connection lines) animation speed
  breaking on game speed unlimited (9).  This should also fix the issue some
  users were having where moving the mouse would speed up the line animation.
- Fixed the building hitbars looking kind of off and tilted.
- Fix high CPU usage on showing help on hover.

### Changed

- Scale unit hitbar widths with the (square root of) their maximum hitpoints, to
  give a better idea of units' toughness at a glance.
- Add more hitbar colours (dark green, orange, red) to make judging unit
  tougness easier.
- Draw town/building link lines consistently in the same direction, regardless
  of which side is selected.  Since some links make sense only as bi-directional
  flow, these links are drawn alternating in both direction.
- Darkened the bottom-left pixel of the hitbar outline to make it look more in
  place.


## [3.0.1] — 2025-02-25

### Added

- Added game speed display to top-right of the map and "PAUSED" in the middle.
- Show the guest count in an inn, to make it easy to know whether scrolling is
  even required.
- Show the amount that non-spy units have been rewarded, so that it's easier to
  tell the effects of the contribution on loyalty.

### Fixed

- Fixed high CPU usage when moving the mouse cursor in-game.  This should also
  fix the unit path line looking like it's moving when the mouse cursor is
  moved.
- Fixed minimap bounding box colours.

### Changed

- Display inn guest list from newest at the top to oldest at the bottom, so that
  it's quicker to check inns for new mercenaries.
- Preferentially use builders from undamaged buildings further away instead of
  from closer buildings that are still not fully repaired.
- Use all of the available space in the building info pane to show wider unit
  hitbars, so that they're easier to comprehend.
- Show the missing portion of unit hitbars in the building info pane, so that
  it's easier to tell when a unit is missing hitpoints.
- Draw a hitbar for the general/king in a camp, so that you can see his health.
- Properly centre units in the building info pane, so that the hitbars are
  clearly discernible.
- Use a four-slot screen for the mode selection.  This gives more space to
  expand the description text.
- Use roman numerals for numbering modes on the mode selection screen.  This
  aesthetic is more appropriate.
- Make mode selection screen translatable.  No translations have been completed
  yet, though.


## [3.0.0] — 2025-02-21

First full release.  See [the v3.0.0-rc1](#3-0-0-rc1) section for more details.


## [3.0.0-rc4] — 2025-02-18

### Fixed

- Fixed own spy icons not showing all the time.  Own spy icons used to show all
  the time before, regardless of selection or icon showing setting.
- Fixed icons not showing on hover.
- Fixed the no games to load message not showing straight away after clicking
  load game on the in-game menu.

### Changed

- Make the hitbar outline (friendly vs enemy) look less flat by giving it a
  shadow.


## [3.0.0-rc3] — 2025-02-17

### Fixed

- Fixed speed 9 (the pseudo-speed for fastest possible speed) being slower than
  it used to in 7kaa.
- Fixed the in-game menu not showing straight away after clicking it.
- Fixed the option, save and load game, and tutor in-game menu selections not
  showing straight away after clicking them.


## [3.0.0-rc2] — 2025-02-16

### Fixed

- Fixed the builder icon sometimes showing as repairing while under attack.
- Fixed a crash when pausing in Classic mode.


<a id="3-0-0-rc1"></a>
## [3.0.0-rc1] — 2025-02-03

### Added

- Display the current running mode in top-right of the main menu.
- Added a rudimentary running mode switcher accessible from a button at the
  top-right of the main menu.
- Made the game look smoother by running at at a graphical 60 FPS.
- Added cyan highlight to selected units, making for easier identification.
- Added coloured outline to hit bars, making it easier to distinguish friendly
  units' hit bars from enemies'.
- Show a hitbar for firms when they are at less than 100% HP.
- Added builder icon to buildings with builders in them, allowing easy
  at-a-glance identification of them.
- Made the game look smoother by constantly animating buildings at a fixed rate.
- Made the game look smoother by constantly animating water and other terrain at
  a fixed rate.
- Made the game look smoother by having the water be more sloshy and run
  approximately into the south-western direction as the waves imply.
- Made the game look smoother by continuing rain and snow while game is paused.
- Display more details when a multiplayer sync error occurs to help hunt down
  and fix the cause.

### Fixed

- Fixed the 100% CPU usage all the time problem.
- Fixed scrolling to the right and down not working properly.
- Fixed the unit path line flow direction to go from the unit to its destination
  instead of backwards.
- Centred the info pane (the one below the minimap) so that it's no longer
  off-centre.
- Fixed a memory leak when loading a saved game.
- Fixed a memory leak with unit waypoints.

### Changed

- Use yellow colour for highlighting units on hover instead of cyan.
- Made it easier to see when units are low on hitpoints by making the unit
  hitbars wider.
- Change right and bottom cursor boundaries to allow the cursor to go further.
- Made development slightly easier.


## [2.9156.0] — 2025-01-07

### Changed

- Forked _Seven Kingdoms: Ancient Adversaries_ into _Seven Kingdoms: Ambition_.
- Made development slightly easier.


## 7K:AA [2.15.6] — 2023-09-22

### Added

- Added display of the land mass size in the F10 menu
- Enabled town peasant migration
- Created button for markets to control stocking type -- thanks MicroVirus and
   sraboy
- Enabled the ability for AI to scout the map
- Added Phoenix vision on fryhtan lairs

### Fixed

- Fixed bugs related to rebelling
- Fixed several crash bugs
- Fixed small graphic update bugs
- Fixed bug when assigning to ship
- Fixed AI declare war bug
- Fixed AI give tribute bug
- Cleaned up some deinit code
- Fixed bugs related to AI surrenders
- Fixed bug on trade ship docking and exiting
- Fixed some path finding issues
- Fixed screen edge mouse handling bug
- Fixed exploit when spying on an enemy nation report
- Improved distance calculating so placing buildings and towns on any side is
   treated the same

### Changed

- Improved AI on handling rebellions
- Greatly improved AI sea based missions
- Improved AI on training and recruiting
- Improved handling of town migration with workers
- Improved AI to be move aggressive in attack missions
- Improved validation of attack actions
- Improved AI at building harbors
- Improved locale code
- Made English the default language on Windows
- Increased the max number of raw resource sites to 10
- Reduced penalty on trade unit destruction
- Increased the speed of speed level 9
- Improved AI handling on granting money to towns and recruiting
