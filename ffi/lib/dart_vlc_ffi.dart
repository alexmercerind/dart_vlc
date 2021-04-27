import 'dart:ffi';

import 'source/typedefs.dart';
import 'source/dynamiclibrary.dart';

abstract class PlayerFFI {
  static Player_create_Dart create = dynamicLibrary.lookup<NativeFunction<Player_create_CXX>>('Player_create').asFunction();

  static Player_open_Dart open = dynamicLibrary.lookup<NativeFunction<Player_open_CXX>>('Player_open').asFunction();

  static Player_trigger_Dart play = dynamicLibrary.lookup<NativeFunction<Player_trigger_CXX>>('Player_play').asFunction();

  static Player_trigger_Dart pause = dynamicLibrary.lookup<NativeFunction<Player_trigger_CXX>>('Player_pause').asFunction();

  static Player_trigger_Dart playOrPause = dynamicLibrary.lookup<NativeFunction<Player_trigger_CXX>>('Player_playOrPause').asFunction();

  static Player_trigger_Dart stop = dynamicLibrary.lookup<NativeFunction<Player_trigger_CXX>>('Player_stop').asFunction();

  static Player_trigger_Dart next = dynamicLibrary.lookup<NativeFunction<Player_trigger_CXX>>('Player_next').asFunction();

  static Player_trigger_Dart back = dynamicLibrary.lookup<NativeFunction<Player_trigger_CXX>>('Player_back').asFunction();
}
