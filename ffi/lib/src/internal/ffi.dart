import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';
import 'dart:typed_data';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/dynamiclibrary.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/player.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/media.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/devices.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/equalizer.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/record.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/broadcast.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/chromecast.dart';

/// NOTE: Here for sending event callbacks.
import 'package:dart_vlc_ffi/src/player.dart';
import 'package:dart_vlc_ffi/src/mediaSource/media.dart';

abstract class PlayerFFI {
  static final PlayerCreateDart create = dynamicLibrary
      .lookup<NativeFunction<PlayerCreateCXX>>('PlayerCreate')
      .asFunction();

  static final PlayerDisposeDart dispose = dynamicLibrary
      .lookup<NativeFunction<PlayerDisposeCXX>>('PlayerDispose')
      .asFunction();

  static final PlayerOpenDart open = dynamicLibrary
      .lookup<NativeFunction<PlayerOpenCXX>>('PlayerOpen')
      .asFunction();

  static final PlayerTriggerDart play = dynamicLibrary
      .lookup<NativeFunction<PlayerTriggerCXX>>('PlayerPlay')
      .asFunction();

  static final PlayerTriggerDart pause = dynamicLibrary
      .lookup<NativeFunction<PlayerTriggerCXX>>('PlayerPause')
      .asFunction();

  static final PlayerTriggerDart playOrPause = dynamicLibrary
      .lookup<NativeFunction<PlayerTriggerCXX>>('PlayerPlayOrPause')
      .asFunction();

  static final PlayerTriggerDart stop = dynamicLibrary
      .lookup<NativeFunction<PlayerTriggerCXX>>('PlayerStop')
      .asFunction();

  static final PlayerTriggerDart next = dynamicLibrary
      .lookup<NativeFunction<PlayerTriggerCXX>>('PlayerNext')
      .asFunction();

  static final PlayerTriggerDart back = dynamicLibrary
      .lookup<NativeFunction<PlayerTriggerCXX>>('PlayerBack')
      .asFunction();

  static final PlayerJumpDart jump = dynamicLibrary
      .lookup<NativeFunction<PlayerJumpCXX>>('PlayerJump')
      .asFunction();

  static final PlayerSeekDart seek = dynamicLibrary
      .lookup<NativeFunction<PlayerJumpCXX>>('PlayerSeek')
      .asFunction();

  static final PlayerSetVolumeDart setVolume = dynamicLibrary
      .lookup<NativeFunction<PlayerSetVolumeCXX>>('PlayerSetVolume')
      .asFunction();

  static final PlayerSetRateDart setRate = dynamicLibrary
      .lookup<NativeFunction<PlayerSetRateCXX>>('PlayerSetRate')
      .asFunction();

  static final PlayerSetUserAgentDart setUserAgent = dynamicLibrary
      .lookup<NativeFunction<PlayerSetUserAgentCXX>>('PlayerSetUserAgent')
      .asFunction();

  static final PlayerSetEqualizerDart setEqualizer = dynamicLibrary
      .lookup<NativeFunction<PlayerSetEqualizerCXX>>('PlayerSetEqualizer')
      .asFunction();

  static final PlayerSetDeviceDart setDevice = dynamicLibrary
      .lookup<NativeFunction<PlayerSetDeviceCXX>>('PlayerSetDevice')
      .asFunction();

  static final PlayerSetPlaylistModeDart setPlaylistMode = dynamicLibrary
      .lookup<NativeFunction<PlayerSetPlaylistModeCXX>>('PlayerSetPlaylistMode')
      .asFunction();

  static final PlayerAddDart add = dynamicLibrary
      .lookup<NativeFunction<PlayerAddCXX>>('PlayerAdd')
      .asFunction();

  static final PlayerRemoveDart remove = dynamicLibrary
      .lookup<NativeFunction<PlayerRemoveCXX>>('PlayerRemove')
      .asFunction();

  static final PlayerInsertDart insert = dynamicLibrary
      .lookup<NativeFunction<PlayerInsertCXX>>('PlayerInsert')
      .asFunction();

  static final PlayerMoveDart move = dynamicLibrary
      .lookup<NativeFunction<PlayerMoveCXX>>('PlayerMove')
      .asFunction();
}

abstract class MediaFFI {
  static final MediaParseDart parse = dynamicLibrary
      .lookup<NativeFunction<MediaParseCXX>>('MediaParse')
      .asFunction();
}

abstract class BroadcastFFI {
  static final BroadcastCreateDart create = dynamicLibrary
      .lookup<NativeFunction<BroadcastCreateCXX>>('BroadcastCreate')
      .asFunction();

  static final BroadcastStartDart start = dynamicLibrary
      .lookup<NativeFunction<BroadcastStartCXX>>('BroadcastStart')
      .asFunction();

  static final BroadcastDisposeDart dispose = dynamicLibrary
      .lookup<NativeFunction<BroadcastDisposeCXX>>('BroadcastDispose')
      .asFunction();
}

abstract class ChromecastFFI {
  static final ChromecastCreateDart create = dynamicLibrary
      .lookup<NativeFunction<ChromecastCreateCXX>>('ChromecastCreate')
      .asFunction();

  static final ChromecastStartDart start = dynamicLibrary
      .lookup<NativeFunction<ChromecastStartCXX>>('ChromecastStart')
      .asFunction();

  static final ChromecastDisposeDart dispose = dynamicLibrary
      .lookup<NativeFunction<ChromecastDisposeCXX>>('ChromecastDispose')
      .asFunction();
}

abstract class RecordFFI {
  static final RecordCreateDart create = dynamicLibrary
      .lookup<NativeFunction<RecordCreateCXX>>('RecordCreate')
      .asFunction();

  static final RecordStartDart start = dynamicLibrary
      .lookup<NativeFunction<RecordStartCXX>>('RecordStart')
      .asFunction();

  static final RecordDisposeDart dispose = dynamicLibrary
      .lookup<NativeFunction<RecordDisposeCXX>>('RecordDispose')
      .asFunction();
}

abstract class DevicesFFI {
  static final DevicesAllDart all = dynamicLibrary
      .lookup<NativeFunction<DevicesAllCXX>>('DevicesAll')
      .asFunction();
}

abstract class EqualizerFFI {
  static final EqualizerCreateEmptyDart createEmpty = dynamicLibrary
      .lookup<NativeFunction<EqualizerCreateEmptyCXX>>('EqualizerCreateEmpty')
      .asFunction();

  static final EqualizerCreateModeDart createMode = dynamicLibrary
      .lookup<NativeFunction<EqualizerCreateModeCXX>>('EqualizerCreateMode')
      .asFunction();

  static final EqualizerSetBandAmpDart setBandAmp = dynamicLibrary
      .lookup<NativeFunction<EqualizerSetBandAmpCXX>>('EqualizerSetBandAmp')
      .asFunction();

  static final EqualizerSetPreAmpDart setPreAmp = dynamicLibrary
      .lookup<NativeFunction<EqualizerSetPreAmpCXX>>('EqualizerSetPreAmp')
      .asFunction();
}

bool isInitialized = false;
void Function(int id, Uint8List frame) videoFrameCallback = (_, __) {};
final ReceivePort receiver = new ReceivePort()
  ..asBroadcastStream()
  ..listen((event) {
    int id = event[0];
    String type = event[1];
    switch (type) {
      case 'playbackEvent':
        {
          players[id]!.playback.isPlaying = event[2];
          players[id]!.playback.isSeekable = event[3];
          players[id]!.playback.isCompleted = false;
          if (!players[id]!.playbackController.isClosed)
            players[id]!.playbackController.add(players[id]!.playback);
          break;
        }
      case 'positionEvent':
        {
          players[id]!.position.position = Duration(milliseconds: event[3]);
          players[id]!.position.duration = Duration(milliseconds: event[4]);
          if (!players[id]!.positionController.isClosed)
            players[id]!.positionController.add(players[id]!.position);
          break;
        }
      case 'completeEvent':
        {
          players[id]!.playback.isCompleted = event[2];
          if (!players[id]!.playbackController.isClosed)
            players[id]!.playbackController.add(players[id]!.playback);
          break;
        }
      case 'volumeEvent':
        {
          players[id]!.general.volume = event[2];
          if (!players[id]!.generalController.isClosed)
            players[id]!.generalController.add(players[id]!.general);
          break;
        }
      case 'rateEvent':
        {
          players[id]!.general.rate = event[2];
          if (!players[id]!.generalController.isClosed)
            players[id]!.generalController.add(players[id]!.general);
          break;
        }
      case 'openEvent':
        {
          players[id]!.current.index = event[2];
          players[id]!.current.isPlaylist = event[3];
          List<Media> medias = [];
          for (int index = 4; index < event.length; index += 2) {
            switch (event[index]) {
              case 'MediaType.file':
                {
                  medias.add(Media.file(File(event[index + 1])));
                  break;
                }
              case 'MediaType.network':
                {
                  medias.add(Media.network(Uri.parse(event[index + 1])));
                  break;
                }
              case 'MediaType.directShow':
                {
                  medias.add(Media.directShow(rawUrl: event[index + 1]));
                  break;
                }
            }
          }
          players[id]!.current.medias = medias;
          players[id]!.current.media = medias[players[id]!.current.index!];
          if (!players[id]!.currentController.isClosed)
            players[id]!.currentController.add(players[id]!.current);
          break;
        }
      case 'videoDimensionsEvent':
        {
          players[id]!.videoDimensions = VideoDimensions(event[2], event[3]);
          if (!players[id]!.videoDimensionsController.isClosed)
            players[id]!
                .videoDimensionsController
                .add(players[id]!.videoDimensions);
          break;
        }
      case 'videoEvent':
        {
          videoFrameCallback(id, event[2]);
          break;
        }
      default:
        break;
    }
  });

extension NativeTypes on List<String> {
  Pointer<Pointer<Utf8>> toNativeUtf8Array() {
    final List<Pointer<Utf8>> listPointer = this
        .map((String string) => string.toNativeUtf8())
        .toList()
        .cast<Pointer<Utf8>>();
    final Pointer<Pointer<Utf8>> pointerPointer =
        calloc.allocate(this.join('').length);
    for (int index = 0; index < this.length; index++)
      pointerPointer[index] = listPointer[index];
    return pointerPointer;
  }
}
