import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/services.dart';
import 'package:path/path.dart' as _path;
import 'package:path_provider/path_provider.dart' as _path;
import 'package:dart_vlc/src/channel.dart';
import 'package:dart_vlc/src/mediaSource/mediaSource.dart';
import 'package:dart_vlc/src/enums/mediaSourceType.dart';
import 'package:dart_vlc/src/enums/mediaType.dart';

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
/// * A [Media] from assets.
///
/// ```dart
/// Media media = await Media.asset('asset/media/music.aac');
/// ```
///
abstract class Media extends MediaSource {
  int id = mediaExtras.length;
  MediaSourceType mediaSourceType = MediaSourceType.media;
  Map<String, String> metas = {};
  Map<String, dynamic> extras = {};
  late MediaType mediaType;
  late String resource;

  /// Makes [Media] object from a [File].
  static Future<Media> file(File file,
      {bool parse: false,
      Map<String, dynamic>? extras,
      Duration timeout: const Duration(seconds: 10)}) async {
    Media media = new _Media();
    media.mediaType = MediaType.file;
    media.resource = file.path;
    if (parse) {
      await media.parse(timeout);
      mediaMetas[media.id] = media.metas;
    }
    if (extras != null) {
      media.extras = extras;
      mediaExtras[media.id] = extras;
    }
    return media;
  }

  /// Makes [Media] object from url.
  static Future<Media> network(dynamic url,
      {bool parse: false,
      Map<String, dynamic>? extras,
      Duration timeout: const Duration(seconds: 10)}) async {
    Media media = new _Media();
    media.mediaType = MediaType.network;
    if (url is Uri)
      media.resource = url.toString();
    else
      media.resource = url;
    if (parse) {
      await media.parse(timeout);
      mediaMetas[media.id] = media.metas;
    }
    if (extras != null) {
      media.extras = extras;
      mediaExtras[media.id] = extras;
    }
    return media;
  }

  /// Makes [Media] object from a asset.
  static Future<Media> asset(String path,
      {bool parse: false,
      Map<String, dynamic>? extras,
      Duration timeout: const Duration(seconds: 10)}) async {
    Media media = new _Media();
    media.mediaType = MediaType.asset;
    media.resource = path;
    ByteData mediaByteData = await rootBundle.load(path);
    Uint8List mediaBytes = mediaByteData.buffer.asUint8List();
    File mediaFile = new File(_path.join(
      (await _path.getTemporaryDirectory()).path,
      path,
    ));
    await mediaFile.create(recursive: true);
    await mediaFile.writeAsBytes(mediaBytes);
    if (parse) {
      await media.parse(timeout);
      mediaMetas[media.id] = media.metas;
    }
    if (extras != null) {
      media.extras = extras;
      mediaExtras[media.id] = extras;
    }
    return media;
  }

  /// Makes [Media] object from direct show.
  static Future<Media> directShow(
      {String vdev = '', String adev = '', required int liveCaching}) async {
    Media media = new _Media();
    media.mediaType = MediaType.directShow;
    media.resource =
        'dshow:// :dshow-vdev=$vdev :dshow-adev=$adev :live-caching=$liveCaching';
    return media;
  }

  /// Internally used method to easily transform data for sending through Platform channel.
  static Media fromMap(dynamic map) {
    Media media = new _Media();
    media.id = map['id'];
    media.mediaType = {
      'MediaType.file': MediaType.file,
      'MediaType.network': MediaType.network,
      'MediaType.asset': MediaType.asset,
    }[map['mediaType']]!;
    media.resource = map['resource'];
    media.metas = mediaMetas[media.id] ?? {};
    media.extras = mediaExtras[media.id] ?? {};
    return media;
  }

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap() => {
        'id': this.id,
        'mediaSourceType': this.mediaSourceType.toString(),
        'mediaType': this.mediaType.toString(),
        'resource': this.resource,
      };

  Future<void> parse(Duration timeout) async {
    dynamic metas = await channel.invokeMethod('Media.parse', {
      'timeout': timeout.inMilliseconds,
      'source': this.toMap(),
    });
    this.metas['title'] = metas['title'];
    this.metas['artist'] = metas['artist'];
    this.metas['genre'] = metas['genre'];
    this.metas['copyright'] = metas['copyright'];
    this.metas['trackNumber'] = metas['trackNumber'];
    this.metas['description'] = metas['description'];
    this.metas['rating'] = metas['rating'];
    this.metas['date'] = metas['date'];
    this.metas['settings'] = metas['settings'];
    this.metas['url'] = metas['url'];
    this.metas['language'] = metas['language'];
    this.metas['nowPlaying'] = metas['nowPlaying'];
    this.metas['encodedBy'] = metas['encodedBy'];
    this.metas['artworkUrl'] = metas['artworkUrl'];
    this.metas['trackTotal'] = metas['trackTotal'];
    this.metas['director'] = metas['director'];
    this.metas['season'] = metas['season'];
    this.metas['episode'] = metas['episode'];
    this.metas['actors'] = metas['actors'];
    this.metas['albumArtist'] = metas['albumArtist'];
    this.metas['discNumber'] = metas['discNumber'];
    this.metas['discTotal'] = metas['discTotal'];
    this.metas['duration'] = metas['duration'];
  }
}
