import 'package:flutter/material.dart';
import 'package:dart_vlc/src/mediaSource/media.dart';
import 'package:dart_vlc/src/mediaSource/mediaSource.dart';
import 'package:dart_vlc/src/enums/mediaSourceType.dart';

/// A playlist object to open inside a [Player.open].
///
/// Takes [List] of [Media] as parameter to open inside the [Player] instance, for playing sequencially.
///
/// ```dart
/// new Playlist(
///   medias: [
///     Media.file(
///       new File('C:/music.mp3'),
///     ),
///     Media.network(
///       'https://alexmercerind.github.io/music.mp3',
///     ),
///     Media.file(
///       new File('C:/audio.mp3'),
///     ),
///   ],
/// );
/// ```
///
class Playlist extends MediaSource {
  MediaSourceType _mediaSourceType = MediaSourceType.playlist;

  /// [List] of [Media] present in the playlist.
  List<Media> medias;

  Playlist({@required this.medias});

  /// Internally used method to easily transform data for sending through Platform channel.
  static Playlist fromMap(dynamic map) => new Playlist(
        medias: map['medias']
            .map(
              (media) => Media.fromMap(media),
            )
            .toList()
            .cast<Media>(),
      );

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap() => {
        'mediaSourceType': this._mediaSourceType.toString(),
        'medias': this
            .medias
            .map(
              (Media media) => media.toMap(),
            )
            .toList(),
      };
}
