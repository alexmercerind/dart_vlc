import 'dart:ffi';
import 'package:ffi/ffi.dart';

import 'lib/dart_vlc_ffi.dart';


extension on List<String> {
  /// Converts List<String> to const char**.
  Pointer<Pointer<Utf8>> toNativeUtf8Array() {
    final List<Pointer<Utf8>> listPointer = this.map((String string) => string.toNativeUtf8()).toList().cast<Pointer<Utf8>>();
    final Pointer<Pointer<Utf8>> pointerPointer = calloc.allocate(this.join('').length);
    for (int index = 0; index < this.length; index++) pointerPointer[index] = listPointer[index];
    return pointerPointer;
  }
}

void main() {
  PlayerFFI.create(0, 0, 0);
  PlayerFFI.open(
    0,
    1,
    ['0', 'MediaType.file', '/home/alexmercerind/music.OGG', '1', 'MediaType.file', '/home/alexmercerind/audio.OGG'].toNativeUtf8Array(),
    2
  );
  while (true) {}
}