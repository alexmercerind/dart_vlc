import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Following typedef is used for:
/// Player::create
typedef PlayerCreateCXX = Void Function(Int32 id, Int32 videoHeight, Int32 videoWidth);
typedef PlayerCreateDart = void Function(int id, int videoHeight, int videoWidth);
/// Following typedef is used for:
/// Player::open
typedef PlayerOpenCXX = Void Function(Int32 id, Int32 autoStart, Pointer<Pointer<Utf8>> source, Int32 sourceSize);
typedef PlayerOpenDart = void Function(int id, int autoStart, Pointer<Pointer<Utf8>> source, int sourceSize);
/// Following typedef is used for:
/// Player::play
/// Player::pause
/// Player::playOrPause
/// Player::stop
/// Player::next
/// Player::back
typedef PlayerTriggerCXX = Void Function(Int32 id);
typedef PlayerTriggerDart = void Function(int id);
