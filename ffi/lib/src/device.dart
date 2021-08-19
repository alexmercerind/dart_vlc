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
    final _devices = DevicesFFI.all(devices);
    for (int i = 0; i < _devices.ref.size; i++) {
      final _device = _devices.ref.devices.elementAt(i);
      devices.add(Device(
          _device.ref.id.toDartString(), _device.ref.name.toDartString()));
    }
    return devices;
  }
}
