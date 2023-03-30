<h1 align="center"><a href="https://github.com/alexmercerind/dart_vlc">dart_vlc</a></h1>
<p align="center">Flutter audio / video playback, broadcast & recording library for Windows & Linux.</p>

<br />

<p align="center">
  <strong>Sponsored with 💖 by</strong>
  <br>
  <a href="https://getstream.io/chat/sdk/flutter/?utm_source=alexmercerind_dart&utm_medium=Github_Repo_Content_Ad&utm_content=Developer&utm_campaign=alexmercerind_December2022_FlutterSDK_klmh22" target="_blank">
    <picture>
      <source media="(prefers-color-scheme: dark)" srcset="https://user-images.githubusercontent.com/28951144/204903234-4a64b63c-2fc2-4eef-be44-d287d27021e5.svg">
      <source media="(prefers-color-scheme: light)" srcset="https://user-images.githubusercontent.com/28951144/204903022-bbaa49ca-74c2-4a8f-a05d-af8314bfd2cc.svg">
      <img alt="Stream Chat" width="350" height="auto" src="https://user-images.githubusercontent.com/28951144/204903022-bbaa49ca-74c2-4a8f-a05d-af8314bfd2cc.svg">
    </picture>
  </a>
  <br>
    <h5 align="center">
      Rapidly ship in-app messaging with Stream's highly reliable chat infrastructure and feature-rich SDKs, including Flutter!
    </h5>
  <h4 align="center">
    <a href="https://getstream.io/chat/sdk/flutter/?utm_source=alexmercerind_dart&utm_medium=Github_Repo_Content_Ad&utm_content=Developer&utm_campaign=alexmercerind_December2022_FlutterSDK_klmh22" target="_blank">
    Try the Flutter Chat tutorial
    </a>
  </h4>
</p>

<br />

<img src='https://alexmercerind.github.io/img/dart_vlc/0.webp'></img>
<img src='https://alexmercerind.github.io/img/dart_vlc/1.webp'></img>

## Installation

_pub.dev_

```yaml
dependencies:
  ...
  dart_vlc: ^0.1.9
```

_GitHub_

```yaml
dependencies:
  dart_vlc:
    git:
      url: https://github.com/alexmercerind/dart_vlc.git
      ref: master

dependency_overrides:
  dart_vlc_ffi:
    git:
      url: https://github.com/alexmercerind/dart_vlc.git
      ref: master
      path: ffi
```

Feel free to open a [new issue](https://github.com/alexmercerind/dart_vlc/issues) or [discussion](https://github.com/alexmercerind/dart_vlc/discussions), if you found a bug or need assistance.

## Documentation

Checkout [Setup](#setup) section to configure plugin on your platform.

#### Initialize the library.

```dart
void main() {
  DartVLC.initialize();
  runApp(MyApp());
}
```

#### Create a new player instance.

```dart
final player = Player(id: 69420);
```

For passing VLC CLI arguments, use `commandlineArguments` argument.

```dart
final player = Player(
  id: 69420,
  commandlineArguments: ['--no-video'],
);
```

#### Create a media for playback.

```dart
final file = Media.file(File('C:/music.mp3'));
final asset = Media.asset('assets/audio/example.mp3');
final network = Media.network('https://www.example.com/music.aac');

// Clip [Media] playback duration.
final media2 = Media.network(
  'https://www.example.com/music.aac',
  startTime: Duration(seconds: 20),
  stopTime: Duration(seconds: 60),
);
```

#### Create a list of medias using playlist.

```dart
final playlist = Playlist(
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
      Media.file(File('C:/music0.mp3')),
      Media.file(File('C:/music1.mp3')),
      Media.file(File('C:/music2.mp3')),
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

#### Controls the playlist.

```dart
player.next();

player.previous();

player.jumpToIndex(10);
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

player.setDevice(devices[0]);
```

#### Save the video screenshot

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
  videoDimensions: const VideoDimensions(640, 360),
);
```

#### Change user agent.

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
final broadcast = Broadcast.create(
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

```dart
final record = Record.create(
  id: 205,
  media: Media.network('https://www.example.com/streaming-media.MP3'),
  pathFile: '/home/alexmercerind/recording.MP3',
);
record.start();
```

## Setup

### Windows

Everything is already set up.

<!--

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

-->

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

<!--

### iOS [WIP]

Disable bitcode generation for the whole project for MobileVLC to work.
Add the following to the `post_install` function living in the `Podfile` of your iOS Flutter project. For reference look at the `Podfile` in the example project.

```ruby
target.build_configurations.each do |config|
    config.build_settings['ENABLE_BITCODE'] = 'NO'
end
```

For the example project to work you need to configure a real device in the xcode project, or comment out the build script `Build Device lib` in in `ios/dart_vlc.podspec`.

-->

## Support

Consider sponsoring this project. Maintenance of open-source software & libraries is severely under-paid or not paid at all.

Writing C++ & native code is a very tedious process.

- [PayPal](https://paypal.me/alexmercerind)
- [Patreon](https://patreon.com/harmonoid)
- [GitHub Sponsors](https://github.com/sponsors/alexmercerind)

## Acknowledgements

- BIG thanks to [@jnschulze](https://github.com/jnschulze) for his awesome contributions to this project & to the Flutter engine itself like adding texture support & adding macOS support here. I have learnt a lot about modern C++ & good-practices when writing code from you, thanks a lot for your guidances, reviews, work & donation. I'm really really thankful to you.
- BIG thanks to [@DomingoMG](https://github.com/DomingoMG) for adding `Record` and `Chromecast` classes. Also, thanks a lot for donation to the project & giving me motivation to start building this. I would've never attempted this if you didn't motivate me. And now that it started, `dart_vlc` has grown a lot.
- Thanks to [@tomassasovsky](https://github.com/tomassasovsky) for adding visual controls to `Video` widget.

- Thanks to following members of libVLC community (irrespective of the order) for giving general ideas about libVLC APIs:
  - [@jeremyVignelles](https://github.com/jeremyVignelles)
  - [@chouquette](https://github.com/chouquette)
  - [@mfkl](https://github.com/mfkl)
  - [@caprica](https://github.com/caprica)
- Finally, thanks to the [VideoLAN](https://www.videolan.org) team for creating [libVLC](https://github.com/videolan/vlc) & [libVLC++](https://github.com/videolan/libvlcpp). Really great guys really great at their work.

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

## Example

You can see an example project [here](https://github.com/alexmercerind/dart_vlc/blob/master/example/lib/main.dart).

![](https://github.com/alexmercerind/dart_vlc/blob/assets/dart_vlc_6.png?raw=true)

`dart_vlc` running on Ubuntu Linux.

## Features

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
- Event streams.
- `add`/`insert`/`remove`/`move` `Media` inside `Playlist` during playback with no interruption.
- Device enumeration & changing.
- Retrieving metadata/tags of a `Media`.
- Embedding `Video` inside the Flutter window (using texture or natively).
- Supporting live streaming links.
- `Broadcast` class for broadcasting `Media`.
- `Record` class for recording `Media`.
- `Chromecast` class.
- `Equalizer` configuration & presets.
- Changing user agent.
- Changing `Video`'s frame size according to video.
- Saving screenshot of the video.
- Changing/retrieving audio track.
- Media clipping.
- Support for Windows, Linux or macOS.

Under progress or planned features (irrespective of order)...

- Bringing project on Android and iOS.
- Removing [libVLC++](https://github.com/videolan/libvlcpp) dependency.
- Subtitle control.
