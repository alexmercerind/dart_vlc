import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef MediaParseCXX = Pointer<Pointer<Utf8>> Function(
    Handle object, Pointer<Utf8> type, Pointer<Utf8> resource, Int32 timeout);
typedef MediaParseDart = Pointer<Pointer<Utf8>> Function(
    Object object, Pointer<Utf8> type, Pointer<Utf8> resource, int timeout);
