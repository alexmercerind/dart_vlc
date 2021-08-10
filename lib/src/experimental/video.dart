import 'package:flutter/material.dart';
import 'package:dart_vlc/dart_vlc.dart';


class Video extends StatelessWidget {
  final double height;
  final double width;
  final Player player;
  final FilterQuality filterQuality;

  const Video({
    Key? key,
    required this.player,
    required this.height,
    required this.width,
    this.filterQuality: FilterQuality.low
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    if (player.textureId != null)
      return Container(
        width: this.width,
        height: this.height,
        color: Colors.black,
        child: Texture(
          textureId: this.player.textureId!,
          filterQuality: this.filterQuality,
        ),
      );
    else
      return Container(
        width: this.width,
        height: this.height,
        color: Colors.black,
      );
  }
}
