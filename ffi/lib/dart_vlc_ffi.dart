import 'dart:ffi';

import 'source/internal/typedefs/player.dart';
import 'source/internal/dynamiclibrary.dart';


abstract class PlayerFFI {

  static PlayerCreateDart create = dynamicLibrary.lookup<NativeFunction<PlayerCreateCXX>>('Player_create').asFunction();

  static PlayerOpenDart open = dynamicLibrary.lookup<NativeFunction<PlayerOpenCXX>>('Player_open').asFunction();

  static PlayerTriggerDart play = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_play').asFunction();

  static PlayerTriggerDart pause = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_pause').asFunction();

  static PlayerTriggerDart playOrPause = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_playOrPause').asFunction();

  static PlayerTriggerDart stop = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_stop').asFunction();

  static PlayerTriggerDart next = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_next').asFunction();

  static PlayerTriggerDart back = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_back').asFunction();
  
  static PlayerJumpDart jump = dynamicLibrary.lookup<NativeFunction<PlayerJumpCXX>>('Player_jump').asFunction();

  static PlayerSeekDart seek = dynamicLibrary.lookup<NativeFunction<PlayerJumpCXX>>('Player_seek').asFunction();
  
  static PlayerSetVolumeDart setVolume = dynamicLibrary.lookup<NativeFunction<PlayerSetVolumeCXX>>('Player_setVolume').asFunction();

  static PlayerSetRateDart setRate = dynamicLibrary.lookup<NativeFunction<PlayerSetRateCXX>>('Player_setRate').asFunction();

  static PlayerAddDart add = dynamicLibrary.lookup<NativeFunction<PlayerAddCXX>>('Player_add').asFunction();

  static PlayerRemoveDart remove = dynamicLibrary.lookup<NativeFunction<PlayerRemoveCXX>>('Player_remove').asFunction();

  static PlayerInsertDart insert = dynamicLibrary.lookup<NativeFunction<PlayerInsertCXX>>('Player_insert').asFunction();

  static PlayerMoveDart move = dynamicLibrary.lookup<NativeFunction<PlayerMoveCXX>>('Player_move').asFunction();
}
