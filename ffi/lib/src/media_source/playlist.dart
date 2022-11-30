import 'package:dart_vlc_ffi/src/internal/utils.dart';
import 'package:dart_vlc_ffi/src/media_source/media.dart';
import 'package:dart_vlc_ffi/src/media_source/media_source.dart';
import 'package:dart_vlc_ffi/src/enums/media_source_type.dart';

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
class Playlist implements MediaSource {
  @override
  MediaSourceType get mediaSourceType => MediaSourceType.playlist;

  /// [List] of [Media] present in the playlist.
  final List<Media> medias;

  const Playlist({
    required this.medias,
  });

  @override
  int get hashCode =>
      medias.hashCode ^
      medias.map((e) => e.hashCode).reduce((value, element) => value ^ element);

  @override
  bool operator ==(Object other) =>
      other is Playlist && listEquals(other.medias, medias);

  @override
  String toString() => 'Playlist[${medias.length}]';
}
