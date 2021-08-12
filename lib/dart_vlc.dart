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
/// If you wish to use this instance for [Video] playback, then provide [videoWidth] & [videoHeight] optional parameters.
/// Higher value may lead to degraded performance.
///
/// ```dart
/// Player player = new Player(
///   id: 0,
///   videoWidth: 1920,
///   videoHeight: 1080,
/// );
/// ```
///
/// Do not provide [videoWidth] & [videoHeight], if you wish to use the [Player] for only audio playback.
///
/// Use various methods & event streams avaiable to control & listen to events of the playback.
///
class Player extends FFI.Player {
  final ValueNotifier<int?> textureId = ValueNotifier<int?>(null);

  Player(
      {required int id,
      int videoWidth: 0,
      int videoHeight: 0,
      List<String>? commandlineArguments})
      : super(
            id: id,
            videoWidth: videoWidth,
            videoHeight: videoHeight,
            commandlineArguments: commandlineArguments) {
    if (videoHeight > 0 && videoWidth > 0 && Platform.isWindows) {
      () async {
        textureId.value = await _channel.invokeMethod('createTexture', {
          'playerId': id,
          'videoWidth': videoWidth,
          'videoHeight': videoHeight
        });
      }();
    }
  }

  @override
  void dispose() async {
    if (textureId.value != null) {
      await _channel.invokeMethod('disposeTexture', {'playerId': id});
      textureId.value = null;
    }

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
          videoStreamControllers[playerId]!.add(new VideoFrame(
              playerId: playerId,
              videoWidth: FFI.players[playerId]!.videoWidth,
              videoHeight: FFI.players[playerId]!.videoHeight,
              byteArray: videoFrame));
        }
      }
    };
    if (Platform.isWindows) {
      String directory = Platform.resolvedExecutable
          .split('\\')
          .sublist(0, Platform.resolvedExecutable.split('\\').length - 1)
          .join('\\');
      FFI.DartVLC.initialize(directory + '\\' + 'dart_vlc_plugin.dll');
    }
    if (Platform.isLinux) {
      String directory = Platform.resolvedExecutable
          .split('/')
          .sublist(0, Platform.resolvedExecutable.split('/').length - 1)
          .join('/');
      FFI.DartVLC.initialize(directory + '/lib/' + 'libdartvlc.so');
    }
  }
}
