import 'package:flutter_vlc/src/audioSource/audio.dart';

/// State of an [AudioPlayer] instance.
class AudioPlayerState {
  /// Index of the currently playing [Audio].
  int index = 0;
  /// [List] of [Audio] currently opened in the [AudioPlayer] instance.
  List<dynamic> audios = <dynamic>[];
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
  static AudioPlayerState fromMap( dynamic map ) {
    AudioPlayerState state = AudioPlayerState();
    state.index = map['index'];
    state.audios = map['audios'].map(( dynamic audio ) => Audio.fromMap(audio)).toList();
    state.isPlaying = map['isPlaying'];
    state.isValid = map['isValid'];
    state.isSeekable = map['isSeekable'];
    state.isCompleted= map['isCompleted'];
    state.position = Duration( milliseconds: map['position'] ?? 0 );
    state.duration = Duration( milliseconds: map['duration'] ?? 0 );
    state.volume = map['volume'];
    state.rate = map['rate'];
    state.isPlaying = map['isPlaylist'];
    return state;
  }
}