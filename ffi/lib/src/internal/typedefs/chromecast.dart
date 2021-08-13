import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef ChromecastCreateCXX = Void Function(Int32 id, Pointer<Utf8> type,
    Pointer<Utf8> resource, Pointer<Utf8> ipAddress);
typedef ChromecastCreateDart = void Function(int id, Pointer<Utf8> type,
    Pointer<Utf8> resource, Pointer<Utf8> ipAddress);
typedef ChromecastStartCXX = Void Function(Int32 id);
typedef ChromecastStartDart = void Function(int id);
typedef ChromecastDisposeCXX = Void Function(Int32 id);
typedef ChromecastDisposeDart = void Function(int id);
