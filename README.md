<h1 align="center"><a href="https://github.com/alexmercerind/dart_vlc">dart_vlc</a></h1>
<h4 align="center">Flutter 🎞 media playback, broadcast, recording & chromecast library for Windows & Linux.</h4>
<h5 align="center">Written in C++ using libVLC & libVLC++.</h5>

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_windows_11_1.PNG?raw=true)

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_windows_11_2.PNG?raw=true)

## Installation

**Flutter**

```yaml
dependencies:
  ...
  dart_vlc: ^0.1.1
```

**Dart CLI**

```yaml
dependencies:
  ...
  dart_vlc_ffi: ^0.1.0
```

More on Dart CLI implementation [here](./ffi/README.md).

Feel free to open issue, incase you find something to be not working.

## Support

Consider supporting the project by starring the repository or buying me a coffee.

<a href="https://www.buymeacoffee.com/alexmercerind"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=alexmercerind&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff"></a>

Thanks a lot for your support.

Looking for contributors for macOS port.

## Documentation

#### Initialize the library

```dart
void main() {
  DartVLC.initialize();
  runApp(MyApp());
}
```

#### Create a new player instance.
```dart
Player player = Player(id: 69420);
```

#### Create a media for playback.
```dart
Media media0 = Media.file(
  File('C:/music.mp3')
);

Media media1 = Media.network(
  'https://www.example.com/music.aac'
);
```

#### Create a list of medias using playlist.
```dart
Playlist playlist = new Playlist(
  medias: [
    Media.file(File('C:/music.mp3')),
    Media.file(File('C:/audio.mp3')),
    Media.network('https://www.example.com/music.aac'),
  ],
);
```

#### Open a media or playlist into a player.

```dart
player.open(
  Media.file(File('C:/music0.mp3')),
  autoStart: true, // default
);
```

```dart
player.open(
  Playlist(
    medias: [
      Media.file(new File('C:/music0.mp3')),
      Media.file(new File('C:/music1.mp3')),
      Media.file(new File('C:/music2.mp3')),
    ],
  ),
  autoStart: false,
);
```

#### Control playback.
```dart
player.play();

player.seek(Duration(seconds: 30));

player.pause();

player.playOrPause();

player.stop();
```

#### Traverse through the playlist.
```dart
player.next();

player.back();

player.jump(10);
```

#### Manipulate an already playing playlist.
```dart
player.add(
  Media.file(File('C:/music0.mp3')),
);

player.remove(4);

player.insert(
  2,
  Media.file(File('C:/music0.mp3')),
);

player.move(0, 4);
```

#### Set playback volume & rate.
```dart
player.setVolume(0.5);

player.setRate(1.25);
```

#### Get & change playback device.
```dart
List<Device> devices = Devices.all;

player.setDevice(
  devices[0],
);
```

#### Show the video inside widget tree.

Instanciate `Player` as follows.
```dart
Player player = Player(
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

Thanks to [@tomassasovsky](https://github.com/tomassasovsky) for adding visual controls to `Video` widget.

#### Retrieve metadata of media.
```dart
Media media = Media.network(
  'https://www.example.com/media.mp3',
  parse: true,
  timeout: Duration(seconds: 10),
);

Map<String, String> metas = media.metas;
```

#### Listen to playback events.

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

#### Set an equalizer.

Create using preset.

```dart
Equalizer equalizer = Equalizer.createMode(EqualizerMode.party);
player.setEqualizer(equalizer);
```

Create custom equalizer.

```dart
Equalizer equalizer = Equalizer.createEmpty();
equalizer.setPreAmp(10.0);
equalizer.setBandAmp(31.25, -10.0);
equalizer.setBandAmp(100.0, -10.0);
player.setEqualizer(equalizer);
```

Get equalizer state.

```dart
equalizer.preAmp;
equalizer.bandAmps;
```

#### Broadcast a media.

Broadcasting to localhost.

```dart
Broadcast broadcast = Broadcast.create(
  id: 0,
  media: Media.file(File('C:/video.mp4')),
  configuration: BroadcastConfiguration(
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

#### Record a media.

Thanks to [@DomingoMG](https://github.com/DomingoMG) for adding `Record` and `Chromecast` classes.

```dart
Record record = Record.create(
  id: 205, 
  media: Media.network('https://www.example.com/streaming-media.MP3'), 
  pathFile: '/home/alexmercerind/recording.MP3',
);
record.start();
```

**NOTE:** For using this plugin on Linux, you must have [VLC](https://www.videolan.org) & [libVLC](https://www.videolan.org/vlc/libvlc.html) installed. 
On debian based distros, run:

```bash
sudo apt-get install vlc
```
```bash
sudo apt-get install libvlc-dev
```
On Fedora, enable RPMFusion repositories first:

```bash
sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm
```
Then run:

```bash
sudo dnf install vlc
```
```bash
sudo dnf install vlc-devel
```

## Example

You can see an example project [here](https://github.com/alexmercerind/dart_vlc/blob/master/example/lib/main.dart).

Windows

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_6.png?raw=true)


## Workings
The repository contains a [C++ wrapper](https://github.com/alexmercerind/dart_vlc/tree/master/dartvlc) based on libVLC++. This makes handling of events and controls a lot easier & has additional features in it.
I preferred to do majority of handling in C++ itself, thus Dart code is minimal & very slight mapping to it.

This project might seem like a Flutter plugin, but it is based on FFI instead. [Here](https://github.com/alexmercerind/dart_vlc/tree/master/ffi) are the FFI bindings to [C++ wrapper](https://github.com/alexmercerind/dart_vlc/tree/master/dartvlc), which are shared by all platforms & same can be used in Dart CLI apps aswell.

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
- `Chromecast` class.
- `Equalizer` support.
- Adding headers for `Media.network` (Not possible, added user agent).
- Switching to FFI for more cross platform freedom.

Under progress or planned features (irrespective of order)...

- Removing [libVLC++](https://github.com/videolan/libvlcpp) dependency. (Maybe).
- Writing metadata tags.
- Making things more efficient.
- Supporting native volume control/lock screen notifications (Maybe).
- Bringing project on other platforms like Android/iOS (Maybe).
- D-Bus MPRIS controls for `Media` playback control (Maybe).

## Acknowledgements

First of all, thanks to the [VideoLAN](https://www.videolan.org) team for creating [libVLC](https://github.com/videolan/vlc) & [libVLC++](https://github.com/videolan/libvlcpp). Really great guys really great at their work.

Massive thanks to [@stuartmorgan](https://github.com/stuartmorgan) from [Flutter](https://flutter.dev) team to my review code & help me fix the loopholes.

Thanks to following members of libVLC community to give me bit of look & advice about how things work:

- [@jeremyVignelles](https://github.com/jeremyVignelles)
- [@chouquette](https://github.com/chouquette)
- [@mfkl](https://github.com/mfkl)
- [@caprica](https://github.com/caprica)



## Contributions

The code in the project is nicely arranged (I guess), I have added comments wherever I felt necessary.

Contributions to the project are open, it will be appreciated if you discuss the bug-fix/feature-addition in the issues first.

## License

Copyright (C) 2021, Hitesh Kumar Saini <saini123hitesh@gmail.com>.

This library & work under this repository is licensed under GNU Lesser General Public License v2.1.

## Vision

There aren't any media (audio or video) playback libraries for Flutter or Dart on Windows/Linux yet. So, this project is all about that.
As one might be already aware, VLC is one of the best media playback tools out there.

So, now you can use it to play audio or video files from Flutter or Dart apps.

Support for other platforms is also under progress.
