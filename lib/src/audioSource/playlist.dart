import 'package:flutter/material.dart';
import 'package:flutter_vlc/src/audioSource/audio.dart';
import 'package:flutter_vlc/src/audioSource/audioSource.dart';
import 'package:flutter_vlc/src/enums/audioSourceType.dart';


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
///       'https://alexmercerind.github.io/music.mp3',
///     ),
///     Audio.file(
///       new File('C:/audio.mp3'),
///     ),
///   ],
/// );
/// ```
/// 
class Playlist extends AudioSource {
  // AudioSourceType audioSourceType = AudioSourceType.playlist;
  AudioSourceType _audioSourceType = AudioSourceType.playlist;
  /// [List] of [Audio] present in the playlist.
  List<Audio> audios;

  Playlist({ @required this.audios });

  /// Internally used method to easily transform data for sending through Platform channel.
  static Playlist fromMap(dynamic map) => new Playlist(
    audios: map['audios'].map(
      (audio) => Audio.fromMap(audio),
    ).toList(),
  );

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap() => {
    'audioSourceType': this._audioSourceType.toString(),
    'audios': this.audios.map(
      (Audio audio) => audio.toMap(),
    ).toList(),
  };
}
