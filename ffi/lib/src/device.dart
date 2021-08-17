import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/devices.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';

/// Represents a playback [Device] for the [Player].
class Device {
  /// ID corresponding to the [Device].
  String id;

  /// Name of the [Device].
  String name;

  Device(this.id, this.name);
}

/// [Devices.all] getter is used to get [List] of all available [Device] for playback in the [Player].
class Devices {
  /// Gets [List] of all available playback [Device].
  static List<Device> get all {
    List<Device> devices = <Device>[];
    DevicesStruct _devices = DevicesFFI.all(devices);
    for (int i = 0; i < _devices.size; i++) {
      devices.add(
        Device(
          _devices.device_ids.elementAt(i).value.toDartString(),
          _devices.device_names.elementAt(i).value.toDartString(),
        ),
      );
    }
    return devices;
  }
}
