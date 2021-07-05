import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/mediaSource/media.dart';

/// Internally used class to avoid direct creation of the object of a [Chromecast] class.
class _Chromecast extends Chromecast {}

class Chromecast {
  /// ID for this [Chromecast].
  late int id;

  /// Chromecast [Media].
  late Media media;

  /// IP address of your chromecast e.g. `192.168.1.XXX`.
  late String ipAddress;

  /// Creates a new [Chromecast] instance.
  static Future<Chromecast> create(
      {required int id,
      required Media media,
      required String ipAddress}) async {
    Chromecast chromecast = new _Chromecast();
    chromecast.id = id;
    chromecast.media = media;
    chromecast.ipAddress = ipAddress;
    ChromecastFFI.create(id, media.mediaType.toString().toNativeUtf8(),
        media.resource.toNativeUtf8(), ipAddress.toNativeUtf8());
    return chromecast;
  }

  /// Start sending [Media] content to chromecast
  void start() {
    ChromecastFFI.start(this.id);
  }

  /// Disposes this instance of [Chromecast].
  void dispose() {
    ChromecastFFI.dispose(this.id);
  }
}
