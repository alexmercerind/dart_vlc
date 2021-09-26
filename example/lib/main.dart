import 'dart:io';
import 'dart:ui' as ui;
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:dart_vlc/dart_vlc.dart';

void main() {
  DartVLC.initialize();
  runApp(DartVLCExample());
}

class DartVLCExample extends StatefulWidget {
  @override
  DartVLCExampleState createState() => DartVLCExampleState();
}

class DartVLCExampleState extends State<DartVLCExample> {
  Player player = Player(id: 0);
  MediaType mediaType = MediaType.file;
  CurrentState current = new CurrentState();
  PositionState position = new PositionState();
  PlaybackState playback = new PlaybackState();
  GeneralState general = new GeneralState();
  VideoDimensions videoDimensions = new VideoDimensions(0, 0);
  List<Media> medias = <Media>[];
  List<Device> devices = <Device>[];
  TextEditingController controller = new TextEditingController();
  TextEditingController metasController = new TextEditingController();
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
        this.setState(() {
          this.videoDimensions = videoDimensions;
        });
      });
      this.player.bufferingProgressStream.listen(
        (bufferingProgress) {
          this.setState(() {
            this.bufferingProgress = bufferingProgress;
          });
        },
      );
    }
  }

  @override
  Future<void> didChangeDependencies() async {
    super.didChangeDependencies();
    this.devices = Devices.all;
    Equalizer equalizer = Equalizer.createMode(EqualizerMode.live);
    equalizer.setPreAmp(10.0);
    equalizer.setBandAmp(31.25, 10.0);
    this.player.setEqualizer(equalizer);
    this.setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    bool isTablet;
    bool isPhone;

    final double devicePixelRatio = ui.window.devicePixelRatio;
    final ui.Size size = ui.window.physicalSize;
    final double width = size.width;
    final double height = size.height;

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
          padding: EdgeInsets.all(4.0),
          children: [
            Row(
              mainAxisSize: MainAxisSize.min,
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Card(
                  clipBehavior: Clip.antiAlias,
                  elevation: 2.0,
                  child: Video(
                    player: player,
                    width: isPhone ? 320 : 640,
                    height: isPhone ? 240 : 480,
                    volumeThumbColor: Colors.blue,
                    volumeActiveColor: Colors.blue,
                  ),
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
                        margin: EdgeInsets.all(4.0),
                        child: Container(
                          margin: EdgeInsets.all(16.0),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: <Widget>[
                                  Text('Playlist creation.'),
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
                                          style: TextStyle(
                                            fontSize: 14.0,
                                          ),
                                          decoration: InputDecoration.collapsed(
                                            hintStyle: TextStyle(
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
                                                style: TextStyle(
                                                  fontSize: 14.0,
                                                ),
                                              ),
                                            ),
                                            DropdownMenuItem<MediaType>(
                                              value: MediaType.network,
                                              child: Text(
                                                MediaType.network.toString(),
                                                style: TextStyle(
                                                  fontSize: 14.0,
                                                ),
                                              ),
                                            ),
                                            DropdownMenuItem<MediaType>(
                                              value: MediaType.asset,
                                              child: Text(
                                                MediaType.asset.toString(),
                                                style: TextStyle(
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
                                          onPressed: () async {
                                            if (this.mediaType ==
                                                MediaType.file) {
                                              this.medias.add(
                                                    Media.file(new File(
                                                        controller.text
                                                            .replaceAll(
                                                                '"', ''))),
                                                  );
                                            } else if (this.mediaType ==
                                                MediaType.network) {
                                              this.medias.add(
                                                    Media.network(
                                                        controller.text),
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
                                  Divider(
                                    height: 12.0,
                                  ),
                                  Divider(
                                    height: 8.0,
                                    color: Colors.transparent,
                                  ),
                                  Text('Playlist'),
                                ] +
                                this
                                    .medias
                                    .map(
                                      (media) => ListTile(
                                        title: Text(
                                          media.resource,
                                          style: TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                        subtitle: Text(
                                          media.mediaType.toString(),
                                          style: TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                      ),
                                    )
                                    .toList() +
                                <Widget>[
                                  Divider(
                                    height: 8.0,
                                    color: Colors.transparent,
                                  ),
                                  Row(
                                    children: [
                                      ElevatedButton(
                                        onPressed: () => this.setState(() {
                                          this.player.open(
                                                new Playlist(
                                                    medias: this.medias,
                                                    playlistMode:
                                                        PlaylistMode.single),
                                              );
                                        }),
                                        child: Text(
                                          'Open into Player',
                                          style: TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                      ),
                                      SizedBox(width: 12.0),
                                      ElevatedButton(
                                        onPressed: () => this.setState(() {
                                          this.medias.clear();
                                        }),
                                        child: Text(
                                          'Clear the list',
                                          style: TextStyle(
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
                        margin: EdgeInsets.all(4.0),
                        child: Container(
                          margin: EdgeInsets.all(16.0),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              Text('Playback event listeners.'),
                              Divider(
                                height: 12.0,
                                color: Colors.transparent,
                              ),
                              Divider(
                                height: 12.0,
                              ),
                              Text('Playback position.'),
                              Divider(
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
                                  onChanged: (double position) => this
                                      .player
                                      .seek(Duration(
                                          milliseconds: position.toInt()))),
                              Text('Event streams.'),
                              Divider(
                                height: 8.0,
                                color: Colors.transparent,
                              ),
                              Table(
                                children: [
                                  TableRow(children: [
                                    Text('player.general.volume'),
                                    Text('${this.general.volume}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.general.rate'),
                                    Text('${this.general.rate}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.position.position'),
                                    Text('${this.position.position}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.position.duration'),
                                    Text('${this.position.duration}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.playback.isCompleted'),
                                    Text('${this.playback.isCompleted}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.playback.isPlaying'),
                                    Text('${this.playback.isPlaying}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.playback.isSeekable'),
                                    Text('${this.playback.isSeekable}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.current.index'),
                                    Text('${this.current.index}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.current.media'),
                                    Text('${this.current.media}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.current.medias'),
                                    Text('${this.current.medias}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.videoDimensions'),
                                    Text('${this.videoDimensions}')
                                  ]),
                                  TableRow(children: [
                                    Text('player.bufferingProgress'),
                                    Text('${this.bufferingProgress}')
                                  ]),
                                ],
                              ),
                            ],
                          ),
                        ),
                      ),
                      Card(
                        elevation: 2.0,
                        margin: EdgeInsets.all(4.0),
                        child: Container(
                          margin: EdgeInsets.all(16.0),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
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
                                      (device) => new ListTile(
                                        title: Text(
                                          device.name,
                                          style: TextStyle(
                                            fontSize: 14.0,
                                          ),
                                        ),
                                        subtitle: Text(
                                          device.id,
                                          style: TextStyle(
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
                        margin: EdgeInsets.all(4.0),
                        child: Container(
                          margin: EdgeInsets.all(16.0),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              Text('Metas parsing.'),
                              Row(
                                mainAxisAlignment: MainAxisAlignment.start,
                                children: [
                                  Expanded(
                                    child: TextField(
                                      controller: this.metasController,
                                      cursorWidth: 1.0,
                                      autofocus: true,
                                      style: TextStyle(
                                        fontSize: 14.0,
                                      ),
                                      decoration: InputDecoration.collapsed(
                                        hintStyle: TextStyle(
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
                                            style: TextStyle(
                                              fontSize: 14.0,
                                            ),
                                          ),
                                        ),
                                        DropdownMenuItem<MediaType>(
                                          value: MediaType.network,
                                          child: Text(
                                            MediaType.network.toString(),
                                            style: TextStyle(
                                              fontSize: 14.0,
                                            ),
                                          ),
                                        ),
                                        DropdownMenuItem<MediaType>(
                                          value: MediaType.asset,
                                          child: Text(
                                            MediaType.asset.toString(),
                                            style: TextStyle(
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
                                      onPressed: () async {
                                        if (this.mediaType == MediaType.file) {
                                          this.metasMedia = Media.file(
                                              new File(
                                                  this.metasController.text),
                                              parse: true);
                                        } else if (this.mediaType ==
                                            MediaType.network) {
                                          this.metasMedia = Media.network(
                                              this.metasController.text,
                                              parse: true);
                                        }
                                        this.setState(() {});
                                      },
                                      child: Text(
                                        'Parse',
                                        style: TextStyle(
                                          fontSize: 14.0,
                                        ),
                                      ),
                                    ),
                                  ),
                                ],
                              ),
                              Divider(
                                height: 12.0,
                              ),
                              Divider(
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
      margin: EdgeInsets.all(4.0),
      child: Container(
        margin: EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text('Playback controls.'),
            Divider(
              height: 8.0,
              color: Colors.transparent,
            ),
            Row(
              children: [
                ElevatedButton(
                  onPressed: () => this.player.play(),
                  child: Text(
                    'play',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => this.player.pause(),
                  child: Text(
                    'pause',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => this.player.playOrPause(),
                  child: Text(
                    'playOrPause',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                SizedBox(width: 12.0),
              ],
            ),
            SizedBox(
              height: 8.0,
            ),
            Row(
              children: [
                ElevatedButton(
                  onPressed: () => this.player.stop(),
                  child: Text(
                    'stop',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => this.player.next(),
                  child: Text(
                    'next',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => this.player.back(),
                  child: Text(
                    'back',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
              ],
            ),
            Divider(
              height: 12.0,
              color: Colors.transparent,
            ),
            Divider(
              height: 12.0,
            ),
            Text('Volume control.'),
            Divider(
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
            Text('Playback rate control.'),
            Divider(
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
      margin: EdgeInsets.all(4.0),
      child: Container(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Container(
              margin: EdgeInsets.only(left: 16.0, top: 16.0),
              alignment: Alignment.topLeft,
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
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
                  (int index) => new ListTile(
                    key: Key(index.toString()),
                    leading: Text(
                      index.toString(),
                      style: TextStyle(fontSize: 14.0),
                    ),
                    title: Text(
                      this.current.medias[index].resource,
                      style: TextStyle(fontSize: 14.0),
                    ),
                    subtitle: Text(
                      this.current.medias[index].mediaType.toString(),
                      style: TextStyle(fontSize: 14.0),
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
