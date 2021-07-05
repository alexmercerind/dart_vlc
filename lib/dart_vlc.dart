/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */


// ignore_for_file: implementation_imports
import 'dart:io';
import 'dart:typed_data';
import 'package:dart_vlc/src/widgets/video.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart' as FFI;
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
    FFI.videoFrameCallback = (int playerId, Uint8List videoFrame) {
      if (videoStreamControllers[playerId] != null && FFI.players[playerId] != null) {
        videoStreamControllers[playerId]!.add(
          new VideoFrame(
            playerId: playerId,
            videoWidth: FFI.players[playerId]!.videoWidth,
            videoHeight: FFI.players[playerId]!.videoHeight,
            byteArray: videoFrame
          )
        );
      }
    };
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
