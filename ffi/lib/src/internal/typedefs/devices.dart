import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef DevicesAllCXX = Pointer<Pointer<Utf8>> Function();
typedef DevicesAllDart = Pointer<Pointer<Utf8>> Function();
typedef DevicesClearCXX = Void Function();
typedef DevicesClearDart = void Function();
