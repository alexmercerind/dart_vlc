import 'package:dart_vlc/src/channel.dart';

/// Represents a [Device] for playback in the [Player].
class Device {
  /// Name of the [Device].
  String name;
  /// Description of the [Device].
  String description;

  Device(this.name, this.description);

  /// Internally used method to easily transform data for sending through Platform channel.
  static Device fromMap(dynamic map) => Device(
    map['name'],
    map['description']
  );

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, String> toMap() => {
    'name': this.name,
    'description': this.description,
  };
}

/// [Devices.all] getter is used to get [List] of all available [Device] for playback in the [Player].
class Devices {

  /// Gets [List] of all available playback [Device].
  static Future<List<Devices>> get all {
    dynamic devices = channel.invokeMethod(
      'getDevices',
      {}
    );
    return devices.map(
      (dynamic device) => Device.fromMap(device),
    ).toList();
  } 
}
