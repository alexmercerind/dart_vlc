import 'dart:async';
import 'package:flutter/widgets.dart';
import 'package:dart_vlc/src/channel.dart';
import 'package:dart_vlc/src/playerState/playerState.dart';
import 'package:dart_vlc/src/mediaSource/mediaSource.dart';


/// Internally used class to avoid direct creation of the object of a+ [Player] class.
class _Player extends Player {}


/// A [Player] to open & play a [Media] or [Playlist] from file, network or asset.
/// 
/// Use [Player.create] method to create a new instance of a [Player].
/// Provide a unique [id] while instanciating.
/// 
/// ```dart
/// Player player = await Player.create(id: 0);
/// ```
/// 
/// Use various methods avaiable to control the playback.
/// [Player.state] stores the current state of [Player] in form of [PlayerState].
/// [Player.stream] can be used to listen to playback events of [Player] instance.
/// 
abstract class Player {
  /// ID associated with the [Player] instance.
  int id;
  /// Playback state of the [Player] instance. 
  PlayerState state = new PlayerState();
  /// Internally used [StreamController],
  StreamController<PlayerState> streamController;
  /// Stream to listen to playback state of the [Player] instance. 
  Stream<PlayerState> stream;

  /// Creates a new [Player] instance.
  /// 
  /// Takes unique id as parameter.
  /// 
  /// ```dart
  /// Player player = await Player.create(id: 0);
  /// ```
  ///
  static Future<Player> create({ @required int id }) async {
    await channel.invokeMethod(
      'create', {
        'id': id,
      },
    );
    players[id] = new _Player()..id = id;
    players[id].streamController = StreamController<PlayerState>.broadcast();
    players[id].stream = players[id].streamController.stream;
    return players[id];
  }

  /// Opens a new media source into the player.
  /// 
  /// Takes a [Media] or [Playlist] to open in the player.
  /// 
  /// Starts playback itself by default. Pass `autoStart: false` to stop this from happening.
  /// 
  /// * Open a new [Media].
  /// 
  /// ```dart
  /// player.open(
  ///   Media.file(
  ///     new File('C:/music.ogg'),
  ///   ),
  ///   autoStart: false,
  /// );
  /// ```
  /// 
  /// * Open a new [Playlist].
  /// 
  /// ```dart
  /// player.open(
  ///   new Playlist(
  ///     medias: [
  ///       Media.file(
  ///         new File('C:/music.mp3'),
  ///       ),
  ///       Media.file(
  ///         new File('C:/audio.mp3'),
  ///       ),
  ///       Media.network('https://alexmercerind.github.io/music.mp3'),
  ///     ],
  ///   ),
  /// );
  /// ```
  /// 
  Future<void> open( MediaSource source, { bool autoStart: true } ) async {
    if (this.streamController.isClosed) {
      this.streamController = StreamController<PlayerState>.broadcast();
    }
    await channel.invokeMethod(
      'open', {
        'id': this.id,
        'autoStart': autoStart,
        'source': source.toMap(),
      },
    );
  }

  /// Plays opened [MediaSource],
  Future<void> play() async {
    await channel.invokeMethod(
    'play', {
        'id': this.id,
      },
    );
  }

  /// Pauses opened [MediaSource],
  Future<void> pause() async {
    await channel.invokeMethod(
    'pause', {
        'id': this.id,
      },
    );
  }

  /// Play or Pause opened [MediaSource],
  Future<void> playOrPause() async {
    await channel.invokeMethod(
    'playOrPause', {
        'id': this.id,
      },
    );
  }

  /// Stops the [Player],
  Future<void> stop() async {
    await channel.invokeMethod(
    'stop', {
        'id': this.id,
      },
    );
    await this.streamController.close();
  }

  /// Jumps to the next [Media] in the [Playlist] opened.
  Future<void> next() async {
    await channel.invokeMethod(
    'next', {
        'id': this.id,
      },
    );
  }

  /// Jumps to the previous [Media] in the [Playlist] opened.
  Future<void> back() async {
    await channel.invokeMethod(
    'back', {
        'id': this.id,
      },
    );
  }

  /// Jumps to [Media] at specific index in the [Playlist] opened.
  /// Pass index as parameter.
  Future<void> jump(int index) async {
    await channel.invokeMethod(
    'back', {
        'id': this.id,
        'index': index,
      },
    );
  }

  /// Seeks the [Media] currently playing in the [Player] instance, to the provided [Duration].
  Future<void> seek(Duration duration) async {
    await channel.invokeMethod(
    'seek', {
        'id': this.id,
        'duration': duration.inMilliseconds,
      },
    );
  }
  
  /// Sets volume of the [Player] instance.
  Future<void> setVolume(double volume) async {
    await channel.invokeMethod(
    'setVolume', {
        'id': this.id,
        'volume': volume,
      },
    );
  }

  /// Sets playback rate of the [Media] currently playing in the [Player] instance.
  Future<void> setRate(double rate) async {
    await channel.invokeMethod(
    'setRate', {
        'id': this.id,
        'rate': rate,
      },
    );
  }
}
