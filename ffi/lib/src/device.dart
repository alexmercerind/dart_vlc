import 'dart:ffi';
import 'package:ffi/ffi.dart';
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
    Pointer<Pointer<Utf8>> devicesPtr = DevicesFFI.all();
    int count = int.parse(devicesPtr.elementAt(0).value.toDartString());
    for (int i = 1; i < 2 * count; i += 2) {
      devices.add(Device(devicesPtr.elementAt(1).value.toDartString(),
          devicesPtr.elementAt(2).value.toDartString()));
    }
    return devices;
  }
}
