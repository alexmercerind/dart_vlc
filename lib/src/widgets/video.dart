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
import 'dart:async';
import 'dart:typed_data';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';
import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/widgets/controls.dart';

/// Internally used map to keep [GlobalKey]s for [Video]'s [ControlState]s.
Map<int, GlobalKey<ControlState>> controls = {};

/// Internally used map to keep [StreamController]s for [Video] [Widget]s.
Map<int, StreamController<VideoFrame>> videoStreamControllers = {};

/// Represents a [Video] frame, used for retriving frame through platform channel.
class VideoFrame {
  VideoFrame({
    required this.playerId,
    required this.videoWidth,
    required this.videoHeight,
    required this.byteArray,
  });

  final int playerId;
  final int videoWidth;
  final int videoHeight;
  final Uint8List byteArray;
}

/// {@template video}
/// Widget for showing [Video] inside the [Widget] tree.
/// Creation of [Player] instance is necessary as a controller, for this [Widget] to show [Video] output.
///
/// An example configuration between a [Player] and a [Video] can be as follows.
/// The [Player.id] and [Video.playerId] must be same for two to work together.
///
/// ```dart
/// class _MyAppState extends State<MyApp> {
///   Player player = Player(id: 0);
///
///   @override
///   Widget build(BuildContext context) {
///     return Scaffold(
///       body: Center(
///         child: Video(
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
/// This [Widget] internally uses [StreamController].
/// Prefer calling [Player.stop] & [Video.dispose] to freed the resources.
/// A global [Key] may be used for this purpose.
/// {@endtemplate}
class Video extends StatefulWidget {
  /// {@macro video}
  Video({
    Key? key,
    @Deprecated('playerId is deprecated. Use player instead.') int? playerId,
    Player? player,
    this.width,
    this.height,
    this.fit = BoxFit.contain,
    this.alignment = Alignment.center,
    this.scale = 1.0,
    this.showControls = true,
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
    this.fillColor = Colors.black,
  })  : player = player ?? players[playerId]! as Player,
        super(key: key);

  /// The [Player] whose [Video] output should be shown.
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

  /// Fill color.
  final Color fillColor;

  @override
  VideoStateBase createState() => VideoStateTexture();
}

abstract class VideoStateBase extends State<Video>
    with AutomaticKeepAliveClientMixin {
  GlobalKey<ControlState> controlKey = GlobalKey<ControlState>();

  int get playerId => widget.player.id;

  @override
  void initState() {
    super.initState();
    if (widget.showControls) controls[playerId] = controlKey;
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    return Container(
      width: widget.width ?? double.infinity,
      height: widget.height ?? double.infinity,
      color: widget.fillColor,
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

  Widget present();
}

/// Texture based Video playback.
class VideoStateTexture extends VideoStateBase {
  StreamSubscription? _videoDimensionsSubscription;
  double? _videoWidth;
  double? _videoHeight;

  @override
  void initState() {
    super.initState();
    _videoWidth = widget.player.videoDimensions.width.toDouble();
    _videoHeight = widget.player.videoDimensions.height.toDouble();
    _videoDimensionsSubscription =
        widget.player.videoDimensionsStream.listen((dimensions) {
      if (_videoWidth != dimensions.width.toDouble() &&
          _videoHeight != dimensions.height.toDouble()) {
        setState(() {
          _videoWidth = dimensions.width.toDouble();
          _videoHeight = dimensions.height.toDouble();
        });
      }
    });
    if (mounted) setState(() {});
  }

  @override
  Widget present() {
    return ValueListenableBuilder<int?>(
      valueListenable: widget.player.textureId,
      builder: (context, textureId, _) {
        if (textureId == null || _videoWidth == null || _videoHeight == null) {
          return const SizedBox.shrink();
        }

        return SizedBox.expand(
          child: ClipRect(
            child: FittedBox(
              alignment: widget.alignment,
              fit: widget.fit,
              child: SizedBox(
                width: _videoWidth,
                height: _videoHeight,
                child: Texture(
                  textureId: textureId,
                  filterQuality: widget.filterQuality,
                ),
              ),
            ),
          ),
        );
      },
    );
  }

  @override
  Future<void> dispose() async {
    _videoDimensionsSubscription?.cancel();
    super.dispose();
  }

  @override
  bool get wantKeepAlive => true;
}

/// NativePorts & decodeImageFromPixels based video playback.
// ignore: unused_element
class VideoStateFallback extends VideoStateBase {
  Widget? videoFrameRawImage;

  Future<RawImage> getVideoFrameRawImage(VideoFrame videoFrame) async {
    Completer<ui.Image> imageCompleter = Completer<ui.Image>();
    ui.decodeImageFromPixels(
        videoFrame.byteArray,
        videoFrame.videoWidth,
        videoFrame.videoHeight,
        ui.PixelFormat.rgba8888,
        (ui.Image i) => imageCompleter.complete(i),
        rowBytes: 4 * videoFrame.videoWidth);
    ui.Image image = await imageCompleter.future;

    return RawImage(
      image: image,
      alignment: widget.alignment,
      fit: widget.fit,
      scale: widget.scale,
      filterQuality: widget.filterQuality,
    );
  }

  @override
  void initState() {
    super.initState();
    videoStreamControllers[playerId] = StreamController<VideoFrame>.broadcast();
    videoStreamControllers[playerId]
        ?.stream
        .listen((VideoFrame videoFrame) async {
      videoFrameRawImage = await getVideoFrameRawImage(videoFrame);
      if (mounted && !(videoStreamControllers[playerId]?.isClosed ?? true)) {
        setState(() {});
      }
    });
    if (mounted) setState(() {});
  }

  @override
  Future<void> dispose() async {
    videoStreamControllers[playerId]?.close();
    super.dispose();
  }

  @override
  Widget present() {
    return videoFrameRawImage != null
        ? SizedBox.expand(
            child: ClipRect(child: videoFrameRawImage),
          )
        : const SizedBox.shrink();
  }

  @override
  bool get wantKeepAlive => true;
}
