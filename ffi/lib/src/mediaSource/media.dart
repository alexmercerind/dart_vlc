import 'dart:io';
import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:path/path.dart' as path;
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/mediaSource/mediaSource.dart';
import 'package:dart_vlc_ffi/src/enums/mediaSourceType.dart';
import 'package:dart_vlc_ffi/src/enums/mediaType.dart';

/// A media object to open inside a [Player].
///
/// Pass `true` to [parse] for retrieving the metadata of the [Media].
/// [timeout] sets the time-limit for retriveing metadata.
/// [Media.metas] can be then, accessed to get the retrived metadata as `Map<String, String>`.
///
/// * A [Media] from a [File].
///
/// ```dart
/// Media media = Media.file(File('C:/music.ogg'));
/// ```
///
/// * A [Media] from a [Uri].
///
/// ```dart
/// Media media = Media.network('http://alexmercerind.github.io/music.mp3');
/// ```
///
/// For starting a [Media] at particular time, one can pass `startTime`.
///
/// ```dart
/// Media media = Media.file(
///   File('C:/music.ogg'),
///   startTime: Duration(milliseconds: 20),
/// );
/// ```
///
class Media implements MediaSource {
  MediaSourceType get mediaSourceType => MediaSourceType.media;
  final MediaType mediaType;
  final String resource;
  final Duration startTime;
  final Duration stopTime;
  final Map<String, String> metas;

  const Media._({
    required this.mediaType,
    required this.resource,
    required this.metas,
    this.startTime: Duration.zero,
    this.stopTime: Duration.zero,
  });

  /// Makes [Media] object from a [File].
  factory Media.file(
    File file, {
    bool parse: false,
    Map<String, dynamic>? extras,
    Duration timeout: const Duration(seconds: 10),
    startTime: Duration.zero,
    stopTime: Duration.zero,
  }) {
    final media = Media._(
      mediaType: MediaType.file,
      resource: file.path,
      metas: {},
      startTime: startTime,
      stopTime: stopTime,
    );
    if (parse) {
      media.parse(timeout);
    }
    return media;
  }

  /// Makes [Media] object from url.
  factory Media.network(
    dynamic url, {
    bool parse: false,
    Map<String, dynamic>? extras,
    Duration timeout: const Duration(seconds: 10),
    startTime: Duration.zero,
    stopTime: Duration.zero,
  }) {
    final resource = (url is Uri) ? url.toString() : url;
    final Media media = Media._(
      mediaType: MediaType.network,
      resource: resource,
      metas: {},
      startTime: startTime,
      stopTime: stopTime,
    );
    if (parse) {
      media.parse(timeout);
    }
    return media;
  }

  /// Makes [Media] object from direct show.
  factory Media.directShow(
      {String? rawUrl,
      Map<String, dynamic>? args,
      String? vdev,
      String? adev,
      int? liveCaching}) {
    final resourceUrl = rawUrl ??
        _buildDirectShowUrl(args ??
            {
              'dshow-vdev': vdev,
              'dshow-adev': adev,
              'live-caching': liveCaching
            });

    return Media._(
        mediaType: MediaType.directShow, resource: resourceUrl, metas: {});
  }

  /// Makes [Media] object from assets.
  ///
  /// **WARNING**
  ///
  /// This method only works for Flutter.
  /// Might result in an exception on Dart CLI.
  ///
  factory Media.asset(
    String asset, {
    startTime: Duration.zero,
  }) {
    String? assetPath;
    if (Platform.isWindows || Platform.isLinux) {
      assetPath = path.join(
        path.dirname(Platform.resolvedExecutable),
        'data',
        'flutter_assets',
        asset,
      );
    } else if (Platform.isMacOS) {
      assetPath = path.join(
        path.dirname(Platform.resolvedExecutable),
        '..',
        'Frameworks',
        'App.framework',
        'Resources',
        'flutter_assets',
        asset,
      );
    } else if (Platform.isIOS) {
      assetPath = path.join(
        path.dirname(Platform.resolvedExecutable),
        'Frameworks',
        'App.framework',
        'flutter_assets',
        asset,
      );
    }
    if (assetPath == null) {
      // TODO: Add Android support.
      throw UnimplementedError('The platform is not supported');
    }
    final url = Uri.file(assetPath, windows: Platform.isWindows);
    return Media._(
      mediaType: MediaType.asset,
      resource: url.toString(),
      metas: {},
      startTime: startTime,
    );
  }

  /// Parses the [Media] to retrieve [Media.metas].
  void parse(Duration timeout) {
    Pointer<Pointer<Utf8>> metas = MediaFFI.parse(
        this,
        this.mediaType.toString().toNativeUtf8(),
        this.resource.toNativeUtf8(),
        timeout.inMilliseconds);
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
  }

  static String _buildDirectShowUrl(Map<String, dynamic> args) {
    return args.entries.fold(
        'dshow://',
        (prev, pair) =>
            prev +
            (pair.value != null
                ? ' :${pair.key.toLowerCase()}=${pair.value}'
                : ''));
  }

  int get hashCode => mediaType.hashCode ^ resource.hashCode;

  bool operator ==(Object other) =>
      other is Media &&
      other.mediaType == mediaType &&
      other.resource == resource;

  @override
  String toString() => '[$mediaType]$resource';
}

extension DurationExtension on Duration {
  String argument(String value) => this == Duration.zero
      ? ''
      : ':$value=${(inMilliseconds / 1000).toStringAsFixed(6)}';
}
