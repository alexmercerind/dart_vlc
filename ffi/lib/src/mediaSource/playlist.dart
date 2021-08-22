import 'package:dart_vlc_ffi/src/enums/playlistMode.dart';
import 'package:dart_vlc_ffi/src/internal/utils.dart';
import 'package:dart_vlc_ffi/src/mediaSource/media.dart';
import 'package:dart_vlc_ffi/src/mediaSource/mediaSource.dart';
import 'package:dart_vlc_ffi/src/enums/mediaSourceType.dart';

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
  MediaSourceType get mediaSourceType => MediaSourceType.playlist;

  /// [List] of [Media] present in the playlist.
  final List<Media> medias;
  final PlaylistMode playlistMode;

  const Playlist(
      {required this.medias, this.playlistMode = PlaylistMode.single});

  int get hashCode => medias.hashCode ^ playlistMode.hashCode;

  bool operator ==(Object other) =>
      other is Playlist &&
      other.playlistMode == playlistMode &&
      listEquals(other.medias, medias);

  @override
  String toString() => 'Playlist[${medias.length}]';
}
