import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Following typedef is used for:
/// Player::create
typedef Player_create_CXX = Void Function(Int32 id, Int32 videoHeight, Int32 videoWidth);
typedef Player_create_Dart = void Function(int id, int videoHeight, int videoWidth);

/// Following typedef is used for:
/// Player::open
typedef Player_open_CXX = Void Function(Int32 id, Int32 autoStart, Pointer<Pointer<Utf8>> source, Int32 sourceSize);
typedef Player_open_Dart = void Function(int id, int autoStart, Pointer<Pointer<Utf8>> source, int sourceSize);

/// Following typedef is used for:
/// Player::play
/// Player::pause
/// Player::playOrPause
/// Player::stop
/// Player::next
/// Player::back
typedef Player_trigger_CXX = Void Function(Int32 id);
typedef Player_trigger_Dart = void Function(int id);
