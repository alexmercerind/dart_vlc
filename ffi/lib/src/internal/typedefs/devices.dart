import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Following typedef is used for:
/// Devices::all
typedef DevicesAllCXX = Pointer<Pointer<Utf8>> Function();
typedef DevicesAllDart = Pointer<Pointer<Utf8>> Function();

/// Following typedef is used for cleaning memory used by char**.
typedef DevicesClearCXX = Void Function();
typedef DevicesClearDart = void Function();
