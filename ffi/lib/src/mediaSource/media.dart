import 'dart:io';
import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:path/path.dart' as path;
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/mediaSource/mediaSource.dart';
import 'package:dart_vlc_ffi/src/enums/mediaSourceType.dart';
import 'package:dart_vlc_ffi/src/enums/mediaType.dart';

/// Internally used class to avoid direct creation of the object of a [Media] class.
class _Media extends Media {}

/// A media object to open inside a [Player].
///
/// Pass `true` to [parse] for retrieving the metadata of the [Media].
/// [timeout] sets the time-limit for retriveing metadata.
/// [Media.metas] can be then, accessed to get the retrived metadata as `Map<String, String>`.
///
/// * A [Media] from a [File].
///
/// ```dart
/// Media media = await Media.file(new File('C:/music.ogg'));
/// ```
///
/// * A [Media] from a [Uri].
///
/// ```dart
/// Media media = await Media.network('http://alexmercerind.github.io/music.mp3');
/// ```
///
///
class Media extends MediaSource {
  MediaSourceType mediaSourceType = MediaSourceType.media;
  Map<String, String> metas = {};
  late MediaType mediaType;
  late String resource;

  /// Makes [Media] object from a [File].
  static Media file(File file,
      {bool parse: false,
      Map<String, dynamic>? extras,
      Duration timeout: const Duration(seconds: 10)}) {
    Media media = new _Media();
    media.mediaType = MediaType.file;
    media.resource = file.path;
    if (parse) {
      media.parse(timeout);
    }
    return media;
  }

  /// Makes [Media] object from url.
  static Media network(dynamic url,
      {bool parse: false,
      Map<String, dynamic>? extras,
      Duration timeout: const Duration(seconds: 10)}) {
    Media media = new _Media();
    media.mediaType = MediaType.network;
    if (url is Uri)
      media.resource = url.toString();
    else
      media.resource = url;
    if (parse) {
      media.parse(timeout);
    }
    return media;
  }

  /// Makes [Media] object from direct show.
  static Media directShow(
      {String vdev = '', String adev = '', required int liveCaching}) {
    Media media = new _Media();
    media.mediaType = MediaType.directShow;
    media.resource =
        'dshow:// :dshow-vdev=$vdev :dshow-adev=$adev :live-caching=$liveCaching';
    return media;
  }

  /// Makes [Media] object from assets.
  ///
  /// **WARNING**
  ///
  /// This method only works for Flutter.
  /// Might result in an exception on Dart CLI.
  ///
  static Media asset(String asset) {
    Media media = new _Media();
    media.mediaType = MediaType.asset;
    late String directory;
    if (Platform.isWindows) {
      directory = Platform.resolvedExecutable
          .split('\\')
          .sublist(0, Platform.resolvedExecutable.split('\\').length - 1)
          .join('\\');
      media.resource = path.join('file:///' + directory, 'data', 'flutter_assets', asset);
    }
    if (Platform.isLinux) {
      directory = Platform.resolvedExecutable
          .split('/')
          .sublist(0, Platform.resolvedExecutable.split('/').length - 1)
          .join('/');
      media.resource = path.join('file://' + directory, 'data', 'flutter_assets', asset);
    }
    return media;
  }

  /// Parses the [Media] to retrieve [Media.metas].
  void parse(Duration timeout) {
    Pointer<Pointer<Utf8>> metas = MediaFFI.parse(
        this.mediaType.toString().toNativeUtf8(),
        this.resource.toNativeUtf8(),
        timeout.inSeconds);
    // Keep this sorted alphabetically by key.
    this.metas['actors'] = metas.elementAt(0).value.toDartString();
    this.metas['album'] = metas.elementAt(1).value.toDartString();
    this.metas['albumArtist'] = metas.elementAt(2).value.toDartString();
    this.metas['artist'] = metas.elementAt(3).value.toDartString();
    this.metas['artworkUrl'] = metas.elementAt(4).value.toDartString();
    this.metas['copyright'] = metas.elementAt(5).value.toDartString();
    this.metas['date'] = metas.elementAt(6).value.toDartString();
    this.metas['description'] = metas.elementAt(7).value.toDartString();
    this.metas['director'] = metas.elementAt(8).value.toDartString();
    this.metas['discNumber'] = metas.elementAt(9).value.toDartString();
    this.metas['discTotal'] = metas.elementAt(10).value.toDartString();
    this.metas['duration'] = metas.elementAt(11).value.toDartString();
    this.metas['encodedBy'] = metas.elementAt(12).value.toDartString();
    this.metas['episode'] = metas.elementAt(13).value.toDartString();
    this.metas['genre'] = metas.elementAt(14).value.toDartString();
    this.metas['language'] = metas.elementAt(15).value.toDartString();
    this.metas['nowPlaying'] = metas.elementAt(16).value.toDartString();
    this.metas['rating'] = metas.elementAt(17).value.toDartString();
    this.metas['season'] = metas.elementAt(18).value.toDartString();
    this.metas['settings'] = metas.elementAt(19).value.toDartString();
    this.metas['title'] = metas.elementAt(20).value.toDartString();
    this.metas['trackNumber'] = metas.elementAt(21).value.toDartString();
    this.metas['trackTotal'] = metas.elementAt(22).value.toDartString();
    this.metas['url'] = metas.elementAt(23).value.toDartString();
    calloc.free(metas);
  }
}
