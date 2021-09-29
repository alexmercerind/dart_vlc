## 0.1.8

- Add `startTime` and `stopTime` parameters to `Media` for clipping (@alexmercerind).
- Add `Player.bufferingProgress` & `Player.bufferingProgressStream` to listen to buffering percentage of the player (@alexmercerind).
- `Video` widget no longer turns black after being scrolled out of the view (@alexmercerind).
- Now Linux uses texture registrar API for video playback (@alexmercerind) (REQUIRES master Flutter channel).
- Now macOS uses texture registrar API for video playback (@jnschulze).
- Initial work on iOS support has been started (@krjw-eyev).

## 0.1.7

- Fixed `Player.open` (`OnOpen` event) randomly causing crash on Windows (@alexmercerind).
- Using constant frame buffer size until #137 is resolved (@alexmercerind).

## 0.1.6

- A hotfix update to fix a critical bug (@alexmercerind).
- Fixed a critical bug that resulted in a crash upon opening more than one `Media` in `Playlist` (apologies) (@alexmercerind).
- Implemented media and playlist equality operators (@jnschulze).
- Added `Player.takeSnapshot` to save snapshot of a playing video (@alexmercerind).

## 0.1.5

- Added initial macOS support (@jnschulze).
- Improved NativePort callbacks & removed unnecessary serialization (@alexmercerind).
- Now using a common dartvlc wrapper CMake library for all platforms (@jnschulze).
- Other bug-fixes related to `Video` (@jnschulze).
- Setup garbage cleaning finalizers for memory allocated on heap (for C++/Dart FFI communication) (@alexmercerind @jnschulze).
- Removed deprecated libVLC API calls (@alexmercerind).

## 0.1.4

- Now `Player` no longer requires `videoWidth` & `videoHeight` to be passed for video playback (@alexmercerind).
- `Video` widget now uses the dimensions of the currently playing video (@alexmercerind).
- For overriding the automatic video dimensions retrieval, `videoDimensions` argument must be passed while instantiating `Player` class (@alexmercerind).  
- `Video` widget no longer asks for `playerId` argument, but `player` instead (@jnszhulze).
- Added `videoDimensionStream` and `videoDimension` attributes to `Player` class to listen to currently playing video dimensions (@alexmercerind).
- Migrated C++ code to use smart pointers instead of raw pointers (@alexmercerind).
- `Player.dispose` no longer causing crash on Windows (#103) (@alexmercerind).
- Added Add fit and alignment properties to `Video` widget (@jnschulze).

## 0.1.3

- Fixed libVLC shared libraries not being copied on Windows (@jnschulze).

## 0.1.2

- Now using `flutter::TextureRegistrar` for performant `Video` on Windows (#54)  (@alexmercerind).
- Fixed `autoStart` in `Player.open` (@alexmercerind).
- Fixed other crashes for Windows (@alexmercerind).
- Improved stability (@alexmercerind).

## 0.1.1

- Fixed setState being called after dispose (#75) (Finally) (@alexmercerind).
- Improved memory management (@alexmercerind).
- Fixed ton of memory leaks (@alexmercerind).
- Fixed `Devices::all` & `Media::parse` causing crash on Windows (@alexmercerind).

## 0.1.0

- Fixed build on Linux (@alexmercerind).
- Changed cmake minimum required version to 3.10 for fixing use with snap installation (@alexmercerind).
- Fixed vlc header not being located in Flutter on Linux version 2.2.4 (@alexmercerind).
- Fixed few memory leaks (@alexmercerind).
- Fixed calling setState after dispose & other Video widget issues (@alexmercerind).
- Fixed build warnings on Linux (@alexmercerind).
- Changed required CMake version to 3.10 on Linux (@alexmercerind).

## 0.0.9

- Fixed multiple `Video` widgets not working after FFI migration (@alexmercerind).
- Now package contains complete libVLC & libVLC++ source inside (@alexmercerind).
  - No longer fetching from videoLAN & GitHub servers required (@alexmercerind).
  - No more build errors for developers in China (@alexmercerind).
- Fixed `Player::setPlaylistMode` (@alexmercerind).
- Fixed built-in play/pause button in Video widget (@alexmercerind).
- Added back `Media.asset` for Flutter (@alexmercerind).

## 0.0.8

- Now using FFI (instead of Platform channels) (@alexmercerind).
- Better performance, being direct C <-> Dart interop with no Flutter involvement.
- Added `Equalizer` class (@alexmercerind).
- Support for Dart CLI. See package `dart_vlc_ffi` (@alexmercerind).
- Added `commandlineArguments` to `Player` constructor to pass VLC commandline arguments (@alexmercerind).
- BREAKING CHANGES
  - Now plugin requires initialization in the `main` method, call `DartVLC.initialize()` to instantiate the plugin (@alexmercerind). 
  - Now all the methods are synchronous & no longer require `await`. Please update your code (@alexmercerind).

## 0.0.7

- Added `Player.setUserAgent` (@alexmercerind).
- Improved & fixed issues related to play/pause button in `Video` widget (@alexmercerind).
- Fixed compilation issues on arch linux (@alexmercerind).
- Fixes to device changing (@alexmercerind).

## 0.0.6

- Now `Player` class has sync constructor & no longer needs `Player.create` (@alexmercerind).
- Fixed memory leak errors on Windows & Linux (@alexmercerind).
- Added controls to `Video` widget (@tomassasovsky).
- Added `Record` class for recording media (@DomingoMG).
- Added `Chromecast` class (@DomingoMG).
- Fixed `Player.setPlaylistMode` on Linux (@alexmercerind).
- Event streams inside `Player` no longer can be `null` (@alexmercerind).

## 0.0.5

- `Broadcast` class to broadcast a `Media` (@alexmercerind).
- Fix to a bug that caused `CurrentState` to not update in `Player` (@alexmercerind).

## 0.0.4

- `Video` `Widget` for showing video output from a `Player` inside `Widget` tree.
  - `Player` must be used as a controller for a `Video` (@alexmercerind).
  - Initialize `Player` with `videoHeight` and `videoWidth` optional parameters, if you wish to use it for video playback (@alexmercerind).
- Null-safety migration (@alexmercerind).

## 0.0.3

- More advanced playlist modification methods like (@alexmercerind):
  - `add` for appending a new `Media` to the `Playlist` of the `Player`.
  - `remove` for removing a `Media` from the `Playlist` of the `Player` from certain index.
  - `insert` method for inserting `Media` to certain index (@alexmercerind).
  - `move` a `Media` from one index to another.
- Ability to get all playback `Device`s on machine & change (@alexmercerind).
  - `Devices.all` gives `List` of all `Device`s.
  - `Player.setDevice` can be used to set a playback device for the `Player` instance (@alexmercerind).
- Now event streams are splitted into four: (@alexmercerind)
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
- Ability to retrieve metadata of a `Media` (either from `Media.network` or `Media.file`) (@alexmercerind). 
  - Now you can access metadata of a `Media` by passing `parse: true` for parsing the metadata (@alexmercerind).
  - Retrieved metadata is stored inside `Media.metas` as `Map<String, String>` (@alexmercerind).


## 0.0.2

- Support for Flutter on Linux (@alexmercerind).
- Fixed bug that caused index to not update properly in `Playlist`, when `next` or `back` or on completion of `Media` (@alexmercerind).
- Changed default `Player` volume to `0.5` (@alexmercerind).

## 0.0.1+1

- Little left-over changes in the project documentation (@alexmercerind).

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
- Initial release, worked & documented by @alexmercerind.