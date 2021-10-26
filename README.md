<h1 align="center"><a href="https://github.com/alexmercerind/dart_vlc">dart_vlc</a></h1>

[![pub package](https://img.shields.io/pub/v/dart_vlc.svg)](https://pub.dartlang.org/packages/dart_vlc) ![CI/CD](https://github.com/alexmercerind/dart_vlc/actions/workflows/ci.yml/badge.svg?branch=master) [![Donate](https://img.shields.io/badge/Donate-PayPal-blue.svg)](https://paypal.me/alexmercerind) [![](https://img.shields.io/twitter/follow/alexmercerind)](https://twitter.com/alexmercerind) [![Join the chat at https://discord.gg/3h3K3JF](https://img.shields.io/discord/716939396464508958?label=discord)](https://discord.gg/3h3K3JF)


<h4 align="center">Flutter media playback, broadcast, recording & chromecast library for Windows, Linux & macOS.</h4>
<h5 align="center">Written in C++ using libVLC & libVLC++.</h5>

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_windows_11_1.PNG?raw=true)

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_windows_11_2.PNG?raw=true)

## Installation

**Flutter**

```yaml
dependencies:
  ...
  dart_vlc: ^0.1.6
```

**Dart CLI**

```yaml
dependencies:
  ...
  dart_vlc_ffi: ^0.1.3
```

More on Dart CLI implementation [here](./ffi/README.md).

Feel free to open a [new issue](https://github.com/alexmercerind/dart_vlc/issues) or [discussion](https://github.com/alexmercerind/dart_vlc/discussions), if you found a bug or need assistance.

## Support

Consider supporting the project by starring the repository or buying me a coffee.

[![Donate](https://img.shields.io/badge/Donate-PayPal-blue.svg)](https://paypal.me/alexmercerind)
[![Donate](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow)](https://buymeacoffee.com/alexmercerind)

Thanks a lot for your support. Android support is on its way.

## Documentation

Checkout [Setup](#setup) section to configure plugin on your platform.

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

For passing VLC CLI arguments, use `commandlineArguments` argument.

```dart
Player player = Player(
  id: 69420,
  commandlineArguments: ['--no-video']
);
```

#### Create a media for playback.
```dart
Media media0 = Media.file(
  File('C:/music.mp3')
);

Media media1 = Media.asset(
  'assets/audio/example.mp3'
);

Media media2 = Media.network(
  'https://www.example.com/music.aac'
);

// Clip the media.
Media media2 = Media.network(
  'https://www.example.com/music.aac',
  startTime: Duration(seconds: 20), // Start media from 20 seconds from the beginning.
  stopTime: Duration(seconds: 60), // End media at 60 seconds from the beginning. 
);
```

#### Create a list of medias using playlist.
```dart
Playlist playlist = new Playlist(
  medias: [
    Media.file(File('C:/music.mp3')),
    Media.file(File('C:/audio.mp3')),
    Media.asset('assets/audio/example.mp3'),
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

#### Save the video snapshot

```dart
player.takeSnapshot(file, 1920, 1080);
```

#### Show the video inside widget tree.

Show `Video` in the `Widget` tree.
```dart
class _MyAppState extends State<MyApp> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Video(
        player: player,
        height: 1920.0,
        width: 1080.0,
        scale: 1.0, // default
        showControls: false, // default
      ),
    );
  }
}
```

By default, `Video` widget's frame size will adapt to the currently playing video.
To override this & define custom video frame size, pass `videoDimensions` argument while instanciating `Player` class as follows.

```dart
Player player = Player(
  id: 69420,
  videoDimensions: const VideoDimensions(640, 360)
);
```

Thanks to [@tomassasovsky](https://github.com/tomassasovsky) for adding visual controls to `Video` widget.

#### Change user agent

```dart
player.setUserAgent(userAgent);
```

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

Listen to currently loaded media & playlist index changes.

```dart
player.currentStream.listen((CurrentState state) {
  state.index;
  state.media;
  state.medias;
  state.isPlaylist;
});
```

Listen to playback position & media duration.

```dart
player.positionStream.listen((PositionState state) {
  state.position;
  state.duration;
});
```

Listen to playback states.

```dart
player.playbackStream.listen((PlaybackState state) {
  state.isPlaying;
  state.isSeekable;
  state.isCompleted;
});
```

Listen to volume & rate of the `Player`.

```dart
player.generalStream.listen((GeneralState state) {
  state.volume;
  state.rate;
});
```

Listen to dimensions of currently playing `Video`.

```dart
player.videoDimensionsStream.listen((VideoDimensions video) {
  video.width;
  video.height;
});
```

Listen to buffering progress of the playing `Media`.

```dart
player.bufferingProgressStream.listen(
  (double event) {
    this.setState(() {
      this.bufferingProgress = event;
    });
  },
);
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

## Setup


### Windows

Everything is already set up.

### macOS

To run on macOS, install CMake through [Homebrew](https://brew.sh):

```bash
brew install cmake
```

If you encounter the error `cmake: command not found` during archiving:

1. Download [CMake](https://cmake.org/download/) and move it to the `Applications` Folder.
2. Run:
```bash
sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
```

### Linux

For using this plugin on Linux, you must have [VLC](https://www.videolan.org) & [libVLC](https://www.videolan.org/vlc/libvlc.html) installed. 

**On Ubuntu/Debian:**

```bash
sudo apt-get install vlc
```
```bash
sudo apt-get install libvlc-dev
```

**On Fedora:**

```bash
sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm
```

```bash
sudo dnf install vlc
```
```bash
sudo dnf install vlc-devel
```

### iOS [WIP]

Disable bitcode generation for the whole project for MobileVLC to work.
Add the following to the `post_install` function living in the `Podfile` of your iOS Flutter project. For reference look at the `Podfile` in the example project.

```ruby
target.build_configurations.each do |config|
    config.build_settings['ENABLE_BITCODE'] = 'NO'
end
```

For the example project to work you need to configure a real device in the xcode project, or comment out the build script `Build Device lib` in in `ios/dart_vlc.podspec`.

## Example

You can see an example project [here](https://github.com/alexmercerind/dart_vlc/blob/master/example/lib/main.dart).

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_6.png?raw=true)

dart_vlc running on Ubuntu Linux.

## Workings

The repository contains a [C++ wrapper](https://github.com/alexmercerind/dart_vlc/tree/master/dartvlc) based on libVLC++. This makes handling of events and controls a lot easier & has additional features in it.
I preferred to do majority of handling in C++ itself, thus Dart code is minimal & very slight mapping to it.

This project might seem like a Flutter plugin, but it is based on FFI instead. [Here](https://github.com/alexmercerind/dart_vlc/tree/master/ffi) are the FFI bindings to [C++ wrapper](https://github.com/alexmercerind/dart_vlc/tree/master/dartvlc), which are shared by all platforms & same can be used in Dart CLI apps aswell. Platform channel interface is only used for [flutter]

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
- Changing `Video`'s frame size according to video.
- Saving snapshot.

Under progress or planned features (irrespective of order)...

- Removing [libVLC++](https://github.com/videolan/libvlcpp) dependency. (Maybe).
- Subtitle control.
- Audio track control.
- Writing metadata tags.
- Making things more efficient.
- Supporting native volume control/lock screen notifications (Maybe).
- Bringing project on other platforms like Android/iOS (Maybe).
- D-Bus MPRIS controls for `Media` playback control (Maybe).

## Acknowledgements

First of all, thanks to the [VideoLAN](https://www.videolan.org) team for creating [libVLC](https://github.com/videolan/vlc) & [libVLC++](https://github.com/videolan/libvlcpp). Really great guys really great at their work.

Thanks to [@jnschulze](https://github.com/jnschulze) for his awesome contributions to this project & to Flutter engine itself like adding texture support.

Thanks to [@krjw-eyev](https://github.com/krjw-eyev) for working on iOS support.

Thanks to [@jnschulze](https://github.com/jnschulze) & [@namniav](https://github.com/namniav) for working on macOS support.

Thanks to [@stuartmorgan](https://github.com/stuartmorgan) from [The Flutter Team](https://flutter.dev) for helping out the project with his opinions.


Thanks to following members of libVLC community (irrespective of the order) for giving general ideas about libVLC APIs:

- [@jeremyVignelles](https://github.com/jeremyVignelles)
- [@chouquette](https://github.com/chouquette)
- [@mfkl](https://github.com/mfkl)
- [@caprica](https://github.com/caprica)



## Contributions

The code in the project is nicely arranged and follows the clean architecture.

Contributions to the project are open, it will be appreciated if you discuss the bug-fix/feature-addition in the issues first.

## License

Copyright (C) 2021, Hitesh Kumar Saini <saini123hitesh@gmail.com>.

This library & work under this repository is licensed under GNU Lesser General Public License v2.1.

## Vision

There aren't any media (audio or video) playback libraries for Flutter or Dart on Windows/Linux yet. So, this project is all about that.
As one might be already aware, VLC is one of the best media playback tools out there.

So, now you can use it to play audio or video files from Flutter or Dart apps.

As the project has grown, awesome people from community have added support for iOS & macOS aswell.
