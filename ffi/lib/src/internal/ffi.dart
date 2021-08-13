import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';
import 'dart:typed_data';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/src/enums/mediaType.dart';
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

abstract class CleanupFFI {
  static final MediaClearDart media = dynamicLibrary
      .lookup<NativeFunction<MediaClearCXX>>('MediaClear')
      .asFunction();

  static final DevicesClearDart devices = dynamicLibrary
      .lookup<NativeFunction<DevicesClearCXX>>('DevicesClear')
      .asFunction();

  static final EqualizerClearDart equalizer = dynamicLibrary
      .lookup<NativeFunction<EqualizerClearCXX>>('EqualizerClear')
      .asFunction();
}

bool isInitialized = false;
void Function(int playerId, Uint8List frame) videoFrameCallback = (_, __) {};
final ReceivePort receiver = new ReceivePort()
  ..asBroadcastStream()
  ..listen((event) {
    if (event[0] is String) {
      int playerId = int.parse(event[0]);
      String playerEvent = event[1];
      switch (playerEvent) {
        case 'playbackEvent':
          {
            players[playerId]!.playback.isPlaying =
                event[2] == '1' ? true : false;
            players[playerId]!.playback.isSeekable =
                event[3] == '1' ? true : false;
            players[playerId]!.playback.isCompleted = false;
            if (!players[playerId]!.playbackController.isClosed)
              players[playerId]!
                  .playbackController
                  .add(players[playerId]!.playback);
            break;
          }
        case 'positionEvent':
          {
            players[playerId]!.position.position =
                Duration(milliseconds: int.parse(event[3]));
            players[playerId]!.position.duration =
                Duration(milliseconds: int.parse(event[4]));
            if (!players[playerId]!.positionController.isClosed)
              players[playerId]!
                  .positionController
                  .add(players[playerId]!.position);
            break;
          }
        case 'openEvent':
          {
            players[playerId]!.current.index = int.parse(event[2]);
            players[playerId]!.current.isPlaylist =
                event[3] == '1' ? true : false;
            int mediasLength = event.length - 4;
            List<Media> medias = [];
            for (int index = 4; index < 4 + mediasLength; index++) {
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
                    Media media = Media();
                    media.mediaType = MediaType.directShow;
                    media.resource = event[index + 1];
                    medias.add(media);
                    break;
                  }
              }
            }
            players[playerId]!.current.medias = medias;
            players[playerId]!.current.media =
                medias[players[playerId]!.current.index!];
            if (!players[playerId]!.currentController.isClosed)
              players[playerId]!
                  .currentController
                  .add(players[playerId]!.current);
            break;
          }
        case 'completeEvent':
          {
            players[playerId]!.playback.isCompleted =
                event[2] == '1' ? true : false;
            if (!players[playerId]!.playbackController.isClosed)
              players[playerId]!
                  .playbackController
                  .add(players[playerId]!.playback);
            break;
          }
        case 'volumeEvent':
          {
            players[playerId]!.general.volume = double.parse(event[2]);
            if (!players[playerId]!.generalController.isClosed)
              players[playerId]!
                  .generalController
                  .add(players[playerId]!.general);
            break;
          }
        case 'rateEvent':
          {
            players[playerId]!.general.rate = double.parse(event[2]);
            if (!players[playerId]!.generalController.isClosed)
              players[playerId]!
                  .generalController
                  .add(players[playerId]!.general);
            break;
          }
      }
    } else {
      videoFrameCallback(event[0], event[1]);
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
