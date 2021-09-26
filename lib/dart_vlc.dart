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
import 'package:flutter/foundation.dart';
import 'package:path/path.dart' as path;
import 'package:flutter/services.dart';

import 'package:dart_vlc/src/widgets/video.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart' as FFI;
import 'package:dart_vlc_ffi/dart_vlc_ffi.dart' as FFI;
export 'package:dart_vlc_ffi/dart_vlc_ffi.dart' hide DartVLC, Player;
export 'package:dart_vlc/src/widgets/video.dart';

/// Platform channel for using [Texture] & flutter::TextureRegistrar on Windows.
final MethodChannel _channel = MethodChannel('dart_vlc');

/// A [Player] to open & play a [Media] or [Playlist] from file, network or asset.
///
/// Use [Player] constructor to create a new instance of a [Player].
/// Provide a unique [id] while instanciating.
///
/// ```dart
/// Player player = new Player(id: 0);
/// ```
///
/// By default, [Video] widget will adapt to the size of the currently playing video itself.
/// If you wish to override the default video size,
/// then you can pass [videoDimensions] argument to override the frame size as follows.
///
/// ```dart
/// Player player = new Player(
///   id: 0,
///   videoDimensions: const VideoDimensions(480, 360)
/// );
/// ```
///
/// Use various methods & event streams available to control & listen to events of the playback.
///
class Player extends FFI.Player {
  final ValueNotifier<int?> textureId = ValueNotifier<int?>(null);

  Player(
      {required int id,
      FFI.VideoDimensions? videoDimensions,
      List<String>? commandlineArguments})
      : super(
            id: id,
            videoDimensions: videoDimensions,
            commandlineArguments: commandlineArguments) {
    () async {
      textureId.value = await _channel.invokeMethod('PlayerRegisterTexture', {'playerId': id});
    }();
  }

  @override
  void dispose() async {
    await _channel.invokeMethod('PlayerUnregisterTexture', {'playerId': id});
    textureId.value = null;
    super.dispose();
  }
}

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
      if (videoStreamControllers[playerId] != null &&
          FFI.players[playerId] != null) {
        if (!videoStreamControllers[playerId]!.isClosed) {
          videoStreamControllers[playerId]!.add(VideoFrame(
              playerId: playerId,
              videoWidth: FFI.players[playerId]!.videoDimensions.width,
              videoHeight: FFI.players[playerId]!.videoDimensions.height,
              byteArray: videoFrame));
        }
      }
    };
    if (Platform.isWindows) {
      final libraryPath = path.join(
          path.dirname(Platform.resolvedExecutable), 'dart_vlc_plugin.dll');
      FFI.DartVLC.initialize(libraryPath);
    } else if (Platform.isLinux) {
      final libraryPath = path.join(path.dirname(Platform.resolvedExecutable),
          'lib', 'libdart_vlc_plugin.so');
      FFI.DartVLC.initialize(libraryPath);
    } else if (Platform.isMacOS) {
      final libraryPath = path.join(
          path.dirname(path.dirname(Platform.resolvedExecutable)),
          'Frameworks',
          'dart_vlc.framework',
          'dart_vlc');
      FFI.DartVLC.initialize(libraryPath);
    } else if (Platform.isIOS) {
      final libraryPath = path.join(
        path.dirname(Platform.resolvedExecutable),
        'Frameworks',
        'dart_vlc.framework',
        'dart_vlc',
      );
      FFI.DartVLC.initialize(libraryPath);
    }
  }
}
