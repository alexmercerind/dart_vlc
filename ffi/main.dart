import 'dart:io';

import 'package:dart_vlc_ffi/dart_vlc.dart';


String get dynamicLibraryPath {
  String directory = Platform.script.path.split('/').sublist(0, Platform.script.path.split('/').length - 1).join('/');
  return directory + '/' + 'dart_vlc.so';
}

Future<void> main() async {
  DartVLC.initialize(dynamicLibraryPath);
  Equalizer.createMode(
    EqualizerMode.live
  );
}
