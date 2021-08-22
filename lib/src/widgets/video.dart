// ignore_for_file: implementation_imports
import 'dart:async';
import 'dart:io';
import 'dart:typed_data';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';
import 'package:dart_vlc_ffi/src/player.dart' hide Player;
import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/widgets/controls.dart';
import 'package:window_size/window_size.dart' as Window;

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
///
class Video extends StatefulWidget {
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

  // For having custom controls.
  final Widget? controls;

  Video({
    @Deprecated('playerId is deprecated. Use player instead.') int? playerId,
    Player? player,
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
    this.controls,
    Key? key,
  })  : player = player ?? players[playerId]! as Player,
        super(key: key);

  VideoState createState() =>
      Platform.isWindows ? _VideoStateTexture() : _VideoStateFallback();
}

abstract class VideoState extends State<Video> {
  GlobalKey<ControlState> controlKey = GlobalKey<ControlState>();
  bool isFullscreen = false;

  int get playerId => widget.player.id;

  @override
  void initState() {
    if (widget.showControls) controls[playerId] = controlKey;
    super.initState();
  }

  void enterFullscreen() {
    if (!isFullscreen) {
      Navigator.of(context, rootNavigator: true).push(
        MaterialPageRoute(
          builder: (context) => Scaffold(
            body: Video(
              player: widget.player,
              showControls: true,
              fit: widget.fit,
              alignment: widget.alignment,
              scale: widget.scale,
              filterQuality: widget.filterQuality,
              progressBarThumbRadius: widget.progressBarThumbRadius,
              progressBarThumbGlowRadius: widget.progressBarThumbGlowRadius,
              progressBarActiveColor: widget.progressBarActiveColor,
              progressBarInactiveColor: widget.progressBarInactiveColor,
              progressBarThumbColor: widget.progressBarThumbColor,
              progressBarThumbGlowColor: widget.progressBarThumbGlowColor,
              progressBarTextStyle: widget.progressBarTextStyle,
              volumeActiveColor: widget.volumeActiveColor,
              volumeInactiveColor: widget.volumeInactiveColor,
              volumeBackgroundColor: widget.volumeBackgroundColor,
              volumeThumbColor: widget.volumeThumbColor,
              showTimeLeft: widget.showTimeLeft,
              controls: Control(
                playerId: playerId,
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
                isFullscreen: true,
                enterFullscreen: () {},
                exitFullscreen: () {
                  Navigator.of(context).pop();
                },
                child: present(),
              ),
            ),
          ),
        ),
      );
      Window.enterFullscreen();
      isFullscreen = true;
    }
  }

  void exitFullscreen() {
    if (isFullscreen) {
      Navigator.of(context).pop();
      Window.exitFullscreen();
      isFullscreen = false;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Container(
        width: widget.width ?? double.infinity,
        height: widget.height ?? double.infinity,
        color: Colors.black,
        child: widget.showControls
            ? (widget.controls ??
                Control(
                  playerId: playerId,
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
                  isFullscreen: false,
                  enterFullscreen: () {
                    Navigator.of(context, rootNavigator: true).push(
                      MaterialPageRoute(
                        builder: (context) => Scaffold(
                          body: Video(
                            player: widget.player,
                            showControls: true,
                            fit: widget.fit,
                            alignment: widget.alignment,
                            scale: widget.scale,
                            filterQuality: widget.filterQuality,
                            progressBarThumbRadius:
                                widget.progressBarThumbRadius,
                            progressBarThumbGlowRadius:
                                widget.progressBarThumbGlowRadius,
                            progressBarActiveColor:
                                widget.progressBarActiveColor,
                            progressBarInactiveColor:
                                widget.progressBarInactiveColor,
                            progressBarThumbColor: widget.progressBarThumbColor,
                            progressBarThumbGlowColor:
                                widget.progressBarThumbGlowColor,
                            progressBarTextStyle: widget.progressBarTextStyle,
                            volumeActiveColor: widget.volumeActiveColor,
                            volumeInactiveColor: widget.volumeInactiveColor,
                            volumeBackgroundColor: widget.volumeBackgroundColor,
                            volumeThumbColor: widget.volumeThumbColor,
                            showTimeLeft: widget.showTimeLeft,
                            controls: Control(
                              key: controlKey,
                              playerId: playerId,
                              progressBarThumbRadius:
                                  widget.progressBarThumbRadius,
                              progressBarThumbGlowRadius:
                                  widget.progressBarThumbGlowRadius,
                              progressBarActiveColor:
                                  widget.progressBarActiveColor,
                              progressBarInactiveColor:
                                  widget.progressBarInactiveColor,
                              progressBarThumbColor:
                                  widget.progressBarThumbColor,
                              progressBarThumbGlowColor:
                                  widget.progressBarThumbGlowColor,
                              volumeActiveColor: widget.volumeActiveColor,
                              volumeInactiveColor: widget.volumeInactiveColor,
                              volumeBackgroundColor:
                                  widget.volumeBackgroundColor,
                              volumeThumbColor: widget.volumeThumbColor,
                              showTimeLeft: widget.showTimeLeft,
                              progressBarTextStyle: widget.progressBarTextStyle,
                              isFullscreen: true,
                              enterFullscreen: () {},
                              exitFullscreen: () {
                                Navigator.of(context).pop();
                                Window.exitFullscreen();
                              },
                              child: present(),
                            ),
                          ),
                        ),
                      ),
                    );
                    Window.enterFullscreen();
                  },
                  exitFullscreen: () {},
                  child: present(),
                ))
            : present());
  }

  Widget present();
}

class _VideoStateTexture extends VideoState {
  StreamSubscription? _videoDimensionsSubscription;
  double? _videoWidth;
  double? _videoHeight;

  @override
  void initState() {
    _videoDimensionsSubscription =
        widget.player.videoDimensionsStream.listen((dimensions) {
      setState(() {
        _videoWidth = dimensions.width.toDouble();
        _videoHeight = dimensions.height.toDouble();
      });
    });
    super.initState();
  }

  Widget present() {
    return ValueListenableBuilder<int?>(
        valueListenable: widget.player.textureId,
        builder: (context, textureId, _) {
          if (textureId == null ||
              _videoWidth == null ||
              _videoHeight == null) {
            return Container();
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
                          )))));
        });
  }

  @override
  Future<void> dispose() async {
    _videoDimensionsSubscription?.cancel();
    super.dispose();
  }
}

class _VideoStateFallback extends VideoState {
  Widget? videoFrameRawImage;

  Future<RawImage> getVideoFrameRawImage(VideoFrame videoFrame) async {
    Completer<ui.Image> imageCompleter = Completer<ui.Image>();
    ui.decodeImageFromPixels(
        videoFrame.byteArray,
        videoFrame.videoWidth,
        videoFrame.videoHeight,
        ui.PixelFormat.rgba8888,
        (ui.Image _image) => imageCompleter.complete(_image),
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
      if (mounted) setState(() {});
    });
    super.initState();
  }

  Widget present() {
    return videoFrameRawImage != null
        ? SizedBox.expand(child: ClipRect(child: videoFrameRawImage))
        : Container();
  }
}
