
import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/channel.dart';

class Chromecast {

  /// ID for this Chromecast.
  late int id;

  /// Sending from [Media].
  late Media media;

  /// Ip address of your chromecast: `192.168.1.XXX`
  late String chromecastIpAddress;

  Chromecast({
    required this.id,
    required this.media,
    required this.chromecastIpAddress
  });

  /// Creates a new [Chromecast] instance.
  static Future<Chromecast> create({ required int id, required Media media, required String chromecastIpAddress }) async {    
    await channel.invokeMethod(
      'Chromecast.create',
      {
        'id': id,
        'media': media.toMap(),
        'chromecastIpAddress': chromecastIpAddress,
      },
    );
    return new Chromecast(id: id, media: media, chromecastIpAddress: chromecastIpAddress);
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