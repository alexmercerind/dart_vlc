import 'package:flutter_vlc/src/enums/audioSourceType.dart';


/// Parent abstract class of [Audio] and [Playlist].
abstract class AudioSource {
  AudioSourceType audioSourceType;

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap();
}
