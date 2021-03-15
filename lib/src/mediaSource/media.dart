import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/services.dart';
import 'package:path/path.dart' as _path;
import 'package:path_provider/path_provider.dart' as _path;
import 'package:dart_vlc/src/mediaSource/mediaSource.dart';
import 'package:dart_vlc/src/enums/mediaSourceType.dart';
import 'package:dart_vlc/src/enums/mediaType.dart';

/// A media object to open inside a [Player].
///
/// * A [Media] from a [File].
///
/// ```dart
/// Media media = Media.file(new File('C:/music.ogg'));
/// ```
///
/// * A [Media] from a [Uri].
///
/// ```dart
/// Media media = Media.network('http://alexmercerind.github.io/music.mp3');
/// ```
///
/// * A [Media] from assets.
///
/// ```dart
/// Media media = Media.asset('asset/media/music.aac');
/// ```
///
class Media extends MediaSource {
  MediaSourceType mediaSourceType = MediaSourceType.media;
  MediaType mediaType;
  String resource;

  /// Makes [Media] object from a [File].
  static Media file(File file) {
    Media media = new Media();
    media.mediaType = MediaType.file;
    media.resource = file.path;
    return media;
  }

  /// Makes [Media] object from url.
  static Media network(dynamic url) {
    Media media = new Media();
    media.mediaType = MediaType.network;
    if (url is Uri)
      media.resource = url.toString();
    else
      media.resource = url;
    return media;
  }

  /// Makes [Media] object from a asset.
  static Future<Media> asset(String path) async {
    Media media = new Media();
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
    return media;
  }

  /// Internally used method to easily transform data for sending through Platform channel.
  static Media fromMap(dynamic map) {
    Media media = new Media();
    media.mediaType = {
      'MediaType.file': MediaType.file,
      'MediaType.network': MediaType.network,
      'MediaType.asset': MediaType.asset,
    }[map['mediaType']];
    media.resource = map['resource'];
    return media;
  }

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap() => {
        'mediaSourceType': this.mediaSourceType.toString(),
        'mediaType': this.mediaType.toString(),
        'resource': this.resource
      };
}
