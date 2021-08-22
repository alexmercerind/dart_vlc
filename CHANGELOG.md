## 0.1.6

- A hotfix update to fix a critical bug.
- Fixed a critical bug that resulted in a crash upon opening more than one `Media` in `Playlist` (apologies).
- Implemented media and playlist equality operators. (Thanks to @jnschulze).
- Added `Player.takeSnapshot` to save snapshot of a playing video.

## 0.1.5

- Added initial macOS support. (Thanks to @jnschulze).
- Improved NativePort callbacks & removed unnecessary serialization.
- Now using a common dartvlc wrapper CMake library for all platforms. (Thanks to @jnschulze).
- Other bug-fixes related to `Video`. (Thanks to @jnschulze).
- Setup garbage cleaning finalizers for memory allocated on heap (for C++/Dart FFI communication).
- Removed deprecated libVLC API calls.

## 0.1.4

- Now `Player` no longer requires `videoWidth` & `videoHeight` to be passed for video playback.
- `Video` widget now uses the dimensions of the currently playing video.
- For overriding the automatic video dimensions retrieval, `videoDimensions` argument must be passed while instantiating `Player` class.  
- `Video` widget no longer asks for `playerId` argument, but `player` instead.
- Added `videoDimensionStream` and `videoDimension` attributes to `Player` class to listen to currently playing video dimensions.
- Migrated C++ code to use smart pointers instead of raw pointers.
- `Player.dispose` no longer causing crash on Windows (#103).
- Added Add fit and alignment properties to `Video` widget (Thanks to @jnschulze).

## 0.1.3

- Fixed libVLC shared libraries not being copied on Windows (thanks to @jnschulze).

## 0.1.2

- Now using `flutter::TextureRegistrar` for performant `Video` on Windows. (#54).
- Fixed `autoStart` in `Player.open`.
- Fixed other crashes for Windows.
- Improved stability.

## 0.1.1

- Fixed setState being called after dispose (#75) (Finally)
- Improved memory management.
- Fixed ton of memory leaks.
- Fixed `Devices::all` & `Media::parse` causing crash on Windows.

## 0.1.0

- Fixed build on Linux.
- Changed cmake minimum required version to 3.10 for fixing use with snap installation.
- Fixed vlc header not being located in Flutter on Linux version 2.2.4.
- Fixed few memory leaks.
- Fixed calling setState after dispose & other Video widget issues.
- Fixed build warnings on Linux.
- Changed required CMake version to 3.10 on Linux.

## 0.0.9

- Fixed multiple `Video` widgets not working after FFI migration.
- Now package contains complete libVLC & libVLC++ source inside.
  - No longer fetching from videoLAN & GitHub servers required.
  - No more build errors for developers in China.
- Fixed `Player::setPlaylistMode`.
- Fixed built-in play/pause button in Video widget.
- Added back `Media.asset` for Flutter.

## 0.0.8

- Now using FFI (instead of Platform channels).
- Better performance, being direct C <-> Dart interop with no Flutter involvement.
- Added `Equalizer` class.
- Support for Dart CLI. See package `dart_vlc_ffi`.
- Added `commandlineArguments` to `Player` constructor to pass VLC commandline arguments.
- BREAKING CHANGES
  - Now plugin requires initialization in the `main` method, call `DartVLC.initialize()` to instantiate the plugin. 
  - Now all the methods are synchronous & no longer require `await`. Please update your code.

## 0.0.7

- Added `Player.setUserAgent`.
- Improved & fixed issues related to play/pause button in `Video` widget.
- Fixed compilation issues on arch linux.
- Fixes to device changing.

## 0.0.6

- Now `Player` class has sync constructor & no longer needs `Player.create`.
- Fixed memory leak errors on Windows & Linux.
- Added controls to `Video` widget. Thanks to @tomassasovsky.
- Added `Record` class for recording media. Thanks to @DomingoMG.
- Added `Chromecast` class. Thanks to @DomingoMG.
- Fixed `Player.setPlaylistMode` on Linux.
- Event streams inside `Player` no longer can be `null`.

## 0.0.5

- `Broadcast` class to broadcast a `Media`.
- Fix to a bug that caused `CurrentState` to not update in `Player`.

## 0.0.4

- `Video` `Widget` for showing video output from a `Player` inside `Widget` tree.
  - `Player` must be used as a controller for a `Video`.
  - Initialize `Player` with `videoHeight` and `videoWidth` optional parameters, if you wish to use it for video playback.
- Null-safety migration.

## 0.0.3

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

- Support for Flutter on Linux.
- Fixed bug that caused index to not update properly in `Playlist`, when `next` or `back` or on completion of `Media`.
- Changed default `Player` volume to `0.5`.

## 0.0.1+1

- Little left-over changes in the project documentation.

## 0.0.1

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
