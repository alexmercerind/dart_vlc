<h1 align="center"><a href="https://github.com/alexmercerind/dart_vlc">dart_vlc</a></h1>
<h4 align="center">Bringing power of VLC to Flutter & Dart apps on Windows & Linux</h4>


## Installation

```yaml
dependencies:
  ...
  dart_vlc: ^0.0.1
```

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_5.png?raw=true)


## Documentation

Create a new `Player` instance.
```dart
Player player = await Player.create(id: 69420);
```

Create a single `Media`.
```dart
Media media0 = await Media.file(
  new File('C:/music.mp3')
);

Media media1 = await Media.network(
  'https://www.example.com/music.aac'
);

Media media2 = await Media.asset(
  'assets/music.ogg'
);
```

Create a list of `Media`s using `Playlist`.
```dart
Playlist playlist = new Playlist(
  medias: [
    await Media.file(new File('C:/music.mp3')),
    await Media.asset('assets/music.ogg'),
    await Media.network('https://www.example.com/music.aac'),
  ],
);
```

Open `Media` or `Playlist` into a `Player` instance.
```dart
player.open(
  new Playlist(
    medias: [
      await Media.file(new File('C:/music0.mp3')),
      await Media.file(new File('C:/music1.mp3')),
      await Media.file(new File('C:/music2.mp3')),
    ],
  ),
  autoStart: true, //default
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

Manipulate `Playlist`.
```dart
player.add(
  await Media.file(new File('C:/music0.mp3')),
);

player.remove(4);

player.insert(
  2,
  await Media.file(new File('C:/music0.mp3')),
);

player.move(0, 4);
```

Set playback volume & rate.
```dart
player.setVolume(0.5);

player.setRate(1.25);
```

Get & change playback `Device`.
```dart
List<Device> devices = await Devices.all;

player.setDevice(
  devices[0],
);
```

Retrieve metadata of `Media`.
```dart
Media media = await Media.network(
  'https://www.example.com/media.mp3',
  parse: true,
  timeout: new Duration(seconds: 10),
);

Map<String, String> metas = media.metas;
```

Listen to playback events.

(Same can be retrieved directly from `Player` instance without having to rely on stream).

```dart
player.currentStream.listen((CurrentState state) {
  state.index;
  state.media;
  state.medias;
  state.isPlaylist;
});
```

```dart
player.positionStream.listen((PositionState state) {
  state.position;
  state.duration;
});
```

```dart
player.playbackStream.listen((PlaybackState state) {
  state.isPlaying;
  state.isSeekable;
  state.isCompleted;
});
```

```dart
player.generalStream.listen((GeneralState state) {
  state.volume;
  state.rate;
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

- `Media` playback from `File`.
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
- `add`/`insert`/`remove`/`move` `Media` inside `Playlist` during playback.
- Device enumeration & changing.
- Retrieving `Meta` of a `Media`.


Under progress (irrespective of order)...

- Embedding video inside the Flutter window.
- Make things more efficient.
- Supporting live streaming links.
- Supporting native volume control/lock screen notifications.
- FFI version of the library for plain Dart applications.
- Bringing project on other platforms like Android/iOS.


## Contributions

The code in the project is nicely arranged (I guess), I have added comments wherever I felt necessary.

Contributions to the project are open, it will be appreciated if you discuss the bug-fix/feature-addition in the issues first.

## License

Copyright (C) 2021, Hitesh Kumar Saini.

This library & work under this repository is licensed under GNU Lesser General Public License v2.1.

## Acknowledgements

Thanks to following people for supporing the project:
- [@DomingoMG](https://github.com/DomingoMG)
- Salman Aljabri

## Spoilers

Currenty video playback is also supported out of the box, but it doesnt show inside Flutter window.


![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_2.PNG?raw=true)

## Vision

There aren't any media (audio or video) playback libraries for Flutter on Windows/Linux yet. So, this project is all about that.
As one might be already aware, VLC is one of the best media playback tools out there.

So, now you can use it to play audio or video [WIP] files from Flutter Desktop app.

The API style of this project is highly influenced by [assets_audio_player](https://github.com/florent37/Flutter-AssetsAudioPlayer) due to its completeness. This project will serve as a base to add Windows & Linux support to already existing audio playback libraries like [just_audio](https://github.com/ryanheise/just_audio) and [assets_audio_player](https://github.com/florent37/Flutter-AssetsAudioPlayer).

Although, the mentioned repositories above are for audio playback, video playback is also a part of consideration for this project.

Thanks to the [VideoLAN](https://www.videolan.org) team for creating [libVLC](https://github.com/videolan/vlc) & [libVLC++](https://github.com/videolan/libvlcpp). Really great guys really great at their work.
