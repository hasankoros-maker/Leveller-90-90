# Leveller

Leveller adds a button beside level cells. Pressing **BOT** opens the selected level in the game's normal play flow and enables a local experimental obstacle-analysis controller.

## Compatibility

The source targets Geometry Dash **2.2.144** and Geode 2.0.0-beta.27. Windows, Android64 and macOS are listed in the manifest; Android requires an arm64-v8a device.

## Automation scope

The bot is deliberately limited to a small, explicit list of difficult levels, including Detah/Death Corridor and Ton 618. It scans nearby solid and hazard objects in `PlayLayer::postUpdate` and emits conservative jump taps. It is an offline heuristic prototype, not a universal solver: ship, wave, dual, portal and tight orb sequences can fail, and completion is not guaranteed.

The bot does not use a network service, does not write online scores, and does not alter completion state. Practice mode is excluded.

## Build

Install the Geode SDK/CLI, then run:

```sh
geode build
```

For Android64:

```sh
geode build -p android64
```
