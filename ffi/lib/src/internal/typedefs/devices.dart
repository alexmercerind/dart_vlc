import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Struct received from C with devices data.
class DevicesStruct extends Struct {
  // ignore: non_constant_identifier_names
  external Pointer<Pointer<Utf8>> device_ids;

  external Pointer<Pointer<Utf8>> device_names;

  @Int32()
  external int size;
}

typedef DevicesAllCXX = DevicesStruct Function(Handle object);
typedef DevicesAllDart = DevicesStruct Function(Object object);
