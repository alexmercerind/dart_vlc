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
import 'dart:async';

import 'package:dart_vlc/channel.dart';
import 'package:flutter/material.dart';

import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/widgets/controls.dart';
import 'package:flutter_native_view/flutter_native_view.dart';

/// {@template nativevideo}
///
/// A widget for showing video inside the [Widget] tree.
/// This [Widget] is more performant compared to [Video] & uses [flutter_native_view](https://github.com/alexmercerind/flutter_native_view.git)
/// to embed the video output directly without any texture interop or pixel-buffer copy calls.
///
/// But, it is highly dependent on platform & other limitations apply. In general, this widget is more performant & should be used if possible.
///
/// An example configuration between a [Player] and a [NativeVideo] can be as follows.
///
/// Register the plugin with `useFlutterNativeView`.
/// ```dart
/// DartVLC.initilize(useFlutterNativeView: true);
/// ```
/// Pass `registerTexture` as `false` when creating [Player] & use [NativeVideo] widget.
/// ```dart
/// class _MyAppState extends State<MyApp> {
///   Player player = Player(id: 0, registerTexture: false);
///
///   @override
///   Widget build(BuildContext context) {
///     return Scaffold(
///       body: Center(
///         child: NativeVideo(
///           player: player,
///           height: 420.0,
///           width: 320.0
///         ),
///       ),
///     );
///   }
/// }
/// ```
///
/// **Supported Platforms**
/// - [x] Windows
///
/// This [Widget] internally uses [StreamController].
/// Prefer calling [Player.stop] & [NativeVideo.dispose] to freed the resources.
/// A global [Key] may be used for this purpose.
/// {@endtemplate}
class NativeVideo extends StatefulWidget {
  /// {@macro nativevideo}
  NativeVideo({
    Key? key,
    required Player player,
    this.width,
    this.height,
    this.fit: BoxFit.contain,
    this.alignment: Alignment.center,
    this.scale: 1.0,
    this.showControls: true,
    this.progressBarActiveColor,
    this.progressBarInactiveColor = Colors.white24,
    this.progressBarThumbColor,
    this.progressBarThumbGlowColor = const Color.fromRGBO(0, 161, 214, .2),
    this.volumeActiveColor,
    this.volumeInactiveColor = Colors.grey,
    this.volumeBackgroundColor = const Color(0xff424242),
    this.volumeThumbColor,
    this.progressBarThumbRadius = 10.0,
    this.progressBarThumbGlowRadius = 15.0,
    this.showTimeLeft = false,
    this.progressBarTextStyle = const TextStyle(),
    this.filterQuality = FilterQuality.low,
  })  : player = player,
        super(key: key);

  /// The [Player] whose [NativeVideo] output should be shown.
  final Player player;

  /// Width of the viewport.
  /// Defaults to the width of the parent.
  final double? width;

  /// Height of the viewport.
  /// Defaults to the height of the parent.
  final double? height;

  /// How to inscribe the picture box into the player viewport.
  /// Defaults to [BoxFit.contain].
  final BoxFit fit;

  /// How to align the picture box within the player viewport.
  /// Defaults to [Alignment.center]
  final AlignmentGeometry alignment;

  /// Scale.
  final double scale;

  /// Filter quality.
  final FilterQuality filterQuality;

  /// Built-In video controls.
  final bool showControls;

  /// Radius of the progressbar's thumb
  final double? progressBarThumbRadius;

  /// Radius of the progressbar's glow of the thumb
  final double? progressBarThumbGlowRadius;

  /// Active color of the progress bar
  final Color? progressBarActiveColor;

  /// Inactive color of the progress bar
  final Color? progressBarInactiveColor;

  /// Thumb color of the progress bar
  final Color? progressBarThumbColor;

  /// Thumb's glow color of the progress bar
  final Color? progressBarThumbGlowColor;

  /// TextStyle for the Progress Bar
  final TextStyle progressBarTextStyle;

  /// Active color of the volume slider
  final Color? volumeActiveColor;

  /// Inactive color of the volume slider
  final Color? volumeInactiveColor;

  /// Background color of the volume slider
  final Color volumeBackgroundColor;

  /// Thumb color of the volume slider
  final Color? volumeThumbColor;

  /// If you want the progress bar to display the time left while playing
  /// instead of the total time, set this to true
  final bool showTimeLeft;

  _NativeVideoState createState() => _NativeVideoState();
}

class _NativeVideoState extends State<NativeVideo> {
  NativeViewController? controller;
  GlobalKey<ControlState> controlKey = GlobalKey<ControlState>();

  int get playerId => widget.player.id;

  @override
  void initState() {
    super.initState();
    channel.invokeMethod(
      kPlayerCreateHWND,
      {
        'playerId': playerId,
      },
    ).then(
      (handle) {
        widget.player.setHWND(handle);
        setState(() {
          controller = NativeViewController(
            handle: handle,
            hitTestBehavior: HitTestBehavior.opaque,
          );
        });
      },
    );
    if (widget.showControls) {
      controls[playerId] = controlKey;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      alignment: Alignment.center,
      width: widget.width ?? double.infinity,
      height: widget.height ?? double.infinity,
      color: Colors.black,
      child: widget.showControls
          ? Control(
              key: controlKey,
              player: widget.player,
              progressBarThumbRadius: widget.progressBarThumbRadius,
              progressBarThumbGlowRadius: widget.progressBarThumbGlowRadius,
              progressBarActiveColor: widget.progressBarActiveColor,
              progressBarInactiveColor: widget.progressBarInactiveColor,
              progressBarThumbColor: widget.progressBarThumbColor,
              progressBarThumbGlowColor: widget.progressBarThumbGlowColor,
              volumeActiveColor: widget.volumeActiveColor,
              volumeInactiveColor: widget.volumeInactiveColor,
              volumeBackgroundColor: widget.volumeBackgroundColor,
              volumeThumbColor: widget.volumeThumbColor,
              showTimeLeft: widget.showTimeLeft,
              progressBarTextStyle: widget.progressBarTextStyle,
              child: present(),
            )
          : present(),
    );
  }

  Widget present() {
    if (controller == null) {
      return Container(
        width: widget.width ?? double.infinity,
        height: widget.height ?? double.infinity,
        color: Colors.black,
      );
    } else {
      return Container(
        width: widget.width ?? double.infinity,
        height: widget.height ?? double.infinity,
        child: NativeView(
          controller: controller!,
          width: widget.width ?? double.infinity,
          height: widget.height ?? double.infinity,
        ),
      );
    }
  }

  @override
  Future<void> dispose() async {
    controller?.dispose();
    super.dispose();
  }
}
