import 'dart:io';
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:dart_vlc/dart_vlc.dart';

void main() async {
  DartVLC.initialize();
  runApp(DartVLCExample());
}

class DartVLCExample extends StatelessWidget {
  const DartVLCExample({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('package:dart_vlc'),
          centerTitle: true,
        ),
        body: PrimaryScreen(),
      ),
    );
  }
}

class PrimaryScreen extends StatefulWidget {
  @override
  PrimaryScreenState createState() => PrimaryScreenState();
}

class PrimaryScreenState extends State<PrimaryScreen> {
  Player player = Player(
    id: 0,
    videoDimensions: const VideoDimensions(640, 360),
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
  Media? metadataCurrentMedia;

  @override
  void initState() {
    super.initState();
    if (mounted) {
      player.currentStream.listen((value) {
        setState(() => current = value);
      });
      player.positionStream.listen((value) {
        setState(() => position = value);
      });
      player.playbackStream.listen((value) {
        setState(() => playback = value);
      });
      player.generalStream.listen((value) {
        setState(() => general = value);
      });
      player.videoDimensionsStream.listen((value) {
        setState(() => videoDimensions = value);
      });
      player.bufferingProgressStream.listen(
        (value) {
          setState(() => bufferingProgress = value);
        },
      );
      player.errorStream.listen((event) {
        debugPrint('libVLC error.');
      });
      devices = Devices.all;
      Equalizer equalizer = Equalizer.createMode(EqualizerMode.live);
      equalizer.setPreAmp(10.0);
      equalizer.setBandAmp(31.25, 10.0);
      player.setEqualizer(equalizer);
    }
  }

  @override
  Widget build(BuildContext context) {
    bool isTablet;
    bool isPhone;
    final devicePixelRatio = MediaQuery.of(context).devicePixelRatio;
    final width = MediaQuery.of(context).size.width;
    final height = MediaQuery.of(context).size.height;
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
    return ListView(
      shrinkWrap: true,
      padding: const EdgeInsets.all(4.0),
      children: [
        Row(
          mainAxisSize: MainAxisSize.min,
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Card(
              elevation: 4.0,
              clipBehavior: Clip.antiAlias,
              child: Video(
                player: player,
                width: isPhone ? 320 : 640,
                height: isPhone ? 180 : 360,
                volumeThumbColor: Colors.blue,
                volumeActiveColor: Colors.blue,
                showControls: !isPhone,
              ),
            )
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
                                      controller: controller,
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
                                      value: mediaType,
                                      onChanged: (mediaType) => this.setState(
                                          () => mediaType = mediaType!),
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
                                        if (mediaType == MediaType.file) {
                                          medias.add(
                                            Media.file(
                                              File(
                                                controller.text
                                                    .replaceAll('"', ''),
                                              ),
                                            ),
                                          );
                                        } else if (mediaType ==
                                            MediaType.network) {
                                          medias.add(
                                            Media.network(
                                              controller.text,
                                            ),
                                          );
                                        }
                                        setState(() {});
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
                                    onPressed: () => setState(
                                      () {
                                        player.open(
                                          Playlist(medias: medias),
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
                                      setState(() => medias.clear());
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
                            onChanged: (double position) => player.seek(
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
                                  Text(
                                    '${general.volume}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.general.rate'),
                                  Text(
                                    '${general.rate}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.position.position'),
                                  Text(
                                    '${position.position}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.position.duration'),
                                  Text(
                                    '${position.duration}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.playback.isCompleted'),
                                  Text(
                                    '${playback.isCompleted}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.playback.isPlaying'),
                                  Text(
                                    '${playback.isPlaying}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.playback.isSeekable'),
                                  Text(
                                    '${playback.isSeekable}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.current.index'),
                                  Text(
                                    '${current.index}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.current.media'),
                                  Text(
                                    '${current.media}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.current.medias'),
                                  Text(
                                    '${current.medias}',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.videoDimensions'),
                                  Text(
                                    '$videoDimensions',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
                                ],
                              ),
                              TableRow(
                                children: [
                                  const Text('player.bufferingProgress'),
                                  Text(
                                    '$bufferingProgress',
                                    maxLines: 1,
                                    overflow: TextOverflow.ellipsis,
                                  )
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
                                    onTap: () => player.setDevice(device),
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
                                  controller: metasController,
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
                                  value: mediaType,
                                  onChanged: (mediaType) =>
                                      setState(() => mediaType = mediaType!),
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
                                    if (mediaType == MediaType.file) {
                                      metadataCurrentMedia = Media.file(
                                        File(metasController.text),
                                        parse: true,
                                      );
                                    } else if (mediaType == MediaType.network) {
                                      metadataCurrentMedia = Media.network(
                                        metasController.text,
                                        parse: true,
                                      );
                                    }
                                    setState(() {});
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
                                .convert(metadataCurrentMedia?.metas),
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
                  onPressed: () => player.play(),
                  child: const Text(
                    'play',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => player.pause(),
                  child: const Text(
                    'pause',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => player.playOrPause(),
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
                  onPressed: () => player.stop(),
                  child: const Text(
                    'stop',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => player.next(),
                  child: const Text(
                    'next',
                    style: TextStyle(
                      fontSize: 14.0,
                    ),
                  ),
                ),
                const SizedBox(width: 12.0),
                ElevatedButton(
                  onPressed: () => player.previous(),
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
              value: player.general.volume,
              onChanged: (volume) {
                player.setVolume(volume);
                setState(() {});
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
              value: player.general.rate,
              onChanged: (rate) {
                player.setRate(rate);
                setState(() {});
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
                physics: const NeverScrollableScrollPhysics(),
                shrinkWrap: true,
                onReorder: (int before, int after) async {
                  // [ReorderableListView] in Flutter is buggy.
                  // The [onReorder] callback receives incorrect indices when the [children] are re-ordered.
                  // Workaround : https://stackoverflow.com/a/54164333/12825435
                  // Issue      : https://github.com/flutter/flutter/issues/24786
                  if (after > current.medias.length) {
                    after = current.medias.length;
                  }
                  if (before < after) after--;
                  player.move(
                    before,
                    after,
                  );
                  setState(() {});
                },
                scrollDirection: Axis.vertical,
                padding: const EdgeInsets.symmetric(vertical: 8.0),
                children: List.generate(
                  current.medias.length,
                  (int index) => ListTile(
                    key: Key(index.toString()),
                    leading: Text(
                      index.toString(),
                      style: const TextStyle(fontSize: 14.0),
                    ),
                    title: Container(
                      padding: const EdgeInsets.only(right: 56.0),
                      child: Text(
                        current.medias[index].resource,
                        overflow: TextOverflow.ellipsis,
                        style: const TextStyle(fontSize: 14.0),
                      ),
                    ),
                    subtitle: Text(
                      current.medias[index].mediaType.toString(),
                      overflow: TextOverflow.ellipsis,
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
