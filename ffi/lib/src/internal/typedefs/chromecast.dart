import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Following typedef is used for:
/// Chromecast::create
typedef ChromecastCreateCXX = Void Function(Int32 id, Pointer<Utf8> type, Pointer<Utf8> resource, Pointer<Utf8> ipAddress);
typedef ChromecastCreateDart = void Function(int id, Pointer<Utf8> type, Pointer<Utf8> resource, Pointer<Utf8> ipAddress);

/// Following typedef is used for:
/// Chromecast::start
typedef ChromecastStartCXX = Void Function(Int32 id);
typedef ChromecastStartDart = void Function(int id);

/// Following typedef is used for:
/// Chromecast::dispose
typedef ChromecastDisposeCXX = Void Function(Int32 id);
typedef ChromecastDisposeDart = void Function(int id);
