import 'dart:io';

import 'package:flutter/material.dart';
import 'package:dart_vlc/dart_vlc.dart';

void main() {
  runApp(DartVLC());
}

class DartVLC extends StatefulWidget {
  @override
  _DartVLCState createState() => _DartVLCState();
}

class _DartVLCState extends State<DartVLC> {
  Player player;
  MediaType mediaType = MediaType.file;
  PlayerState state = new PlayerState();
  List<Media> medias = <Media>[];
  TextEditingController controller = new TextEditingController();

  @override
  void didChangeDependencies() async {
    super.didChangeDependencies();
    this.player = await Player.create(id: 0);
    this.player.stream.listen((PlayerState state) {
      this.setState(() => this.state = state);
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        backgroundColor: Colors.grey[100],
        appBar: AppBar(
          title: const Text('dart_vlc'),
          centerTitle: true,
        ),
        body: ListView(
          padding: EdgeInsets.all(4.0),
          children: [
            Card(
              elevation: 2.0,
              color: Colors.white,
              margin: EdgeInsets.all(4.0),
              child: Container(
                margin: EdgeInsets.all(16.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: <Widget>[
                        Text('Add to playlist.'),
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
                                  hintText: 'Media resource location.',
                                ),
                              ),
                            ),
                            Container(
                              width: 148.0,
                              child: DropdownButton<MediaType>(
                                value: this.mediaType,
                                onChanged: (mediaType) => this
                                    .setState(() => this.mediaType = mediaType),
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
                                    this.medias.add(
                                          Media.file(new File(controller.text)),
                                        );
                                  } else if (this.mediaType ==
                                      MediaType.network) {
                                    this.medias.add(
                                          Media.network(controller.text),
                                        );
                                  } else if (this.mediaType ==
                                      MediaType.asset) {
                                    this.medias.add(
                                          await Media.asset(controller.text),
                                        );
                                  }
                                  this.setState(() {});
                                },
                                child: Text(
                                  'Add',
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
                                      ),
                                    );
                              }),
                              child: Text(
                                'Open',
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
                                'Clear',
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
              color: Colors.white,
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
                        ElevatedButton(
                          onPressed: () => this.player.playOrPause(),
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
                      value: this.player?.state?.volume ?? 1.0,
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
                      value: this.player?.state?.rate ?? 1.0,
                      onChanged: (rate) {
                        this.player.setRate(rate);
                        this.setState(() {});
                      },
                    ),
                  ],
                ),
              ),
            ),
            Card(
              elevation: 2.0,
              color: Colors.white,
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
                      max: this.state.duration.inMilliseconds.toDouble(),
                      value: this.state.position.inMilliseconds.toDouble(),
                      onChanged: (double position) {
                        this.player.seek(
                              Duration(milliseconds: position.toInt()),
                            );
                      },
                    ),
                    Text('Stats for nerds.'),
                    Divider(
                      height: 8.0,
                      color: Colors.transparent,
                    ),
                    Table(
                      children: [
                        TableRow(children: [
                          Text('player.state.volume'),
                          Text('${this.state.volume}')
                        ]),
                        TableRow(children: [
                          Text('player.state.rate'),
                          Text('${this.state.rate}')
                        ]),
                        TableRow(children: [
                          Text('player.state.position'),
                          Text('${this.state.position}')
                        ]),
                        TableRow(children: [
                          Text('player.state.duration'),
                          Text('${this.state.duration}')
                        ]),
                        TableRow(children: [
                          Text('player.state.index'),
                          Text('${this.state.index}')
                        ]),
                        TableRow(children: [
                          Text('player.state.isCompleted'),
                          Text('${this.state.isCompleted}')
                        ]),
                        TableRow(children: [
                          Text('player.state.isPlaying'),
                          Text('${this.state.isPlaying}')
                        ]),
                        TableRow(children: [
                          Text('player.state.isSeekable'),
                          Text('${this.state.isSeekable}')
                        ]),
                        TableRow(children: [
                          Text('player.state.isValid'),
                          Text('${this.state.isValid}')
                        ]),
                        TableRow(children: [
                          Text('player.state.medias'),
                          Text('${this.state.medias}')
                        ]),
                      ],
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
