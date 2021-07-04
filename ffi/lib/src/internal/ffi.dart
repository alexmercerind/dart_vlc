import 'dart:ffi';
import 'dart:isolate';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/dynamiclibrary.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/player.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/callback.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/media.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/devices.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/equalizer.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/record.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/broadcast.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/chromecast.dart';


extension NativeTypes on List<String> {
  Pointer<Pointer<Utf8>> toNativeUtf8Array() {
    final List<Pointer<Utf8>> listPointer = this.map((String string) => string.toNativeUtf8()).toList().cast<Pointer<Utf8>>();
    final Pointer<Pointer<Utf8>> pointerPointer = calloc.allocate(this.join('').length);
    for (int index = 0; index < this.length; index++) pointerPointer[index] = listPointer[index];
    return pointerPointer;
  }
}

bool isInitialized = false;
final ReceivePort receiver = new ReceivePort();


abstract class PlayerFFI {

  static final PlayerCreateDart create = dynamicLibrary.lookup<NativeFunction<PlayerCreateCXX>>('Player_create').asFunction();

  static final PlayerOpenDart open = dynamicLibrary.lookup<NativeFunction<PlayerOpenCXX>>('Player_open').asFunction();

  static final PlayerTriggerDart play = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_play').asFunction();

  static final PlayerTriggerDart pause = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_pause').asFunction();

  static final PlayerTriggerDart playOrPause = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_playOrPause').asFunction();

  static final PlayerTriggerDart stop = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_stop').asFunction();

  static final PlayerTriggerDart next = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_next').asFunction();

  static final PlayerTriggerDart back = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_back').asFunction();
  
  static final PlayerJumpDart jump = dynamicLibrary.lookup<NativeFunction<PlayerJumpCXX>>('Player_jump').asFunction();

  static final PlayerSeekDart seek = dynamicLibrary.lookup<NativeFunction<PlayerJumpCXX>>('Player_seek').asFunction();
  
  static final PlayerSetVolumeDart setVolume = dynamicLibrary.lookup<NativeFunction<PlayerSetVolumeCXX>>('Player_setVolume').asFunction();

  static final PlayerSetRateDart setRate = dynamicLibrary.lookup<NativeFunction<PlayerSetRateCXX>>('Player_setRate').asFunction();

  static final PlayerSetUserAgentDart setUserAgent = dynamicLibrary.lookup<NativeFunction<PlayerSetUserAgentCXX>>('Player_setUserAgent').asFunction();

  static final PlayerSetEqualizerDart setEqualizer = dynamicLibrary.lookup<NativeFunction<PlayerSetEqualizerCXX>>('Player_setEqualizer').asFunction();

  static final PlayerSetDeviceDart setDevice = dynamicLibrary.lookup<NativeFunction<PlayerSetDeviceCXX>>('Player_setDevice').asFunction();

  static final PlayerSetPlaylistModeDart setPlaylistMode = dynamicLibrary.lookup<NativeFunction<PlayerSetPlaylistModeCXX>>('Player_setPlaylistMode').asFunction();
  
  static final PlayerAddDart add = dynamicLibrary.lookup<NativeFunction<PlayerAddCXX>>('Player_add').asFunction();

  static final PlayerRemoveDart remove = dynamicLibrary.lookup<NativeFunction<PlayerRemoveCXX>>('Player_remove').asFunction();

  static final PlayerInsertDart insert = dynamicLibrary.lookup<NativeFunction<PlayerInsertCXX>>('Player_insert').asFunction();

  static final PlayerMoveDart move = dynamicLibrary.lookup<NativeFunction<PlayerMoveCXX>>('Player_move').asFunction();
}


abstract class MediaFFI {

  static final MediaParseDart parse = dynamicLibrary.lookup<NativeFunction<MediaParseCXX>>('Media_parse').asFunction();

}


abstract class BroadcastFFI {

  static final BroadcastCreateDart create = dynamicLibrary.lookup<NativeFunction<BroadcastCreateCXX>>('Broadcast_create').asFunction();
  
  static final BroadcastStartDart start = dynamicLibrary.lookup<NativeFunction<BroadcastStartCXX>>('Broadcast_start').asFunction();
  
  static final BroadcastDisposeDart dispose = dynamicLibrary.lookup<NativeFunction<BroadcastDisposeCXX>>('Broadcast_dispose').asFunction();
}


abstract class ChromecastFFI {

  static final ChromecastCreateDart create = dynamicLibrary.lookup<NativeFunction<ChromecastCreateCXX>>('Chromecast_create').asFunction();
  
  static final ChromecastStartDart start = dynamicLibrary.lookup<NativeFunction<ChromecastStartCXX>>('Chromecast_start').asFunction();
  
  static final ChromecastDisposeDart dispose = dynamicLibrary.lookup<NativeFunction<ChromecastDisposeCXX>>('Chromecast_dispose').asFunction();
}


abstract class RecordFFI {

  static final RecordCreateDart create = dynamicLibrary.lookup<NativeFunction<RecordCreateCXX>>('Record_create').asFunction();
  
  static final RecordStartDart start = dynamicLibrary.lookup<NativeFunction<RecordStartCXX>>('Record_start').asFunction();
  
  static final RecordDisposeDart dispose = dynamicLibrary.lookup<NativeFunction<RecordDisposeCXX>>('Record_dispose').asFunction();
}


abstract class DevicesFFI {

  static final DevicesAllDart all = dynamicLibrary.lookup<NativeFunction<DevicesAllCXX>>('Devices_all').asFunction();
}

abstract class EqualizerFFI {

  static final EqualizerCreateEmptyDart createEmpty = dynamicLibrary.lookup<NativeFunction<EqualizerCreateEmptyCXX>>('Equalizer_createEmpty').asFunction();
  
  static final EqualizerCreateModeDart createMode = dynamicLibrary.lookup<NativeFunction<EqualizerCreateModeCXX>>('Equalizer_createMode').asFunction();
  
  static final EqualizerSetBandAmpDart setBandAmp = dynamicLibrary.lookup<NativeFunction<EqualizerSetBandAmpCXX>>('Equalizer_setBandAmp').asFunction();

  static final EqualizerSetPreAmpDart setPreAmp = dynamicLibrary.lookup<NativeFunction<EqualizerSetPreAmpCXX>>('Equalizer_setPreAmp').asFunction();
}


abstract class DartVLC {

  static void initialize(String dynamicLibraryPath) {
    if (!isInitialized) {
      dynamicLibrary = DynamicLibrary.open(dynamicLibraryPath);
      RegisterPostCObjectDart registerPostCObject = dynamicLibrary.lookup<NativeFunction<RegisterPostCObjectCXX>>('RegisterDart_PostCObject').asFunction();
      RegisterCallbackPortDart registerCallbackPort = dynamicLibrary.lookup<NativeFunction<RegisterCallbackPortCXX>>('RegisterDart_CallbackPort').asFunction();
      registerPostCObject(NativeApi.postCObject);
      registerCallbackPort(receiver.sendPort.nativePort);
      isInitialized = true;
    }
  }
}
