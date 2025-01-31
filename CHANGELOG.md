# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project softly adheres to [Semantic
Versioning](https://semver.org/spec/v2.0.0.html) where the API is, roughly
speaking, considered to be the player's experience.

## [Unreleased]

### Added

- Display current running mode in top-right of main menu.
- Add rudimentary running mode switcher with a button at the top-right of the
  main menu.
- Make game look smoother by running at at a graphical 60 FPS.
- Add cyan highlight to selected units, making for easier identification.
- Add coloured outline to hit bars, making it easier to distinguish friendly
  units' hit bars from enemies'.
- Show a hitbar for firms when they are at less than 100% HP.
- Add builder icon to buildings with builders in them, allowing easy at-a-glance
  identification of them.
- Make game look smoother by constantly animating buildings at a fixed rate.
- Make game look smoother by constantly animating water and other terrain at a
  fixed rate.
- Make game look smoother by having the water be more sloshy and run
  approximately into the south-western direction as the waves imply.
- Make game look smoother by continuing rain and snow while game is paused.

### Fixed

- Fixed 100% CPU usage all the time.
- Fixed unit path line flow direction to go from the unit to its destination.
- Centred the info pane (the one below the minimap) so that it's no longer
  off-centre.
- A memory leak when loading a saved game.
- A memory leak with unit waypoints.

### Changed

- Use yellow colour for highlighting units on hover instead of cyan.
- Made development slightly easier.

## [2.9156.0] - 2025-01-07

### Changed

- Forked _Seven Kingdoms: Ancient Adversaries_ into _Seven Kingdoms: Ambition_.
- Made development slightly easier.

## [2.15.6] - 2023-09-22

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
