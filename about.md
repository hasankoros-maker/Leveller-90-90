# Leveller

Leveller adds a small button beside level cells. Pressing it opens the selected level in the game's normal play flow. The project is designed as a cross-platform Geode mod for Geometry Dash 2.2.x.

## Compatibility

The source targets Geometry Dash 2.2 or newer within the 2.2 branch. Windows and Android are the primary targets. The macOS entry is included for Geode-supported desktop builds. Apple/iOS support depends on whether Geode is available for the specific device and launcher; it cannot be guaranteed by a mod manifest alone.

## Automation scope

A universal level-completion bot is not implemented in this starter because it requires version-specific PlayLayer input hooks, object timing/path analysis, and extensive testing. The button and normal-mode launch point are implemented without altering completion state or injecting fake progress.

## Build

Install the Geode SDK/CLI, then run:

```sh
geode build
```

For Android:

```sh
geode build -p android64
```
