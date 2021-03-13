import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_vlc/src/audioPlayerState/audioPlayerState.dart';
import 'package:flutter_vlc/src/audioSource/audioSource.dart';


/// Internally used map to keep [AudioPlayer] instances & manage event streams.
Map<int, AudioPlayer> _audioPlayers = {};
/// Internally used class to avoid direct creation of the object of a+ [AudioPlayer] class.
class _AudioPlayer extends AudioPlayer {}

/// Platform channel for invoking methods & handling calls.
final MethodChannel _channel = new MethodChannel('flutter_vlc')
..setMethodCallHandler(
  ( MethodCall methodCall ) async {
    switch ( methodCall.method ) {
      /// [AudioPlayer] event.
      case 'audioPlayerState': {
        int id = methodCall.arguments['id'];
        AudioPlayerState state = AudioPlayerState.fromMap(methodCall.arguments);
        /// Notify the stream of the [AudioPlayer] with respective [id].
        _audioPlayers[id].streamController.add(state);
        /// Update static state of the [AudioPlayer].
        _audioPlayers[id].state = state;
      }
    }
  }
);


/// A [AudioPlayer] to open & play an [Audio] or [Playlist] from file, network or asset.
/// 
/// Use [AudioPlayer.create] method to create a new instance of an [AudioPlayer].
/// Provide a unique [id] while instanciating.
/// 
/// ```dart
/// AudioPlayer audioPlayer = await AudioPlayer.create(id: 0);
/// ```
/// 
/// Use various methods avaiable to control the playback.
/// [AudioPlayer.state] stores the current state of [AudioPlayer] in form of [AudioPlayerState].
/// [AudioPlayer.stream] can be used to listen to playback events of [AudioPlayer] instance.
/// 
abstract class AudioPlayer {
  /// ID associated with the [AudioPlayer] instance.
  int id;
  /// Playback state of the [AudioPlayer] instance. 
  AudioPlayerState state = new AudioPlayerState();
  /// Internally used [StreamController],
  StreamController<AudioPlayerState> streamController;
  /// Stream to listen to playback state of the [AudioPlayer] instance. 
  Stream<AudioPlayerState> stream;

  /// Creates a new [AudioPlayer] instance.
  /// 
  /// Takes unique id as parameter.
  /// 
  /// ```dart
  /// AudioPlayer audioPlayer = await AudioPlayer.create(id: 0);
  /// ```
  ///
  static Future<AudioPlayer> create({ @required int id }) async {
    await _channel.invokeMethod(
      'create', {
        'id': id,
      },
    );
    _audioPlayers[id] = new _AudioPlayer()..id = id;
    _audioPlayers[id].streamController = StreamController<AudioPlayerState>.broadcast();
    _audioPlayers[id].stream = _audioPlayers[id].streamController.stream;
    return _audioPlayers[id];
  }

  /// Opens a new audio source into the audio player.
  /// 
  /// Takes an [Audio] or [Playlist] to open in the audio player.
  /// 
  /// Starts playback itself by default. Pass `autoStart: false` to stop this from happening.
  /// 
  /// * Open a new [Audio].
  /// 
  /// ```dart
  /// audioPlayer.open(
  ///   Audio.file(
  ///     new File('C:/music.ogg'),
  ///   ),
  ///   autoStart: false,
  /// );
  /// ```
  /// 
  /// * Open a new [Playlist].
  /// 
  /// ```dart
  /// audioPlayer.open(
  ///   audios: [
  ///     Audio.file(
  ///       new File('C:/music.mp3'),
  ///     ),
  ///     Audio.file(
  ///       new File('C:/audio.mp3'),
  ///     ),
  ///     Audio.network('https://alexmercerind.github.io/music.mp3'),
  ///   ],
  /// );
  /// ```
  /// 
  Future<void> open( AudioSource source, { bool autoStart: true } ) async {
    if (this.streamController.isClosed) {
      this.streamController = StreamController<AudioPlayerState>.broadcast();
    }
    await _channel.invokeMethod(
      'open', {
        'id': this.id,
        'autoStart': autoStart,
        'source': source.toMap(),
      },
    );
  }

  /// Plays opened [AudioSource],
  Future<void> play() async {
    await _channel.invokeMethod(
    'play', {
        'id': this.id,
      },
    );
  }

  /// Pauses opened [AudioSource],
  Future<void> pause() async {
    await _channel.invokeMethod(
    'pause', {
        'id': this.id,
      },
    );
  }

  /// Play or Pause opened [AudioSource],
  Future<void> playOrPause() async {
    await _channel.invokeMethod(
    'playOrPause', {
        'id': this.id,
      },
    );
  }

  /// Stops the [AudioPlayer],
  Future<void> stop() async {
    await _channel.invokeMethod(
    'stop', {
        'id': this.id,
      },
    );
    await this.streamController.close();
  }

  /// Jumps to the next [Audio] in the [Playlist] opened.
  Future<void> next() async {
    await _channel.invokeMethod(
    'next', {
        'id': this.id,
      },
    );
  }

  /// Jumps to the previous [Audio] in the [Playlist] opened.
  Future<void> back() async {
    await _channel.invokeMethod(
    'back', {
        'id': this.id,
      },
    );
  }

  /// Jumps to [Audio] at specific index in the [Playlist] opened.
  /// Pass index as parameter.
  Future<void> jump(int index) async {
    await _channel.invokeMethod(
    'back', {
        'id': this.id,
        'index': index,
      },
    );
  }

  /// Seeks the [Audio] currently playing in the [AudioPlayer] instance, to the provided [Duration].
  Future<void> seek(Duration duration) async {
    await _channel.invokeMethod(
    'seek', {
        'id': this.id,
        'duration': duration.inMilliseconds,
      },
    );
  }
  
  /// Sets volume of the [AudioPlayer] instance.
  Future<void> setVolume(double volume) async {
    await _channel.invokeMethod(
    'setVolume', {
        'id': this.id,
        'volume': volume,
      },
    );
  }

  /// Sets playback rate of the [Audio] currently playing in the [AudioPlayer] instance.
  Future<void> setRate(double rate) async {
    await _channel.invokeMethod(
    'setRate', {
        'id': this.id,
        'rate': rate,
      },
    );
  }
}
