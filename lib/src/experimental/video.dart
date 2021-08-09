import 'package:flutter/material.dart';
import 'package:dart_vlc/dart_vlc.dart';


class Video extends StatelessWidget {
  final Player player;
  const Video({Key? key, required this.player}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      height: 360,
      width: 480,
      child: Texture(
        textureId: this.player.textureId!,
      ),
    );
  }
}
