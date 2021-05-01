import 'dart:async';
import 'package:flutter/material.dart';
import 'package:audio_video_progress_bar/audio_video_progress_bar.dart';

import 'package:dart_vlc/src/device.dart';
import 'package:dart_vlc/src/channel.dart';
import 'package:dart_vlc/src/playerState/playerState.dart';

class Control extends StatefulWidget {
  final Widget child;
  final int playerId;
  final double height;
  final double width;
  final bool? showTimeLeft;
  final double? progressBarThumbRadius;
  final double? progressBarThumbGlowRadius;
  final Color? progressBarActiveColor;
  final Color? progressBarInactiveColor;
  final Color? progressBarThumbColor;
  final Color? progressBarThumbGlowColor;
  final TextStyle? progressBarTextStyle;
  final Color? volumeActiveColor;
  final Color? volumeInactiveColor;
  final Color? volumeBackgroundColor;
  final Color? volumeThumbColor;

  Control({
    required this.child,
    required this.playerId,
    required this.height,
    required this.width,
    required this.showTimeLeft,
    required this.progressBarThumbRadius,
    required this.progressBarThumbGlowRadius,
    required this.progressBarActiveColor,
    required this.progressBarInactiveColor,
    required this.progressBarThumbColor,
    required this.progressBarThumbGlowColor,
    required this.progressBarTextStyle,
    required this.volumeActiveColor,
    required this.volumeInactiveColor,
    required this.volumeBackgroundColor,
    required this.volumeThumbColor,
    Key? key,
  }) : super(key: key);

  @override
  _ControlState createState() => _ControlState();
}

class _ControlState extends State<Control> {
  bool _hideControls = true;
  bool _displayTapped = false;
  Timer? _hideTimer;

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () {
        if (players[widget.playerId]!.playback.isPlaying) {
          if (_displayTapped) {
            setState(() {
              _hideControls = true;
            });
          } else {
            _cancelAndRestartTimer();
          }
        } else {
          setState(() {
            _hideControls = true;
          });
        }
      },
      child: MouseRegion(
        onHover: (_) => _cancelAndRestartTimer(),
        child: AbsorbPointer(
          absorbing: _hideControls,
          child: Container(
            width: widget.width,
            height: widget.height,
            child: Stack(
              children: [
                widget.child,
                AnimatedOpacity(
                  duration: Duration(milliseconds: 300),
                  opacity: _hideControls ? 0.0 : 1.0,
                  child: Stack(
                    children: [
                      Container(
                        decoration: BoxDecoration(
                          gradient: LinearGradient(
                            begin: Alignment.topCenter,
                            end: Alignment.bottomCenter,
                            colors: [
                              Color(0xCC000000),
                              Color(0x00000000),
                              Color(0x00000000),
                              Color(0x00000000),
                              Color(0x00000000),
                              Color(0x00000000),
                              Color(0xCC000000),
                            ],
                          ),
                        ),
                      ),
                      Positioned(
                        left: 0,
                        right: 0,
                        bottom: 0,
                        child: Padding(
                          padding:
                              EdgeInsets.only(bottom: 60, right: 20, left: 20),
                          child: StreamBuilder<PositionState>(
                            stream: players[widget.playerId]?.positionStream,
                            builder: (BuildContext context,
                                AsyncSnapshot<PositionState> snapshot) {
                              final durationState = snapshot.data;
                              final progress =
                                  durationState?.position ?? Duration.zero;
                              final total =
                                  durationState?.duration ?? Duration.zero;
                              return Theme(
                                data: ThemeData.dark(),
                                child: ProgressBar(
                                  progress: progress,
                                  total: total,
                                  barHeight: 3,
                                  progressBarColor:
                                      widget.progressBarActiveColor,
                                  thumbColor: widget.progressBarThumbColor,
                                  baseBarColor: widget.progressBarInactiveColor,
                                  thumbGlowColor:
                                      widget.progressBarThumbGlowColor,
                                  thumbRadius:
                                      widget.progressBarThumbRadius ?? 10.0,
                                  thumbGlowRadius:
                                      widget.progressBarThumbGlowRadius ?? 30.0,
                                  timeLabelLocation: TimeLabelLocation.sides,
                                  timeLabelType: widget.showTimeLeft!
                                      ? TimeLabelType.remainingTime
                                      : TimeLabelType.totalTime,
                                  timeLabelTextStyle:
                                      widget.progressBarTextStyle,
                                  onSeek: (duration) {
                                    players[widget.playerId]!.seek(duration);
                                  },
                                ),
                              );
                            },
                          ),
                        ),
                      ),
                      Positioned(
                        left: 0,
                        right: 0,
                        bottom: 10,
                        child: Row(
                          mainAxisSize: MainAxisSize.min,
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            IconButton(
                              color: Colors.white,
                              iconSize: 30,
                              icon: Icon(Icons.skip_previous),
                              onPressed: () => players[widget.playerId]!.back(),
                            ),
                            SizedBox(width: 50),
                            IconButton(
                                color: Colors.white,
                                iconSize: 30,
                                icon: Icon(Icons.replay_10),
                                onPressed: () {
                                  int positionInMilliseconds =
                                      players[widget.playerId]!
                                              .position
                                              .position
                                              ?.inMilliseconds ??
                                          0;
                                  if (!(positionInMilliseconds - 1000)
                                      .isNegative)
                                    positionInMilliseconds -= 1000;
                                  players[widget.playerId]!
                                      .seek(Duration(
                                          milliseconds: positionInMilliseconds))
                                      .then((value) => setState(() {}));
                                }),
                            SizedBox(width: 20),
                            IconButton(
                              color: Colors.white,
                              iconSize: 30,
                              icon: Icon(
                                  players[widget.playerId]!.playback.isPlaying
                                      ? Icons.pause
                                      : Icons.play_arrow),
                              onPressed: () => players[widget.playerId]!
                                  .playOrPause()
                                  .then((value) => setState(() {})),
                            ),
                            SizedBox(width: 20),
                            IconButton(
                                color: Colors.white,
                                iconSize: 30,
                                icon: Icon(Icons.forward_10),
                                onPressed: () {
                                  int durationInMilliseconds =
                                      players[widget.playerId]!
                                              .position
                                              .duration
                                              ?.inMilliseconds ??
                                          0;
                                  int positionInMilliseconds =
                                      players[widget.playerId]!
                                              .position
                                              .position
                                              ?.inMilliseconds ??
                                          1;
                                  if ((positionInMilliseconds + 1000) <=
                                      durationInMilliseconds) {
                                    positionInMilliseconds += 1000;
                                    players[widget.playerId]!
                                        .seek(Duration(
                                            milliseconds:
                                                positionInMilliseconds))
                                        .then((value) => setState(() {}));
                                  }
                                }),
                            SizedBox(width: 50),
                            IconButton(
                              color: Colors.white,
                              iconSize: 30,
                              icon: Icon(Icons.skip_next),
                              onPressed: () => players[widget.playerId]!.next(),
                            ),
                          ],
                        ),
                      ),
                      Positioned(
                        right: 15,
                        bottom: 5,
                        child: Row(
                          crossAxisAlignment: CrossAxisAlignment.end,
                          children: [
                            VolumeControl(
                              playerId: widget.playerId,
                              thumbColor: widget.volumeThumbColor,
                              inactiveColor: widget.volumeInactiveColor,
                              activeColor: widget.volumeActiveColor,
                              backgroundColor: widget.volumeBackgroundColor,
                            ),
                            FutureBuilder(
                              future: Devices.all,
                              builder: (context,
                                  AsyncSnapshot<List<Device>> snapshot) {
                                return PopupMenuButton(
                                  iconSize: 30,
                                  icon:
                                      Icon(Icons.speaker, color: Colors.white),
                                  onSelected: (Device device) async {
                                    await players[widget.playerId]!
                                        .setDevice(device);
                                    setState(() {});
                                  },
                                  itemBuilder: (context) {
                                    return snapshot.data!
                                        .map(
                                          (device) => PopupMenuItem(
                                            child: Text(device.name,
                                                style: TextStyle(
                                                  fontSize: 14.0,
                                                )),
                                            value: device,
                                          ),
                                        )
                                        .toList();
                                  },
                                );
                              },
                            ),
                          ],
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  void _cancelAndRestartTimer() {
    _hideTimer?.cancel();
    _startHideTimer();

    setState(() {
      _hideControls = false;
      _displayTapped = true;
    });
  }

  void _startHideTimer() {
    _hideTimer = Timer(const Duration(seconds: 3), () {
      setState(() {
        _hideControls = true;
      });
    });
  }
}

class VolumeControl extends StatefulWidget {
  final int playerId;
  final Color? activeColor;
  final Color? inactiveColor;
  final Color? backgroundColor;
  final Color? thumbColor;

  const VolumeControl({
    required this.playerId,
    required this.activeColor,
    required this.inactiveColor,
    required this.backgroundColor,
    required this.thumbColor,
    Key? key,
  }) : super(key: key);

  @override
  _VolumeControlState createState() => _VolumeControlState();
}

class _VolumeControlState extends State<VolumeControl> {
  double volume = 0.5;
  bool _showVolume = false;
  double unmutedVolume = 0.5;

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        AnimatedOpacity(
          duration: Duration(milliseconds: 250),
          opacity: _showVolume ? 1 : 0,
          child: AbsorbPointer(
            absorbing: !_showVolume,
            child: MouseRegion(
              onEnter: (_) {
                setState(() => _showVolume = true);
              },
              onExit: (_) {
                setState(() => _showVolume = false);
              },
              child: Container(
                width: 60,
                height: 250,
                child: Card(
                  color: widget.backgroundColor,
                  shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(100)),
                  child: RotatedBox(
                    quarterTurns: -1,
                    child: SliderTheme(
                      data: SliderThemeData(
                        activeTrackColor: widget.activeColor,
                        inactiveTrackColor: widget.inactiveColor,
                        thumbColor: widget.thumbColor,
                      ),
                      child: Slider(
                        min: 0.0,
                        max: 1.0,
                        value: players[widget.playerId]!.general.volume,
                        onChanged: (volume) {
                          players[widget.playerId]!.setVolume(volume);
                          setState(() {});
                        },
                      ),
                    ),
                  ),
                ),
              ),
            ),
          ),
        ),
        MouseRegion(
          onEnter: (_) {
            setState(() => _showVolume = true);
          },
          onExit: (_) {
            setState(() => _showVolume = false);
          },
          child: IconButton(
            color: Colors.white,
            onPressed: () => muteUnmute(),
            icon: Icon(getIcon()),
          ),
        ),
      ],
    );
  }

  IconData getIcon() {
    if (players[widget.playerId]!.general.volume > .5)
      return Icons.volume_up_sharp;
    else if (players[widget.playerId]!.general.volume > 0)
      return Icons.volume_down_sharp;
    else
      return Icons.volume_off_sharp;
  }

  void muteUnmute() async {
    if (players[widget.playerId]!.general.volume > 0) {
      unmutedVolume = players[widget.playerId]!.general.volume;
      await players[widget.playerId]!.setVolume(0);
    } else {
      await players[widget.playerId]!.setVolume(unmutedVolume);
    }
    setState(() {});
  }
}
