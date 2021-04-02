import 'dart:async';
import 'dart:typed_data';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';


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
///   Player player;
///   bool init = true;  
/// 
///   @override
///   Future<void> didChangeDependencies() async {
///     super.didChangeDependencies();
///     if (this.init) {
///       this.player = await Player.create(id: 0);
///     }
///     this.init = false;
///   }
///   
///   @override
///   Widget build(BuildContext context) {
///     return Scaffold(
///       body: Center(
///         child: Video(
///           playerId: 0,
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
  /// Id of the [Player] whose [Video] output should be shown.
  final int playerId;
  /// Width of the viewport.
  final double width;
  /// Height of the viewport.
  final double height;
  /// Scale.
  final double scale;

  /// TODO: Add in-built [Video] controls & other visual parameters.
  final bool showControls;

  Video({
    required this.playerId,
    required this.width,
    required this.height,
    this.scale: 1.0,
    this.showControls: true,
    Key? key,
  }) : super(key: key);

  VideoState createState() => VideoState();
}


class VideoState extends State<Video> {
  Widget? videoFrameRawImage;

  Future<RawImage> getVideoFrameRawImage(VideoFrame videoFrame) async {
    Completer<ui.Image> imageCompleter  = new Completer<ui.Image>();
    ui.decodeImageFromPixels(
      videoFrame.byteArray,
      videoFrame.videoWidth,
      videoFrame.videoHeight,
      ui.PixelFormat.bgra8888,
      (ui.Image _image) => imageCompleter.complete(_image),
      rowBytes: 4 * videoFrame.videoWidth,
      targetWidth: videoFrame.videoWidth,
      targetHeight: videoFrame.videoHeight,
    );
    ui.Image image = await imageCompleter.future;
    return new RawImage(
      image: image,
      height: widget.height,
      width: widget.width,
      scale: widget.scale,
      filterQuality: FilterQuality.high,
    );
  }

  @override
  Future<void> dispose() async {
    super.dispose();
    await videoStreamControllers[widget.playerId]?.close();
  }

  @override
  void initState() {
    super.initState();
    videoStreamControllers[widget.playerId] = new StreamController<VideoFrame>.broadcast();
    videoStreamControllers[widget.playerId]?.stream.listen(
      (VideoFrame videoFrame) async {
        this.videoFrameRawImage = await this.getVideoFrameRawImage(videoFrame);
        this.setState(() {});
      }
    );
  }

  @override
  Widget build(BuildContext context) {
    return this.videoFrameRawImage ?? Container(
      color: Colors.black,
      height: widget.height,
      width: widget.width,
    );
  }
}
