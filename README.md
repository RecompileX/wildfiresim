# Wildfire Simulator

Wildfire Simulator is a C++ strategy game built with raylib. Manage an active wildfire by dropping water and fire retardant across grid-based maps. Save as many trees and houses as possible while avoiding harmful retardant drops on houses and water.

## Features

- Wind-driven wildfire spread
- Water and retardant aerial drops
- Drop length limits and cooldowns
- Five playable maps
- Structure survival and environmental scoring
- Music, victory audio, and volume settings

## Screenshots

### Gameplay

Fight an expanding wildfire with carefully placed water and retardant lines.

![Wildfire Simulator gameplay](assets/images/Screenshot%202026-08-31%20194709.png)

### Island Lagoon map

Island Lagoon is one of the game's five available maps.

![Island Lagoon map](assets/images/Screenshot%202026-08-31%20194812.png)

### Main menu

Choose a map or open the settings screen before starting a game.

![Wildfire Simulator main menu](assets/images/Screenshot%202026-08-31%20194907.png)

## Controls

1. Select **Water** or **Retardant** at the top of the screen.
2. Click once to choose the start of a drop.
3. Click again to choose its end.
4. Wait for the resource cooldown before placing another drop.

Water extinguishes burning structures. Retardant can protect trees, but dropping it on houses or water lowers the environmental portion of your score.

## Scoring

The final score is out of 1,000 points:

- 750 points are based on the percentage of trees and houses that survive.
- 250 points are based on avoiding retardant contamination of houses and water.

## Building

### Requirements

- CMake 3.20 or newer
- A C++23-compatible compiler
- raylib

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

Run the executable from the repository root so it can find the files under `assets/`.

On Windows with a single-configuration generator:

```powershell
.\build\wildfiresim.exe
```

## Project structure

```text
assets/      Maps, music, screenshots, and the application icon
external/    Bundled raygui and toolbox headers
include/     Project headers
resources/   Windows resource file
src/         C++ source files
```
