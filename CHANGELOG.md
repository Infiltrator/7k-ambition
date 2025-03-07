# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project softly adheres to [Semantic
Versioning](https://semver.org/spec/v2.0.0.html) where the API is, roughly
speaking, considered to be the player's experience.


## [Unreleased]


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
