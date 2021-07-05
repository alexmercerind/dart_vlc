import 'package:dart_vlc_ffi/src/enums/playlistMode.dart';
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
class Playlist extends MediaSource {
  /// [List] of [Media] present in the playlist.
  List<Media> medias;
  MediaSourceType mediaSourceType = MediaSourceType.playlist;
  PlaylistMode playlistMode;
  Playlist({required this.medias, this.playlistMode = PlaylistMode.single});
}
