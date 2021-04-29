## 0.0.6

**This new release of dart_vlc adds:**

- Now `Player` class has sync constructor & no longer needs `Player.create`.
- Fixed memory leak errors on Windows & Linux.
- Added controls to `Video` widget. Thanks to @tomassasovsky.
- Added `Record` class for recording media. Thanks to @DomingoMG.
- Added `Chromecast` class. Thanks to @DomingoMG.
- Fixed `Player.setPlaylistMode` on Linux.
- Event streams inside `Player` no longer can be `null`.

## 0.0.5

**This new release of dart_vlc adds:**

- `Broadcast` class to broadcast a `Media`.
- Fix to a bug that caused `CurrentState` to not update in `Player`.

## 0.0.4

**This new release of dart_vlc adds:**

- `Video` `Widget` for showing video output from a `Player` inside `Widget` tree.
  - `Player` must be used as a controller for a `Video`.
  - Initialize `Player` with `videoHeight` and `videoWidth` optional parameters, if you wish to use it for video playback.
- Null-safety migration.

## 0.0.3

**This new release of dart_vlc adds:**

- More advanced playlist modification methods like:
  - `add` for appending a new `Media` to the `Playlist` of the `Player`.
  - `remove` for removing a `Media` from the `Playlist` of the `Player` from certain index.
  - `insert` method for inserting `Media` to certain index.
  - `move` a `Media` from one index to another.
- Ability to get all playback `Device`s on machine & change.
  - `Devices.all` gives `List` of all `Device`s.
  - `Player.setDevice` can be used to set a playback device for the `Player` instance.
- Now event streams are splitted into four:
  - `Player.currentStream`
    - Contains:
      - `index`
      - `media`
      - `medias`
      - `isPlaylist`
  - `Player.positionStream`
    - Contains:
      - `position`
      - `duration`
  - `Player.playbackStream`
    - Contains:
      - `isPlaying`
      - `isSeekable`
      - `isCompleted`
  - `Player.generalStream`
    - Contains:
      - `volume`
      - `rate`
- Ability to retrieve metadata of a `Media` (either from `Media.network` or `Media.file`). 
  - Now you can access metadata of a `Media` by passing `parse: true` for parsing the metadata.
  - Retrieved metadata is stored inside `Media.metas` as `Map<String, String>`.


## 0.0.2

**This new release of dart_vlc adds:**

- Support for Flutter on Linux.
- Fixed bug that caused index to not update properly in `Playlist`, when `next` or `back` or on completion of `Media`.
- Changed default `Player` volume to `0.5`.

## 0.0.1+1

**This new release of dart_vlc adds:**

- Little left-over changes in the project documentation.

## 0.0.1

**This first release of dart_vlc adds:**

- `Media` playback from file.
- `Media` playback from network.
- `Media` playback from assets.
- `play`/`pause`/`playOrPause`/`stop`.
- Multiple `Player` instances.
- `Playlist`.
- `next`/`back`/`jump` for playlists.
- `setVolume`.
- `setRate`.
- `seek`.
- Events.
- Automatic fetching of headers, libs & shared libraries.
- Changing VLC version from CMake.
- Event streams.
  - `Player.currentState`
    - `index`: Index of current media in `Playlist`.
    - `medias`: List of all opened `Media`s.
    - `media`: Currently playing `Media`.
    - `isPlaylist`: Whether a single `Media` is loaded or a `Playlist`.
  - `Player.positionState`
    - `position`: Position of currently playing media in `Duration`.
    - `duration`: Position of currently playing media in `Duration`.
  - `Player.playbackState`
    - `isPlaying`.
    - `isSeekable`.
    - `isCompleted`.
  - `Player.generalState`
    - `volume`: Volume of current `Player` instance.
    - `rate`: Rate of current `Player` instance.
