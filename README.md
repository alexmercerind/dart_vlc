<h1 align="center"><a href="https://github.com/alexmercerind/dart_vlc">dart_vlc</a></h1>
<h4 align="center">Bringing power of VLC to Flutter & Dart apps on Windows & Linux</h4>


## Installation

```yaml
dependencies:
  ...
  dart_vlc: ^0.0.5
```

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_6.png?raw=true)

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_7.png?raw=true)


## Support

Consider supporting the project by either/and:
- :star: Starring the repository, to get this hardwork noticed.
- :coffee: Buying me a coffee.


Thanks to following people for supporting this project. I'm REALLY GLAD to recieve your appreciation for the time I've spent:
- [@domingomg97](https://twitter.com/domingomg97)
- Pavel Zika
- Salman Aljabri

<a href="https://www.buymeacoffee.com/alexmercerind"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=alexmercerind&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff"></a>

## Documentation

- Create a new `Player` instance.
```dart
Player player = await Player.create(id: 69420);
```

- Create a single `Media`.
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

- Create a list of `Media`s using `Playlist`.
```dart
Playlist playlist = new Playlist(
  medias: [
    await Media.file(new File('C:/music.mp3')),
    await Media.asset('assets/music.ogg'),
    await Media.network('https://www.example.com/music.aac'),
  ],
);
```

- Open `Media` or `Playlist` into a `Player` instance.
```dart
player.open(
  new Playlist(
    medias: [
      await Media.file(new File('C:/music0.mp3')),
      await Media.file(new File('C:/music1.mp3')),
      await Media.file(new File('C:/music2.mp3')),
    ],
  ),
  autoStart: true, // default
);
```

- Control playback.
```dart
player.play();

player.seek(Duration(seconds: 30));

player.pause();

player.playOrPause();

player.stop();
```

- Manipulate `Playlist`.
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

- Set playback volume & rate.
```dart
player.setVolume(0.5);

player.setRate(1.25);
```

- Get & change playback `Device`.
```dart
List<Device> devices = await Devices.all;

player.setDevice(
  devices[0],
);
```

- Show the `Video` inside `Widget` tree.

Instanciate `Player` as follows.
```dart
Player player = await Player.create(
  id: 69420,
  videoWidth: 480,
  videoHeight: 320,
);
```
Show `Video` in the `Widget` tree.
```dart
class _MyAppState extends State<MyApp> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Video(
        playerId: 69420,
        height: 1920.0,
        width: 1080.0,
        scale: 1.0, // default
        showControls: false, // default
      ),
    );
  }
}
```

- Retrieve metadata of `Media`.
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
player.currentStream?.listen((CurrentState state) {
  state.index;
  state.media;
  state.medias;
  state.isPlaylist;
});
```

```dart
player.positionStream?.listen((PositionState state) {
  state.position;
  state.duration;
});
```

```dart
player.playbackStream?.listen((PlaybackState state) {
  state.isPlaying;
  state.isSeekable;
  state.isCompleted;
});
```

```dart
player.generalStream?.listen((GeneralState state) {
  state.volume;
  state.rate;
});
```

- `Broadcast` a `Media`.

Broadcasting to localhost.

```dart
Broadcast broadcast = await Broadcast.create(
  id: 0,
  media: await Media.file(new File('C:/video.mp4')),
  configuration: new BroadcastConfiguration(
    access: 'http',
    mux: 'mpeg1',
    dst: '127.0.0.1:8080',
    vcodec: 'mp1v',
    vb: 1024,
    acodec: 'mpga',
    ab: 128,
  ),
);
broadcast.start();
```

Dispose the `Broadcast` instance to release resources.
```dart
broadcast.dispose();
```

- `Record` a `Media`.

```dart
Record record = await Record.create(
  id: 205, 
  media: await Media.network('https://www.example.com/streaming-media.MP3'), 
  pathFile: '/home/alexmercerind/recording.MP3',
);
record.start();
```

**NOTE:** For using this plugin on Linux, you must have [VLC](https://www.videolan.org) & [libVLC](https://www.videolan.org/vlc/libvlc.html) installed. On debian based distros, run:

```bash
sudo apt-get install vlc
```
```bash
sudo apt-get install libvlc-dev
```

## Example

You can see an example project [here](https://github.com/alexmercerind/dart_vlc/blob/master/example/lib/main.dart).

Windows

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_0.PNG?raw=true)


## Workings

The internal wrapper used in the plugin is [here](https://github.com/alexmercerind/dart_vlc/tree/master/dartvlc) in the repository (Based on libVLC++). It makes handling of events and controls a lot easier & has additional features to it.

Same wrapper will be used for upcoming FFI version.

I preferred to do majority of plugin handling in C++ itself, thus Dart code is minimal & very slight mapping to it.

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
- Embedding `Video` inside the Flutter window.
- Supporting live streaming links.
- `Broadcast` class for broadcasting `Media`.
- `Record` class for recording `Media`.

Under progress or planned features (irrespective of order)...

- FFI version of the library for plain Dart applications.
- Removing [libVLC++](https://github.com/videolan/libvlcpp) dependency. (Maybe).
- Writing metadata tags.
- Making things more efficient.
- Supporting native volume control/lock screen notifications (Maybe).
- Bringing project on other platforms like Android/iOS (Maybe).
- Adding headers for MRLs (Maybe).
- D-Bus MPRIS controls for `Media` playback control (Maybe).

## Acknowledgements

First of all, thanks to the [VideoLAN](https://www.videolan.org) team for creating [libVLC](https://github.com/videolan/vlc) & [libVLC++](https://github.com/videolan/libvlcpp). Really great guys really great at their work.

Thanks to following members of libVLC community to give me bit of look & advice about how things work:

- [@jeremyVignelles](https://github.com/jeremyVignelles)
- [@chouquette](https://github.com/chouquette)
- [@mfkl](https://github.com/mfkl)
- [@caprica](https://github.com/caprica)


## Contributions

The code in the project is nicely arranged (I guess), I have added comments wherever I felt necessary.

Contributions to the project are open, it will be appreciated if you discuss the bug-fix/feature-addition in the issues first.

## License

Copyright (C) 2021, Hitesh Kumar Saini.

This library & work under this repository is licensed under GNU Lesser General Public License v2.1.

## Vision

There aren't any media (audio or video) playback libraries for Flutter on Windows/Linux yet. So, this project is all about that.
As one might be already aware, VLC is one of the best media playback tools out there.

So, now you can use it to play audio or video files from Flutter Desktop app.

The API style of this project is highly influenced by [assets_audio_player](https://github.com/florent37/Flutter-AssetsAudioPlayer) due to its ease of use. This project will serve as a base to add Windows & Linux support to already existing audio playback libraries like [just_audio](https://github.com/ryanheise/just_audio) and [assets_audio_player](https://github.com/florent37/Flutter-AssetsAudioPlayer).

Although, the mentioned repositories above are for audio playback, video playback is also a part of consideration for this project.
