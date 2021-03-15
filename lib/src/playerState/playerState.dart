import 'package:dart_vlc/src/mediaSource/media.dart';

/// State of a [Player] instance.
class PlayerState {
  /// Index of the currently playing [Media].
  int index = 0;
  /// [List] of [Media] currently opened in the [Player] instance.
  List<dynamic> medias = <dynamic>[];
  /// Whether [Player] instance is playing or not.
  bool isPlaying = false;
  /// Whether [Player] instance is valid or not.
  bool isValid = true;
  /// Whether [Player] instance is seekable or not.
  bool isSeekable = true;
  /// Whether the current [Media] has ended playing or not.
  bool isCompleted = false;
  /// Position of playback in [Duration] of currently playing [Media].
  Duration position = Duration.zero;
  /// Length of currently playing [Media] in [Duration].
  Duration duration = Duration.zero;
  /// Volume of [Player] instance.
  double volume = 1.0;
  /// Rate of playback of [Player] instance.
  double rate = 1.0;
  /// Whether a [Playlist] is opened or a [Media].
  bool isPlaylist = false;

  /// Internally used method to easily transform data for sending through Platform channel.
  static PlayerState fromMap( dynamic map ) {
    PlayerState state = PlayerState();
    state.index = map['index'];
    state.medias = map['medias'].map(( dynamic media ) => Media.fromMap(media)).toList();
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
