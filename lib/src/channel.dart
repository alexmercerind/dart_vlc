import 'package:flutter/services.dart';
import 'package:dart_vlc/src/player.dart';
import 'package:dart_vlc/src/widgets/video.dart';
import 'package:dart_vlc/src/mediaSource/media.dart';

/// Internally used map to keep [Player] instances & manage event streams.
Map<int, Player> players = {};

/// Internally used map to keep [Media] instances & manage [Media.metas].
Map<int, Map<String, String>> mediaMetas = {};

/// Internally used map to keep [Media] instances & manage [Media.extras].
Map<int, Map<String, dynamic>> mediaExtras = {};

/// Platform channel for invoking methods & handling calls.
final MethodChannel channel = new MethodChannel('dart_vlc')
  ..setMethodCallHandler((MethodCall methodCall) async {
    switch (methodCall.method) {
      case 'playerState':
        {
          int id = methodCall.arguments['id'];
          switch (methodCall.arguments['type']) {
            case 'openEvent':
              {
                players[id]!.current.index = methodCall.arguments['index'];
                players[id]!.current.medias = methodCall.arguments['medias']
                    .map(
                      (media) => Media.fromMap(media),
                    )
                    .toList()
                    .cast<Media>();
                players[id]!.current.media =
                    players[id]!.current.medias[players[id]!.current.index!];
                players[id]!.current.isPlaylist =
                    methodCall.arguments['isPlaylist'];
                players[id]!.currentController.add(players[id]!.current);
                players[id]!.playback.isCompleted = false;
                players[id]!.playbackController.add(players[id]!.playback);
                break;
              }
            case 'positionEvent':
              {
                players[id]!.position.position = Duration(
                    milliseconds: methodCall.arguments['position'] ?? 0);
                players[id]!.position.duration = Duration(
                    milliseconds: methodCall.arguments['duration'] ?? 0);
                players[id]!.positionController.add(players[id]!.position);
                break;
              }
            case 'playbackEvent':
              {
                players[id]!.playback.isPlaying =
                    methodCall.arguments['isPlaying'];
                players[id]!.playback.isSeekable =
                    methodCall.arguments['isSeekable'];
                players[id]!.playbackController.add(players[id]!.playback);
                break;
              }
            case 'completeEvent':
              {
                players[id]!.playback.isCompleted =
                    methodCall.arguments['isCompleted'];
                players[id]!.playbackController.add(players[id]!.playback);
                break;
              }
            case 'volumeEvent':
              {
                players[id]!.general.volume = methodCall.arguments['volume'];
                players[id]!.generalController.add(players[id]!.general);
                break;
              }
            case 'rateEvent':
              {
                players[id]!.general.rate = methodCall.arguments['rate'];
                players[id]!.generalController.add(players[id]!.general);
                break;
              }
            case 'exceptionEvent':
              {
                /* TODO: Deal with exception case. */
                break;
              }
            default:
              break;
          }
          break;
        }
      case 'videoFrame':
        {
          int playerId = methodCall.arguments['id'];
          if (videoStreamControllers.containsKey(playerId)) {
            VideoFrame frame = new VideoFrame(
              playerId: playerId,
              videoWidth: methodCall.arguments['videoWidth'],
              videoHeight: methodCall.arguments['videoHeight'],
              byteArray: methodCall.arguments['byteArray'],
            );
            videoStreamControllers[playerId]!.add(frame);
          }
          break;
        }
      default:
        break;
    }
  });
