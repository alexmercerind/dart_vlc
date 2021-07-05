import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Following typedef is used for:
/// Devices::all
typedef DevicesAllCXX = Pointer<Pointer<Utf8>> Function();
typedef DevicesAllDart = Pointer<Pointer<Utf8>> Function();
