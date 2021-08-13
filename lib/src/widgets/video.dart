// ignore_for_file: implementation_imports
import 'dart:async';
import 'dart:io';
import 'dart:typed_data';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';
import 'package:dart_vlc_ffi/src/player.dart' hide Player;
import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/widgets/controls.dart';

/// Internally used map to keep [GlobalKey]s for [Video]'s [ControlState]s.
Map<int, GlobalKey<ControlState>> controls = {};

/// Internally used map to keep [StreamController]s for [Video] [Widget]s.
Map<int, StreamController<VideoFrame>> videoStreamControllers = {};

/// Represents a [Video] frame, used for retriving frame through platform channel.
class VideoFrame {
  final int playerId;
  final int videoWidth;
  final int videoHeight;
  final Uint8List byteArray;

  VideoFrame({
    required this.playerId,
    required this.videoWidth,
    required this.videoHeight,
    required this.byteArray,
  });
}

/// Widget for showing [Video] inside the [Widget] tree.
/// Creation of [Player] instance is necessary as a controller, for this [Widget] to show [Video] output.
///
/// An example configuration between a [Player] and a [Video] can be as follows.
/// The [Player.id] and [Video.playerId] must be same for two to work together.
///
/// ```dart
/// class _MyAppState extends State<MyApp> {
///   Player player = new Player(id: 0);
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
///
class Video extends StatefulWidget {
  /// The [Player] whose [Video] output should be shown.
  final Player player;

  /// Width of the viewport.
  final double width;

  /// Height of the viewport.
  final double height;

  /// Scale.
  final double scale;

  /// Filter quality.
  final FilterQuality filterQuality;

  // Built-In video controls.
  final bool showControls;

  // Radius of the progressbar's thumb
  final double? progressBarThumbRadius;

  // Radius of the progressbar's glow of the thumb
  final double? progressBarThumbGlowRadius;

  // Active color of the progress bar
  final Color? progressBarActiveColor;

  // Inactive color of the progress bar
  final Color? progressBarInactiveColor;

  // Thumb color of the progress bar
  final Color? progressBarThumbColor;

  // Thumb's glow color of the progress bar
  final Color? progressBarThumbGlowColor;

  // TextStyle for the Progress Bar
  final TextStyle progressBarTextStyle;

  // Active color of the volume slider
  final Color? volumeActiveColor;

  // Inactive color of the volume slider
  final Color? volumeInactiveColor;

  // Background color of the volume slider
  final Color volumeBackgroundColor;

  // Thumb color of the volume slider
  final Color? volumeThumbColor;

  // if you want the progress bar to display the time left while playing
  // instead of the total time, set this to true
  final bool showTimeLeft;

  Video({
    @Deprecated('playerId is deprecated. Use player instead.') int? playerId,
    Player? player,
    required this.width,
    required this.height,
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
    Key? key,
  })  : player = player ?? players[playerId]! as Player,
        super(key: key);

  _VideoStateBase createState() =>
      Platform.isWindows ? _VideoStateTexture() : _VideoStateFallback();
}

abstract class _VideoStateBase extends State<Video> {
  GlobalKey<ControlState> controlKey = new GlobalKey<ControlState>();

  int get playerId => widget.player.id;

  @override
  void initState() {
    if (widget.showControls) controls[playerId] = controlKey;
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    if (widget.showControls) {
      return Control(
          key: controlKey,
          playerId: playerId,
          height: widget.height,
          width: widget.width,
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
          child: present());
    }
    return present();
  }

  Widget present();
}

class _VideoStateTexture extends _VideoStateBase {
  Widget present() {
    return ValueListenableBuilder<int?>(
        valueListenable: widget.player.textureId,
        builder: (context, textureId, _) {
          return Container(
              width: widget.width,
              height: widget.height,
              color: Colors.black,
              child: textureId != null
                  ? Texture(
                      textureId: textureId,
                      filterQuality: widget.filterQuality,
                    )
                  : null);
        });
  }
}

class _VideoStateFallback extends _VideoStateBase {
  Widget? videoFrameRawImage;

  Future<RawImage> getVideoFrameRawImage(VideoFrame videoFrame) async {
    Completer<ui.Image> imageCompleter = new Completer<ui.Image>();
    ui.decodeImageFromPixels(
      videoFrame.byteArray,
      videoFrame.videoWidth,
      videoFrame.videoHeight,
      ui.PixelFormat.rgba8888,
      (ui.Image _image) => imageCompleter.complete(_image),
      rowBytes: 4 * videoFrame.videoWidth,
      targetWidth: widget.width.toInt(),
      targetHeight: widget.height.toInt(),
    );
    ui.Image image = await imageCompleter.future;
    return RawImage(
      image: image,
      height: widget.height,
      width: widget.width,
      scale: widget.scale,
      filterQuality: widget.filterQuality,
    );
  }

  @override
  Future<void> dispose() async {
    await videoStreamControllers[playerId]?.close();
    super.dispose();
  }

  @override
  void initState() {
    videoStreamControllers[playerId] = StreamController<VideoFrame>.broadcast();
    videoStreamControllers[playerId]
        ?.stream
        .listen((VideoFrame videoFrame) async {
      videoFrameRawImage = await getVideoFrameRawImage(videoFrame);
    });

    super.initState();

    if (mounted) {
      setState(() {});
    }
  }

  Widget present() {
    return videoFrameRawImage ??
        Container(
          color: Colors.black,
          height: widget.height,
          width: widget.width,
        );
  }
}
