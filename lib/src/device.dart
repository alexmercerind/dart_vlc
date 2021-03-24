import 'dart:io';
import 'package:dart_vlc/src/channel.dart';

/// Represents a playback [Device] for the [Player].
class Device {
  /// ID corresponding to the [Device].
  String id;

  /// Name of the [Device].
  String name;

  Device(this.id, this.name);

  /// Internally used method to easily transform data for sending through Platform channel.
  static Device fromMap(dynamic map) => Device(
        Platform.isWindows
            ? (map['id'] != ''
                ? '{0.0.0.00000000}.' + map['id'].toLowerCase()
                : '')
            : map['id'],
        map['name'],
      );

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, String> toMap() => {
        'id': this.id,
        'name': this.name,
      };
}

/// [Devices.all] getter is used to get [List] of all available [Device] for playback in the [Player].
class Devices {
  /// Gets [List] of all available playback [Device].
  static Future<List<Device>> get all async {
    dynamic devices = await channel.invokeMethod('Devices.all', {});
    return devices
        .map(
          (dynamic device) => Device.fromMap(device),
        )
        .toList()
        .cast<Device>();
  }
}
