import 'dart:io';
import 'dart:async';
import 'package:flutter/widgets.dart';
import 'package:flutter/services.dart';


/// Internally used map to keep [AudioPlayer] instances & manage event streams.
Map<int, AudioPlayer> _audioPlayers = {};

/// Platform channel for invoking methods & handling calls.
final MethodChannel _channel = new MethodChannel('flutter_vlc')
..setMethodCallHandler(
  (MethodCall methodCall) async {
    switch (methodCall.method) {
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


/// Enum to specify the type of [AudioSource] passed in [AudioPlayer.open].
enum AudioSourceType {
  /// A single [Audio].
  audio,
  /// A [Playlist] containing multiple [Audio] to play sequencially.
  playlist,
}


/// Enum to specify the type of [Audio].
enum AudioType {
  /// An [Audio] opened from a [File].
  file,
  /// An [Audio] opened from a [Uri].
  network,
  /// An [Audio] opened from assets.
  asset,
}

/// Parent abstract class of [Audio] and [Playlist].
abstract class AudioSource {
  AudioSourceType audioSourceType;

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap();
}

/// An audio object to open inside an [AudioPlayer].
/// 
/// * An [Audio] from a [File].
/// 
/// ```dart
/// Audio audio = Audio.file(new File('C:/music.ogg'));
/// ```
/// 
/// * An [Audio] from a [Uri].
/// 
/// ```dart
/// Audio audio = Audio.network(Uri.https('alexmercerind.github.io', '/music.mp3'));
/// ```
/// 
/// * An [Audio] from assets.
/// 
/// ```dart
/// Audio audio = Audio.asset('asset/audio/music.aac');
/// ```
/// 
class Audio extends AudioSource {
  AudioSourceType audioSourceType = AudioSourceType.audio;
  AudioType audioType;
  String resource;

  /// Makes [Audio] object from a [File].
  static Audio file(File file) {
    Audio audio = new Audio();
    audio.audioType = AudioType.file;
    audio.resource = file.path;
    return audio;
  }

  /// Makes [Audio] object from a [Uri].
  static Audio network(Uri uri) {
    Audio audio = new Audio();
    audio.audioType = AudioType.network;
    audio.resource = uri.toString();
    return audio;
  }

  /// Makes [Audio] object from an asset.
  static Audio asset(String path) {
    Audio audio = new Audio();
    audio.audioType = AudioType.file;
    audio.resource = path;
    return audio;
  }

  /// Internally used method to easily transform data for sending through Platform channel.
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

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap() {
    return {
      'audioSourceType': this.audioSourceType.toString(),
      'audioType': this.audioType.toString(),
      'resource': this.resource,
    };
  }
}


/// A playlist object to open inside an [AudioPlayer.open].
/// 
/// Takes [List] of [Audio] as parameter to open inside the [AudioPlayer] instance, for playing sequencially.
/// 
/// ```dart
/// new Playlist(
///   audios: [
///     Audio.file(
///       new File('C:/music.mp3'),
///     ),
///     Audio.network(
///       Uri.https('alexmercerind.github.io', '/music.mp3'),
///     ),
///     Audio.file(
///       new File('C:/audio.mp3'),
///     ),
///   ],
/// );
/// ```
/// 
class Playlist extends AudioSource {
  AudioSourceType audioSourceType = AudioSourceType.playlist;
  /// [List] of [Audio] present in the playlist.
  List<Audio> audios;

  Playlist({@required this.audios});

  /// Internally used method to easily transform data for sending through Platform channel.
  static Playlist fromMap(dynamic map) {
    return new Playlist(
      audios: map['audios'].map(
        (audio) => Audio.fromMap(audio),
      ).toList(),
    );
  }

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap() {
    return {
      'audioSourceType': this.audioSourceType.toString(),
      'audios': this.audios.map(
        (Audio audio) => audio.toMap(),
      ).toList(),
    };
  }
}


/// State of an [AudioPlayer] instance.
class AudioPlayerState {
  /// Index of the currently playing [Audio].
  int index = 0;
  /// [List] of [Audio] currently opened in the [AudioPlayer] instance.
  List<dynamic> audios;
  /// Whether [AudioPlayer] instance is playing or not.
  bool isPlaying = false;
  /// Whether [AudioPlayer] instance is valid or not.
  bool isValid = true;
  /// Whether [AudioPlayer] instance is seekable or not.
  bool isSeekable = true;
  /// Whether the current [Audio] has ended playing or not.
  bool isCompleted = false;
  /// Position of playback in [Duration] of currently playing [Audio].
  Duration position = Duration.zero;
  /// Length of currently playing [Audio] in [Duration].
  Duration duration = Duration.zero;
  /// Volume of [AudioPlayer] instance.
  double volume = 1.0;
  /// Rate of playback of [AudioPlayer] instance.
  double rate = 1.0;
  /// Whether a [Playlist] is opened or an [Audio].
  bool isPlaylist = false;

  /// Internally used method to easily transform data for sending through Platform channel.
  static AudioPlayerState fromMap(dynamic map) {
    AudioPlayerState state = AudioPlayerState();
    state.index = map['index'];
    state.audios = map['audios'].map(
      (dynamic audio) => Audio.fromMap(audio),
    ).toList();
    state.isPlaying = map['isPlaying'];
    state.isValid = map['isValid'];
    state.isSeekable = map['isSeekable'];
    state.isCompleted = map['isCompleted'];
    state.position = Duration(milliseconds: map['position'] ?? 0);
    state.duration = Duration(milliseconds: map['duration'] ?? 0);
    state.volume = map['volume'];
    state.rate = map['rate'];
    state.isPlaying = map['isPlaylist'];
    return state;
  }
}


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
  static Future<AudioPlayer> create({@required int id}) async {
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
  ///     Audio.network(
  ///       Uri.https('alexmercerind.github.io', '/music.mp3'),
  ///     ),
  ///     Audio.file(
  ///       new File('C:/audio.mp3'),
  ///     ),
  ///   ],
  /// );
  /// ```
  /// 
  Future<void> open(AudioSource source, {bool autoStart: true}) async {
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


/// Internally used class to avoid direct creation of the object of a+ [AudioPlayer] class.
class _AudioPlayer extends AudioPlayer {}
