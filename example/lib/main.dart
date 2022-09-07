import 'dart:io';
import 'dart:ui' as ui;
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:dart_vlc/dart_vlc.dart';

void main() async {
  await DartVLC.initialize(useFlutterNativeView: true);
  runApp(DartVLCExample());
}

class DartVLCExample extends StatefulWidget {
  @override
  DartVLCExampleState createState() => DartVLCExampleState();
}

class DartVLCExampleState extends State<DartVLCExample> {
  Player player = Player(
    id: 0,
    videoDimensions: VideoDimensions(640, 360),
    registerTexture: !Platform.isWindows,
  );
  MediaType mediaType = MediaType.file;
  CurrentState current = CurrentState();
  PositionState position = PositionState();
  PlaybackState playback = PlaybackState();
  GeneralState general = GeneralState();
  VideoDimensions videoDimensions = VideoDimensions(0, 0);
  List<Media> medias = <Media>[];
  List<Device> devices = <Device>[];
  TextEditingController controller = TextEditingController();
  TextEditingController metasController = TextEditingController();
  double bufferingProgress = 0.0;
  Media? metasMedia;

  @override
  void initState() {
    super.initState();
    if (this.mounted) {
      this.player.currentStream.listen((current) {
        this.setState(() => this.current = current);
      });
      this.player.positionStream.listen((position) {
        this.setState(() => this.position = position);
      });
      this.player.playbackStream.listen((playback) {
        this.setState(() => this.playback = playback);
      });
      this.player.generalStream.listen((general) {
        this.setState(() => this.general = general);
      });
      this.player.videoDimensionsStream.listen((videoDimensions) {
        this.setState(() => this.videoDimensions = videoDimensions);
      });
      this.player.bufferingProgressStream.listen(
        (bufferingProgress) {
          this.setState(() => this.bufferingProgress = bufferingProgress);
        },
      );
      this.player.errorStream.listen((event) {
        print('libvlc error.');
      });
      this.devices = Devices.all;
      Equalizer equalizer = Equalizer.createMode(EqualizerMode.live);
      equalizer.setPreAmp(10.0);
      equalizer.setBandAmp(31.25, 10.0);
      this.player.setEqualizer(equalizer);
    }
  }

  @override
  Widget build(BuildContext context) {
    bool isTablet;
    bool isPhone;
    final double devicePixelRatio = ui.window.devicePixelRatio;
    final double width = ui.window.physicalSize.width;
    final double height = ui.window.physicalSize.height;
    if (devicePixelRatio < 2 && (width >= 1000 || height >= 1000)) {
      isTablet = true;
      isPhone = false;
    } else if (devicePixelRatio == 2 && (width >= 1920 || height >= 1920)) {
      isTablet = true;
      isPhone = false;
    } else {
      isTablet = false;
      isPhone = true;
    }
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('dart_vlc'),
          centerTitle: true,
        ),
        body: ListView(
          shrinkWrap: true,
          padding: const EdgeInsets.all(4.0),
          children: [
            Row(
              mainAxisSize: MainAxisSize.min,
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Platform.isWindows
                    ? NativeVideo(
                        player: player,
                        width: isPhone ? 320 : 640,
                        height: isPhone ? 180 : 360,
                        volumeThumbColor: Colors.blue,
                        volumeActiveColor: Colors.blue,
                        showControls: !isPhone,
                      )
                    : Video(
                        player: player,
                        width: isPhone ? 320 : 640,
                        height: isPhone ? 180 : 360,
                        volumeThumbColor: Colors.blue,
                        volumeActiveColor: Colors.blue,
                        showControls: !isPhone,
                      ),
              ],
            ),
            Row(
              mainAxisSize: MainAxisSize.min,
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Expanded(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.start,
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      if (isPhone) _controls(context, isPhone),
                      Card(
                        elevation: 2.0,
                        margin: const EdgeInsets.all(4.0),
                        child: Container(
                          margin: const EdgeInsets.all(16.0),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: <Widget>[
                                  const Text('Playlist creation.'),
                                  Divider(
                                    height: 8.0,
                                    color: Colors.transparent,
                                  ),
                                  Row(
                                    mainAxisAlignment: MainAxisAlignment.start,
                                    children: [
                                      Expanded(
                                        child: TextField(
                                          controller: this.controller,
                                          cursorWidth: 1.0,
                                          autofocus: true,
                                          style: const TextStyle(
                                            fontSize: 14.0,
                                          ),
                                          decoration: InputDecoration.collapsed(
                                            hintStyle: const TextStyle(
                                              fontSize: 14.0,
                                            ),
                                            hintText: 'Enter Media path.',
                                          ),
                                        ),
                                      ),
                                      Container(
                                        width: 152.0,
                                        child: DropdownButton<MediaType>(
                                          value: this.mediaType,
                                          onChanged: (mediaType) => this
                                              .setState(() =>
                                                  this.mediaType = mediaType!),
                                          items: [
                                            DropdownMenuItem<MediaType>(
                                              value: MediaType.file,
                                              child: Text(
                                                MediaType.file.toString(),
                                                style: const TextStyle(
                                                  fontSize: 14.0,
                                                ),
                                              ),
                                            ),
                                            DropdownMenuItem<MediaType>(
                                              value: MediaType.network,
                                              child: Text(
                                                MediaType.network.toString(),
                                                style: const TextStyle(
                                                  fontSize: 14.0,
                                                ),
                                              ),
                                            ),
                                            DropdownMenuItem<MediaType>(
                                              value: MediaType.asset,
                                              child: Text(
                                                MediaType.asset.toString(),
                                                style: const TextStyle(
                                                  fontSize: 14.0,
                                                ),
                                              ),
                                            ),
                                          ],
                                        ),
                                      ),
                                      Padding(
                                        padding: EdgeInsets.only(left: 10.0),
                                        child: ElevatedButton(
                                          onPressed: () {
                                            if (this.mediaType ==
                                                MediaType.file) {
                                              this.medias.add(
                                                    Media.file(
                                                      File(
                                                        controller.text
                                                            .replaceAll(
                                                                '"', ''),
                                                      ),
                                                    ),
                                                  );
                                            } else if (this.mediaType ==
                                                MediaType.network) {
                                              this.medias.add(
                                                    Media.network(
                                                      controller.text,
                                                    ),
                                                  );
                                            }
                                            this.setState(() {});
                                          },
                                          child: Text(
                                            'Add to Playlist',
                                            style: TextStyle(
                                              fontSize: 14.0,
                                            ),
                                          ),
                                        ),
                                      ),
                                    ],
                                  ),
                                  const Divider(
                                    height: 12.0,
                                  ),
                                  const Divider(
                                    height: 8.0,
                                    color: Colors.transparent,
                                  ),
                                  const Text('Playlist'),
                                ] +
                                this
                                    .medias
                                    .map(
                                      (media) => ListTile(
                                        title: Text(
                                          media.resource,
                                          style: const TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                        subtitle: Text(
                                          media.mediaType.toString(),
                                          style: const TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                      ),
                                    )
                                    .toList() +
                                <Widget>[
                                  const Divider(
                                    height: 8.0,
                                    color: Colors.transparent,
                                  ),
                                  Row(
                                    children: [
                                      ElevatedButton(
                                        onPressed: () => this.setState(
                                          () {
                                            this.player.open(
                                                  Playlist(
                                                    medias: this.medias,
                                                    playlistMode:
                                                        PlaylistMode.single,
                                                  ),
                                                );
                                          },
                                        ),
                                        child: Text(
                                          'Open into Player',
                                          style: const TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                      ),
                                      const SizedBox(width: 12.0),
                                      ElevatedButton(
                                        onPressed: () {
                                          this.setState(
                                              () => this.medias.clear());
                                        },
                                        child: Text(
                                          'Clear the list',
                                          style: const TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                      ),
                                    ],
                                  ),
                                ],
                          ),
                        ),
                      ),
                      Card(
                        elevation: 2.0,
                        margin: const EdgeInsets.all(4.0),
                        child: Container(
                          margin: const EdgeInsets.all(16.0),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              const Text('Playback event listeners.'),
                              const Divider(
                                height: 12.0,
                                color: Colors.transparent,
                              ),
                              const Divider(
                                height: 12.0,
                              ),
                              const Text('Playback position.'),
                              const Divider(
                                height: 8.0,
                                color: Colors.transparent,
                              ),
                              Slider(
                                min: 0,
                                max: this
                                        .position
                                        .duration
                                        ?.inMilliseconds
                                        .toDouble() ??
                                    1.0,
                                value: this
                                        .position
                                        .position
                                        ?.inMilliseconds
                                        .toDouble() ??
                                    0.0,
                                onChanged: (double position) =>
                                    this.player.seek(
                                          Duration(
                                            milliseconds: position.toInt(),
                                          ),
                                        ),
                              ),
                              const Text('Event streams.'),
                              const Divider(
                                height: 8.0,
                                color: Colors.transparent,
                              ),
                              Table(
                                children: [
                                  TableRow(
                                    children: [
                                      const Text('player.general.volume'),
                                      Text('${this.general.volume}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.general.rate'),
                                      Text('${this.general.rate}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.position.position'),
                                      Text('${this.position.position}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.position.duration'),
                                      Text('${this.position.duration}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.playback.isCompleted'),
                                      Text('${this.playback.isCompleted}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.playback.isPlaying'),
                                      Text('${this.playback.isPlaying}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.playback.isSeekable'),
                                      Text('${this.playback.isSeekable}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.current.index'),
                                      Text('${this.current.index}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.current.media'),
                                      Text('${this.current.media}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.current.medias'),
                                      Text('${this.current.medias}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.videoDimensions'),
                                      Text('${this.videoDimensions}')
                                    ],
                                  ),
                                  TableRow(
                                    children: [
                                      const Text('player.bufferingProgress'),
                                      Text('${this.bufferingProgress}')
                                    ],
                                  ),
                                ],
                              ),
                            ],
                          ),
                        ),
                      ),
                      Card(
                        elevation: 2.0,
                        margin: const EdgeInsets.all(4.0),
                        child: Container(
                          margin: const EdgeInsets.all(16.0),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: const [
                                  Text('Playback devices.'),
                                  Divider(
                                    height: 12.0,
                                    color: Colors.transparent,
                                  ),
                                  Divider(
                                    height: 12.0,
                                  ),
                                ] +
                                this
                                    .devices
                                    .map(
                                      (device) => ListTile(
                                        title: Text(
                                          device.name,
                                          style: const TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                        subtitle: Text(
                                          device.id,
                                          style: const TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                        onTap: () =>
                                            this.player.setDevice(device),
                                      ),
                                    )
                                    .toList(),
                          ),
                        ),
                      ),
                      Card(
                        elevation: 2.0,
                        margin: const EdgeInsets.all(4.0),
                        child: Container(
                          margin: const EdgeInsets.all(16.0),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              const Text('Metas parsing.'),
                              Row(
                                mainAxisAlignment: MainAxisAlignment.start,
                                children: [
                                  Expanded(
                                    child: TextField(
                                      controller: this.metasController,
                                      cursorWidth: 1.0,
                                      autofocus: true,
                                      style: const TextStyle(
                                        fontSize: 14.0,
                                      ),
                                      decoration: InputDecoration.collapsed(
                                        hintStyle: const TextStyle(
                                          fontSize: 14.0,
                                        ),
                                        hintText: 'Enter Media path.',
                                      ),
                                    ),
                                  ),
                                  Container(
                                    width: 152.0,
                                    child: DropdownButton<MediaType>(
                                      value: this.mediaType,
                                      onChanged: (mediaType) => this.setState(
                                          () => this.mediaType = mediaType!),
                                      items: [
                                        DropdownMenuItem<MediaType>(
                                          value: MediaType.file,
                                          child: Text(
                                            MediaType.file.toString(),
                                            style: const TextStyle(
                                              fontSize: 14.0,
                                            ),
                                          ),
                                        ),
                                        DropdownMenuItem<MediaType>(
                                          value: MediaType.network,
                                          child: Text(
                                            MediaType.network.toString(),
                                            style: const TextStyle(
                                              fontSize: 14.0,
                                            ),
                                          ),
                                        ),
                                        DropdownMenuItem<MediaType>(
                                          value: MediaType.asset,
                                          child: Text(
                                            MediaType.asset.toString(),
                                            style: const TextStyle(
                                              fontSize: 14.0,
                                            ),
                                          ),
                                        ),
                                      ],
                                    ),
                                  ),
                                  Padding(
                                    padding: EdgeInsets.only(left: 16.0),
                                    child: ElevatedButton(
                                      onPressed: () {
                                        if (this.mediaType == MediaType.file) {
                                          this.metasMedia = Media.file(
                                            File(this.metasController.text),
                                            parse: true,
                                          );
                                        } else if (this.mediaType ==
                                            MediaType.network) {
                                          this.metasMedia = Media.network(
                                            this.metasController.text,
                                            parse: true,
                                          );
                                        }
                                        this.setState(() {});
                                      },
                                      child: const Text(
                                        'Parse',
                                        style: TextStyle(
                                          fontSize: 14.0,
                                        ),
                                      ),
                                    ),
                                  ),
                                ],
                              ),
                              const Divider(
                                height: 12.0,
                              ),
                              const Divider(
                                height: 8.0,
                                color: Colors.transparent,
                              ),
                              Text(
                                JsonEncoder.withIndent('    ')
                                    .convert(this.metasMedia?.metas),
                              ),
                            ],
                          ),
                        ),
                      ),
                      if (isPhone) _playlist(context),
                    ],
                  ),
                ),
                if (isTablet)
                  Expanded(
                    child: Column(
                      mainAxisAlignment: MainAxisAlignment.start,
                      mainAxisSize: MainAxisSize.min,
                      children: [
                        _controls(context, isPhone),
                        _playlist(context),
                      ],
                    ),
                  ),
              ],
            )
          ],
        ),
      ),
    );
  }

  Widget _controls(BuildContext context, bool isPhone) {
    return Card(
      elevation: 2.0,
      margin: const EdgeInsets.all(4.0),
      child: Container(
        margin: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text('Playback controls.'),
            const Divider(
              height: 8.0,
              color: Colors.transparent,
            ),
            Row(
              children: [
                ElevatedButton(
                  onPressed: () => this.player.play(),
                  child: const Text(
                    'play',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => this.player.pause(),
                  child: const Text(
                    'pause',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => this.player.playOrPause(),
                  child: const Text(
                    'playOrPause',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
              ],
            ),
            const SizedBox(
              height: 8.0,
            ),
            Row(
              children: [
                ElevatedButton(
                  onPressed: () => this.player.stop(),
                  child: const Text(
                    'stop',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => this.player.next(),
                  child: const Text(
                    'next',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => this.player.previous(),
                  child: const Text(
                    'previous',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
              ],
            ),
            const Divider(
              height: 12.0,
              color: Colors.transparent,
            ),
            const Divider(
              height: 12.0,
            ),
            const Text('Volume control.'),
            const Divider(
              height: 8.0,
              color: Colors.transparent,
            ),
            Slider(
              min: 0.0,
              max: 1.0,
              value: this.player.general.volume,
              onChanged: (volume) {
                this.player.setVolume(volume);
                this.setState(() {});
              },
            ),
            const Text('Playback rate control.'),
            const Divider(
              height: 8.0,
              color: Colors.transparent,
            ),
            Slider(
              min: 0.5,
              max: 1.5,
              value: this.player.general.rate,
              onChanged: (rate) {
                this.player.setRate(rate);
                this.setState(() {});
              },
            ),
          ],
        ),
      ),
    );
  }

  Widget _playlist(BuildContext context) {
    return Card(
      elevation: 2.0,
      margin: const EdgeInsets.all(4.0),
      child: Container(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Container(
              margin: const EdgeInsets.only(left: 16.0, top: 16.0),
              alignment: Alignment.topLeft,
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: const [
                  Text('Playlist manipulation.'),
                  Divider(
                    height: 12.0,
                    color: Colors.transparent,
                  ),
                  Divider(
                    height: 12.0,
                  ),
                ],
              ),
            ),
            Container(
              height: 456.0,
              child: ReorderableListView(
                shrinkWrap: true,
                onReorder: (int initialIndex, int finalIndex) async {
                  /// 🙏🙏🙏
                  /// In the name of God,
                  /// With all due respect,
                  /// I ask all Flutter engineers to please fix this issue.
                  /// Peace.
                  /// 🙏🙏🙏
                  ///
                  /// Issue:
                  /// https://github.com/flutter/flutter/issues/24786
                  /// Prevention:
                  /// https://stackoverflow.com/a/54164333/12825435
                  ///
                  if (finalIndex > this.current.medias.length)
                    finalIndex = this.current.medias.length;
                  if (initialIndex < finalIndex) finalIndex--;

                  this.player.move(initialIndex, finalIndex);
                  this.setState(() {});
                },
                scrollDirection: Axis.vertical,
                padding: const EdgeInsets.symmetric(vertical: 8.0),
                children: List.generate(
                  this.current.medias.length,
                  (int index) => ListTile(
                    key: Key(index.toString()),
                    leading: Text(
                      index.toString(),
                      style: const TextStyle(fontSize: 14.0),
                    ),
                    title: Text(
                      this.current.medias[index].resource,
                      style: const TextStyle(fontSize: 14.0),
                    ),
                    subtitle: Text(
                      this.current.medias[index].mediaType.toString(),
                      style: const TextStyle(fontSize: 14.0),
                    ),
                  ),
                  growable: true,
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
