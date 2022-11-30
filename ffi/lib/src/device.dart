import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';

/// Represents a playback [Device] for the [Player].
class Device {
  /// ID corresponding to the [Device].
  final String id;

  /// Name of the [Device].
  final String name;

  const Device(this.id, this.name);
}

/// [Devices.all] getter is used to get [List] of all available [Device] for playback in the [Player].
class Devices {
  /// Gets [List] of all available playback [Device].
  static List<Device> get all {
    List<Device> devices = <Device>[];
    final devicesPtr = DevicesFFI.all(devices);
    for (int i = 0; i < devicesPtr.ref.size; i++) {
      final devicePtr = devicesPtr.ref.devices.elementAt(i);
      devices.add(
        Device(
          devicePtr.ref.id.toDartString(),
          devicePtr.ref.name.toDartString(),
        ),
      );
    }
    return devices;
  }
}
