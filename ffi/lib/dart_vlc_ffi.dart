/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

import 'dart:ffi';
import 'dart:isolate';

import 'package:dart_vlc_ffi/source/internal/typedefs/player.dart';
import 'package:dart_vlc_ffi/source/internal/typedefs/callback.dart';
import 'package:dart_vlc_ffi/source/internal/dynamiclibrary.dart';


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

  static final PlayerAddDart add = dynamicLibrary.lookup<NativeFunction<PlayerAddCXX>>('Player_add').asFunction();

  static final PlayerRemoveDart remove = dynamicLibrary.lookup<NativeFunction<PlayerRemoveCXX>>('Player_remove').asFunction();

  static final PlayerInsertDart insert = dynamicLibrary.lookup<NativeFunction<PlayerInsertCXX>>('Player_insert').asFunction();

  static final PlayerMoveDart move = dynamicLibrary.lookup<NativeFunction<PlayerMoveCXX>>('Player_move').asFunction();
}


abstract class CallbackFFI {

  static void initialize() {
    RegisterPostCObjectDart registerPostCObject = dynamicLibrary.lookup<NativeFunction<RegisterPostCObjectCXX>>('RegisterDart_PostCObject').asFunction();
    RegisterCallbackPortDart registerCallbackPort = dynamicLibrary.lookup<NativeFunction<RegisterCallbackPortCXX>>('RegisterDart_CallbackPort').asFunction();
    registerPostCObject(NativeApi.postCObject);
    registerCallbackPort(receiver.sendPort.nativePort);
  }
}
