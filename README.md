<h1 align="center"><a href="https://github.com/alexmercerind/dart_vlc">dart_vlc</a></h1>
<h4 align="center">Bringing power of VLC to Flutter & Dart apps on Windows & Linux</h4>


## Installation

```yaml
dependencies:
  ...
  dart_vlc: ^0.0.1
```

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_3.png?raw=true)


## Documentation

Create a new `Player` instance.
```dart
Player player = await Player.create(id: 69420);
```

Create a single `Media`.
```dart
Media media0 = Media.file(
  new File('C:/music.mp3')
);

Media media1 = Media.network(
  'https://alexmercerind.github.io/music.aac'
);

Media media2 = await Media.asset(
  'assets/music.ogg'
);
```

Open `Media` or `Playlist` into a `Player` instance.
```dart
player.open(
  new Playlist(
    medias: [
      Media.file(new File('C:/music0.mp3')),
      Media.file(new File('C:/music1.mp3')),
      Media.file(new File('C:/music2.mp3')),
    ],
  ),
  autoStart: true, //default
);
```

Create a list of `Media`s using `Playlist`.
```dart
Playlist playlist = new Playlist(
  medias: [
    Media.file(new File('C:/music.mp3')),
    await Media.asset('assets/music.ogg'),
    Media.network('https://alexmercerind.github.io/music.aac'),
  ],
);
```

Control playback.
```dart
player.play();

player.seek(Duration(seconds: 30));

player.pause();

player.playOrPause();

player.stop();
```

Manipulate playlist.
```dart
player.add(
  Media.file(new File('C:/music0.mp3')),
);

player.remove(4);

player.insert(
  2,
  Media.file(new File('C:/music0.mp3')),
);

player.move(0, 4);
```

Set playback volume & rate.
```dart
player.setVolume(0.5);

player.setRate(1.25);
```

Get & change playback devices.
```dart
List<Device> devices = await Devices.all;

player.setDevice(
  devices[0],
);
```

Listen to playback events.

(Same can be retrieved directly from `Player` instance without having to rely on stream).

```dart
player.currentStream.listen((CurrentState state) {
  // state.index;
  // state.media;
  // state.medias;
  // state.isPlaylist;
});
```

```dart
player.positionStream.listen((PositionState state) {
  // state.position;
  // state.duration;
});
```

```dart
player.playbackStream.listen((PlaybackState state) {
  // state.isPlaying;
  // state.isSeekable;
  // state.isCompleted;
});
```

```dart
player.generalStream.listen((GeneralState state) {
  // state.volume;
  // state.rate;
});
```

**NOTE:** For using this plugin on Linux, you must have [libVLC](https://www.videolan.org/vlc/libvlc.html) installed. On debian based distros, run:

```bash
sudo apt-get install libvlc-dev
```

## Support

Consider supporting the project by either/and:
- Starring the repository, to get this hardwork noticed.
- Buying me a coffee.

<a href="https://www.buymeacoffee.com/alexmercerind"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=alexmercerind&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff"></a>

## Example

You can see an example project [here](https://github.com/alexmercerind/dart_vlc/blob/master/example/lib/main.dart).

Windows

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_0.PNG?raw=true)

## Progress

Done

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
- Working on Linux.
- Working on Windows.
- `add`/`insert`/`remove`/`move` `Media` inside `Playlist` during playback.
- Device enumeration & changing.


Under progress (irrespective of order)...

- Retrieving metadata of a file.
- Embeding video inside the Flutter window.
- FFI version of the library for plain Dart applications.
- Supporting live streaming links.
- Bringing project on other platforms like Android/iOS.
- Supporting native volume control/lock screen notifications.

## Contributions

The code in the project is very nicely arranged, I have added comments wherever I felt necessary.

Contributions to the project are open, it will be appreciated if you discuss the bug-fix/feature-addition in the issues first.

## License

Copyright (C) 2021, Hitesh Kumar Saini.

This library & work under this repository is licensed under GNU Lesser General Public License v2.1.

## Acknowledgements

Thanks to [@DomingoMG](https://github.com/DomingoMG) for the donation & testing of the project.

## Spoilers

Currenty video playback is also supported out of the box, but it doesnt show inside Flutter window.


![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_2.PNG?raw=true)

## Vision

There aren't any media (audio or video) playback libraries for Flutter on Windows/Linux yet. So, this project is all about that.
As one might be already aware, VLC is one of the best media playback tools out there.

So, now you can use it to play audio or video [WIP] files from Flutter Desktop app.

The API style of this project is highly influenced by [assets_audio_player](https://github.com/florent37/Flutter-AssetsAudioPlayer) due to its completeness. This project will serve as a base to add Windows & Linux support to already existing audio playback libraries like [just_audio](https://github.com/ryanheise/just_audio) and [assets_audio_player](https://github.com/florent37/Flutter-AssetsAudioPlayer).

Although, the mentioned repositories above are for audio playback, video playback is also a part of consideration for this project.

Thanks to the [VideoLAN](https://www.videolan.org) team for creating [libVLC](https://github.com/videolan/vlc) & [libVLC++](https://github.com/videolan/libvlcpp).
