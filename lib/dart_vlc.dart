/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

import 'dart:io';

import 'package:dart_vlc_ffi/dart_vlc_ffi.dart' as FFI;
export 'package:dart_vlc_ffi/dart_vlc_ffi.dart' hide DartVLC;

export 'package:dart_vlc/src/widgets/video.dart';

/// Initializes the DartVLC plugin.
/// 
/// ```dart
/// void main() {
///   DartVLC.initialize();
///   runApp(MyApp());
/// }
/// ```
/// 
abstract class DartVLC {
  static void initialize() {
    if (Platform.isLinux) {
      String directory = Platform.resolvedExecutable
      .split('/')
      .sublist(0, Platform.resolvedExecutable.split('/').length - 1)
      .join('/');
      FFI.DartVLC.initialize(
        directory + '/lib/' + 'libdartvlc.so'
      );
    }
  }
}
