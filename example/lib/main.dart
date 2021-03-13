import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_vlc/flutter_vlc.dart';


void main() {
  runApp(FlutterVLCApp());
}


class FlutterVLCApp extends StatefulWidget {
  @override
  _FlutterVLCAppState createState() => _FlutterVLCAppState();
}


class _FlutterVLCAppState extends State<FlutterVLCApp> {
  AudioPlayer audioPlayer;
  AudioType audioType = AudioType.file;
  AudioPlayerState state = new AudioPlayerState();
  List<Audio> audios = <Audio>[];
  TextEditingController controller = new TextEditingController();

  @override
  void didChangeDependencies() async {
    super.didChangeDependencies();
    this.audioPlayer = await AudioPlayer.create(id: 0);
    this.audioPlayer.stream.listen((AudioPlayerState state) {
      this.setState(() => this.state = state);
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        backgroundColor: Colors.grey[100],
        appBar: AppBar(
          title: const Text('Flutter VLC'),
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
                              hintText: 'Audio resource location.',
                            ),
                          ),
                        ),
                        Container(
                          width: 148.0,
                          child: DropdownButton<AudioType>(
                            value: this.audioType,
                            onChanged: (audioType) => this.setState(() => this.audioType = audioType),
                            items: [
                              DropdownMenuItem<AudioType>(
                                value: AudioType.file,
                                child: Text(
                                  AudioType.file.toString(),
                                  style: TextStyle(
                                    fontSize: 14.0,
                                  ),
                                ),
                              ),
                              DropdownMenuItem<AudioType>(
                                value: AudioType.network,
                                child: Text(
                                  AudioType.network.toString(),
                                  style: TextStyle(
                                    fontSize: 14.0,
                                  ),
                                ),
                              ),
                              DropdownMenuItem<AudioType>(
                                value: AudioType.asset,
                                child: Text(
                                  AudioType.asset.toString(),
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
                            onPressed: () => this.setState(() {
                              if (this.audioType == AudioType.file) {
                                this.audios.add(
                                  Audio.file(
                                    new File(controller.text)
                                  ),
                                );
                              }
                              else if (this.audioType == AudioType.network) {
                                this.audios.add(
                                  Audio.network(
                                    controller.text
                                  ),
                                );
                              }
                              else if (this.audioType == AudioType.asset) {
                                this.audios.add(
                                  Audio.asset(
                                    controller.text
                                  ),
                                );
                              }
                            }),
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
                  ] + this.audios.map(
                    (audio) => ListTile(
                      title: Text(
                        audio.resource,
                        style: TextStyle(
                          fontSize: 14.0,
                        ),
                      ),
                      subtitle: Text(
                        audio.audioType.toString(),
                        style: TextStyle(
                          fontSize: 14.0,
                        ),
                      ),
                    ),
                  ).toList() + <Widget>[
                    Divider(
                      height: 8.0,
                      color: Colors.transparent,
                    ),
                    Row(
                      children: [
                        ElevatedButton(
                          onPressed: () => this.setState(() {
                            this.audioPlayer.open(
                              new Playlist(
                                audios: this.audios,
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
                            this.audios.clear();
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
                          onPressed: () => this.audioPlayer.play(),
                          child: Text(
                            'play',
                            style: TextStyle(
                              fontSize: 14.0,
                            ),
                          ),
                        ),
                        SizedBox(width: 12.0),
                        ElevatedButton(
                          onPressed: () => this.audioPlayer.pause(),
                          child: Text(
                            'pause',
                            style: TextStyle(
                              fontSize: 14.0,
                            ),
                          ),
                        ),
                        SizedBox(width: 12.0),
                        ElevatedButton(
                          onPressed: () => this.audioPlayer.playOrPause(),
                          child: Text(
                            'playOrPause',
                            style: TextStyle(
                              fontSize: 14.0,
                            ),
                          ),
                        ),
                        SizedBox(width: 12.0),
                        ElevatedButton(
                          onPressed: () => this.audioPlayer.playOrPause(),
                          child: Text(
                            'stop',
                            style: TextStyle(
                              fontSize: 14.0,
                            ),
                          ),
                        ),
                        SizedBox(width: 12.0),
                        ElevatedButton(
                          onPressed: () => this.audioPlayer.next(),
                          child: Text(
                            'next',
                            style: TextStyle(
                              fontSize: 14.0,
                            ),
                          ),
                        ),
                        SizedBox(width: 12.0),
                        ElevatedButton(
                          onPressed: () => this.audioPlayer.back(),
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
                      value: this.audioPlayer?.state?.volume ?? 1.0,
                      onChanged: (volume) {
                        this.audioPlayer.setVolume(volume);
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
                      value: this.audioPlayer?.state?.rate ?? 1.0,
                      onChanged: (rate) {
                        this.audioPlayer.setRate(rate);
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
                        this.audioPlayer.seek(
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
                        TableRow(
                          children: [
                            Text('audioPlayer.state.volume'),
                            Text('${this.state.volume}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.rate'),
                            Text('${this.state.rate}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.position'),
                            Text('${this.state.position}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.duration'),
                            Text('${this.state.duration}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.index'),
                            Text('${this.state.index}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.isCompleted'),
                            Text('${this.state.isCompleted}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.isPlaying'),
                            Text('${this.state.isPlaying}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.isSeekable'),
                            Text('${this.state.isSeekable}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.isValid'),
                            Text('${this.state.isValid}')
                          ]
                        ),
                        TableRow(
                          children: [
                            Text('audioPlayer.state.audios'),
                            Text('${this.state.audios}')
                          ]
                        ),
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
