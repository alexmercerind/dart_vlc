// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

// ignore_for_file: implementation_imports
import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/cupertino.dart';
import 'package:flutter_native_view/flutter_native_view.dart';
import 'package:path/path.dart' as path;

import 'package:dart_vlc/channel.dart';
import 'package:dart_vlc/src/widgets/video.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart' as FFI;
import 'package:dart_vlc_ffi/dart_vlc_ffi.dart' as FFI;
export 'package:dart_vlc_ffi/dart_vlc_ffi.dart' hide DartVLC, Player;
export 'package:dart_vlc/src/widgets/video.dart';
export 'package:dart_vlc/src/widgets/native_video.dart';

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
  final bool registerTexture;
  final ValueNotifier<int?> textureId = ValueNotifier<int?>(null);

  Player({
    required int id,
    this.registerTexture = true,
    FFI.VideoDimensions? videoDimensions,
    List<String>? commandlineArguments,
    bool: false,
  }) : super(
            id: id,
            videoDimensions: videoDimensions,
            commandlineArguments: commandlineArguments) {
    () async {
      if (registerTexture) {
        textureId.value = await channel
            .invokeMethod(kPlayerRegisterTexture, {'playerId': id});
      }
    }();
  }

  @override
  void dispose() async {
    if (registerTexture) {
      await channel.invokeMethod(kPlayerUnregisterTexture, {'playerId': id});
    }
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
abstract class DartVLC {
  static Future<void> initialize({
    bool useFlutterNativeView = false,
  }) async {
    if (useFlutterNativeView && Platform.isWindows) {
      /// Windows specific [NativeVideo].
      WidgetsFlutterBinding.ensureInitialized();
      await FlutterNativeView.ensureInitialized();
    }
    FFI.videoFrameCallback = (int playerId, Uint8List videoFrame) {
      if (videoStreamControllers[playerId] != null &&
          FFI.players[playerId] != null) {
        if (!videoStreamControllers[playerId]!.isClosed) {
          videoStreamControllers[playerId]!.add(
            VideoFrame(
              playerId: playerId,
              videoWidth: FFI.players[playerId]!.videoDimensions.width,
              videoHeight: FFI.players[playerId]!.videoDimensions.height,
              byteArray: videoFrame,
            ),
          );
        }
      }
    };
    if (Platform.isWindows) {
      final libraryPath = path.join(
        path.dirname(Platform.resolvedExecutable),
        'dart_vlc_plugin.dll',
      );
      FFI.DartVLC.initialize(libraryPath);
    } else if (Platform.isLinux) {
      final libraryPath = path.join(
        path.dirname(Platform.resolvedExecutable),
        'lib',
        'libdart_vlc_plugin.so',
      );
      FFI.DartVLC.initialize(libraryPath);
    } else if (Platform.isMacOS) {
      final libraryPath = path.join(
        path.dirname(path.dirname(Platform.resolvedExecutable)),
        'Frameworks',
        'dart_vlc.framework',
        'dart_vlc',
      );
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
