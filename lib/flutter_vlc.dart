import 'dart:async';
import 'package:flutter/widgets.dart';
import 'package:flutter/services.dart';

final MethodChannel _channel = new MethodChannel('flutter_vlc');


enum AudioSourceType {
  audio,
  playlist,
}


enum AudioType {
  file,
  network,
  asset,
}


abstract class AudioSource {
  AudioSourceType audioSourceType;

  Map<String, dynamic> toMap();
}


class Audio extends AudioSource {
  AudioSourceType audioSourceType = AudioSourceType.audio;
  AudioType audioType;
  String resource;

  static Audio file(String path) {
    Audio audio = new Audio();
    audio.audioType = AudioType.file;
    audio.resource = path;
    return audio;
  }

  static Audio network(String url) {
    Audio audio = new Audio();
    audio.audioType = AudioType.network;
    audio.resource = url;
    return audio;
  }

  static Audio asset(String path) {
    Audio audio = new Audio();
    audio.audioType = AudioType.file;
    audio.resource = path;
    return audio;
  }

  static Audio fromMap(dynamic map) {
    Audio audio = new Audio();
    audio.audioType = {
      'AudioType.file': AudioType.file,
      'AudioType.network': AudioType.network,
      'AudioType.asset': AudioType.asset,
    }[map['audioType']];
    audio.resource = map['resource'];
    return audio;
  }

  Map<String, dynamic> toMap() {
    return {
      'audioSourceType': this.audioSourceType.toString(),
      'audioType': this.audioType.toString(),
      'resource': this.resource,
    };
  }
}


class Playlist extends AudioSource {
  AudioSourceType audioSourceType = AudioSourceType.playlist;
  int start;
  List<Audio> audios;

  Playlist({@required this.audios, @required this.start});

  static Playlist fromMap(dynamic map) {
    return new Playlist(
      start: map['start'],
      audios: map['audios'].map(
        (audio) => Audio.fromMap(audio),
      ).toList(),
    );
  }

  Map<String, dynamic> toMap() {
    return {
      'audioSourceType': this.audioSourceType.toString(),
      'start': this.start,
      'audios': this.audios.map(
        (Audio audio) => audio.toMap(),
      ).toList(),
    };
  }
}


class AudioPlayerState {
  int index = 0;
  List<Audio> audios;
  bool isPlaying = false;
  bool isValid = true;
  bool isSeekable = true;
  bool isCompleted = false;
  Duration position = Duration.zero;
  Duration duration = Duration.zero;
  double volume = 1.0;
  double rate = 1.0;
  bool isPlaylist = false;
}


class AudioPlayer {
  int id;
  AudioPlayerState state = new AudioPlayerState();

  static Future<AudioPlayer> create({@required int id}) async {
    await _channel.invokeMethod(
      'create', {
        'id': id,
      },
    );
    return new AudioPlayer()..id = id;
  }

  Future<void> open(AudioSource source) async {
    print(source.toMap());
    await _channel.invokeMethod(
    'open', {
        'id': this.id,
        'source': source.toMap(),
      },
    );
  }

  Future<void> play() async {
    await _channel.invokeMethod(
    'play', {
        'id': this.id,
      },
    );
  }

  Future<void> pause() async {
    await _channel.invokeMethod(
    'pause', {
        'id': this.id,
      },
    );
  }

  Future<void> stop() async {
    await _channel.invokeMethod(
    'stop', {
        'id': this.id,
      },
    );
  }

  Future<void> next() async {
    await _channel.invokeMethod(
    'next', {
        'id': this.id,
      },
    );
  }

  Future<void> back() async {
    await _channel.invokeMethod(
    'back', {
        'id': this.id,
      },
    );
  }

  Future<void> jump(int index) async {
    await _channel.invokeMethod(
    'back', {
        'id': this.id,
        'index': index,
      },
    );
  }

  Future<void> seek(Duration duration) async {
    await _channel.invokeMethod(
    'seek', {
        'id': this.id,
        'duration': duration.inMilliseconds,
      },
    );
  }

  Future<void> setVolume(double volume) async {
    await _channel.invokeMethod(
    'setVolume', {
        'id': this.id,
        'volume': volume,
      },
    );
  }

  Future<void> setRate(double rate) async {
    await _channel.invokeMethod(
    'setRate', {
        'id': this.id,
        'rate': rate,
      },
    );
  }
}
