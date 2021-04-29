import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/channel.dart';


/// Internally used class to avoid direct creation of the object of a [Chromecast] class.
class _Chromecast extends Chromecast {}


class Chromecast {
  /// ID for this [Chromecast].
  late int id;

  /// Sending from [Media].
  late Media media;

  /// IP address of your chromecast: `192.168.1.XXX`
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
    await channel.invokeMethod(
      'Chromecast.create',
      {
        'id': id,
        'media': media.toMap(),
        'ipAddress': ipAddress,
      },
    );
    return chromecast;
  }

  /// Start sending [Media] content to chromecast
  Future<void> send() async {
    await channel.invokeMethod(
      'Chromecast.send',
      {
        'id': id,
      },
    );
  }

  /// Disposes this instance of [Chromecast].
  Future<void> stop() async {
    await channel.invokeMethod(
      'Chromecast.dispose',
      {
        'id': id,
      },
    );
  }
}
