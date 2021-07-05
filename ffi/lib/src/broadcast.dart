import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/mediaSource/media.dart';

/// Internally used class to avoid direct creation of the object of a [Broadcast] class.
class _Broadcast extends Broadcast {}

/// Used to declare a [Broadcast] configuration.
///
/// An example configuration for a [Broadcast] can be as follows.
///
/// ```dart
/// new BroadcastConfiguration(
///   access: 'http',
///   mux: 'mpeg1',
///   dst: '127.0.0.1:8080',
///   vcodec: 'mp1v',
///   vb: 1024,
///   acodec: 'mpga',
///   ab: 128,
/// );
/// ```
class BroadcastConfiguration {
  /// Type of access for [Broadcast] e.g. `http`.
  final String access;

  /// MUX e.g. `mpeg1`.
  final String mux;

  /// Destination e.g. `127.0.0.1:8080`.
  final String dst;

  /// Video codec for transcoding the broadcast e.g. `mp1v`.
  final String vcodec;

  /// Video bitrate for transcoding the broadcast.
  final int vb;

  /// Audio codec for transcoding the broadcast e.g. `mpga`.
  final String acodec;

  /// Audio bitrate for transcoding the broadcast.
  final int ab;

  const BroadcastConfiguration({
    required this.access,
    required this.mux,
    required this.dst,
    required this.vcodec,
    required this.vb,
    required this.acodec,
    required this.ab,
  });
}

/// Creates new [Broadcast] for a [Media].
///
/// Example creation can be as follows.
///
/// ```dart
/// Broadcast broadcast = Broadcast.create(
///   id: 0,
///   media: await Media.file(new File('C:/music.ogg')),
///   configuration: new BroadcastConfiguration(
///     access: 'http',
///     mux: 'mpeg1',
///     dst: '127.0.0.1:8080',
///     vcodec: 'mp1v',
///     vb: 1024,
///     acodec: 'mpga',
///     ab: 128,
///   ),
/// );
///
/// broadcast.start();
/// ```
///
/// Call [Broadcast.dispose] for releasing the resources.
///
abstract class Broadcast {
  /// ID for this broadcast.
  late int id;

  /// Broadcasting [Media].
  late Media media;

  /// Configuration of this broadcast.
  late BroadcastConfiguration configuration;

  /// Creates a new [Broadcast] instance.
  static Broadcast create(
      {required int id,
      required Media media,
      required BroadcastConfiguration configuration}) {
    Broadcast broadcast = new _Broadcast();
    broadcast.id = id;
    broadcast.media = media;
    broadcast.configuration = configuration;
    BroadcastFFI.create(
        id,
        media.mediaType.toString().toNativeUtf8(),
        media.resource.toNativeUtf8(),
        configuration.access.toNativeUtf8(),
        configuration.mux.toNativeUtf8(),
        configuration.dst.toNativeUtf8(),
        configuration.vcodec.toNativeUtf8(),
        configuration.vb,
        configuration.acodec.toNativeUtf8(),
        configuration.ab);
    return broadcast;
  }

  /// Starts broadcasting the [Media].
  void start() {
    BroadcastFFI.start(this.id);
  }

  /// Disposes this instance of [Broadcast].
  void dispose() {
    BroadcastFFI.dispose(this.id);
  }
}
