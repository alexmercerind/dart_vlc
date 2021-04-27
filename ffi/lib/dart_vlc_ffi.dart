import 'dart:ffi';

import 'source/typedefs.dart';
import 'source/dynamiclibrary.dart';

abstract class PlayerFFI {
  static PlayerCreateDart create = dynamicLibrary.lookup<NativeFunction<PlayerCreateCXX>>('Player_create').asFunction();

  static PlayerOpenDart open = dynamicLibrary.lookup<NativeFunction<PlayerOpenCXX>>('Player_open').asFunction();

  static PlayerTriggerDart play = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_play').asFunction();

  static PlayerTriggerDart pause = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_pause').asFunction();

  static PlayerTriggerDart playOrPause = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_playOrPause').asFunction();

  static PlayerTriggerDart stop = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_stop').asFunction();

  static PlayerTriggerDart next = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_next').asFunction();

  static PlayerTriggerDart back = dynamicLibrary.lookup<NativeFunction<PlayerTriggerCXX>>('Player_back').asFunction();
}
