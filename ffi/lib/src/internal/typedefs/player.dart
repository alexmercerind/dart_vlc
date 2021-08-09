import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Following typedef is used for:
/// Player::create
typedef PlayerCreateCXX = Void Function(
    Int32 id,
    Int32 videoHeight,
    Int32 videoWidth,
    Int32 commandlineArgumentsCount,
    Pointer<Pointer<Utf8>> commandlineArguments);
typedef PlayerCreateDart = void Function(
    int id,
    int videoHeight,
    int videoWidth,
    int commandlineArgumentsCount,
    Pointer<Pointer<Utf8>> commandlineArguments);

/// Following typedef is used for:
/// Player::~Player
typedef PlayerDisposeCXX = Void Function(Int32 id);
typedef PlayerDisposeDart = void Function(int id);


/// Following typedef is used for:
/// Player::open
typedef PlayerOpenCXX = Void Function(
    Int32 id, Int32 autoStart, Pointer<Pointer<Utf8>> source, Int32 sourceSize);
typedef PlayerOpenDart = void Function(
    int id, int autoStart, Pointer<Pointer<Utf8>> source, int sourceSize);

/// Following typedef is used for:
/// Player::play
/// Player::pause
/// Player::playOrPause
/// Player::stop
/// Player::next
/// Player::back
typedef PlayerTriggerCXX = Void Function(Int32 id);
typedef PlayerTriggerDart = void Function(int id);

/// Following typedef is used for:
/// Player::jump
typedef PlayerJumpCXX = Void Function(Int32 id, Int32 index);
typedef PlayerJumpDart = void Function(int id, int index);

/// Following typedef is used for:
/// Player::seek
typedef PlayerSeekCXX = Void Function(Int32 id, Int32 position);
typedef PlayerSeekDart = void Function(int id, int position);

/// Following typedef is used for:
/// Player::setVolume
typedef PlayerSetVolumeCXX = Void Function(Int32 id, Float volume);
typedef PlayerSetVolumeDart = void Function(int id, double volume);

/// Following typedef is used for:
/// Player::setRate
typedef PlayerSetRateCXX = Void Function(Int32 id, Float volume);
typedef PlayerSetRateDart = void Function(int id, double volume);

/// Following typedef is used for:
/// Player::setUserAgent
typedef PlayerSetUserAgentCXX = Void Function(
    Int32 id, Pointer<Utf8> userAgent);
typedef PlayerSetUserAgentDart = void Function(int id, Pointer<Utf8> userAgent);

/// Following typedef is used for:
/// Player::setEqualizer
typedef PlayerSetEqualizerCXX = Void Function(Int32 id, Int32 equalizerId);
typedef PlayerSetEqualizerDart = void Function(int id, int equalizerId);

/// Following typedef is used for:
/// Player::setDevice
typedef PlayerSetDeviceCXX = Void Function(
    Int32 id, Pointer<Utf8> deviceId, Pointer<Utf8> deviceName);
typedef PlayerSetDeviceDart = void Function(
    int id, Pointer<Utf8> deviceId, Pointer<Utf8> deviceName);

/// Following typedef is used for:
/// Player::setPlaylistMode
typedef PlayerSetPlaylistModeCXX = Void Function(Int32 id, Pointer<Utf8> mode);
typedef PlayerSetPlaylistModeDart = void Function(int id, Pointer<Utf8> mode);

/// Following typedef is used for:
/// Player::add
typedef PlayerAddCXX = Void Function(
    Int32 id, Pointer<Utf8> type, Pointer<Utf8> resource);
typedef PlayerAddDart = void Function(
    int id, Pointer<Utf8> type, Pointer<Utf8> resource);

/// Following typedef is used for:
/// Player::remove
typedef PlayerRemoveCXX = Void Function(Int32 id, Int32 index);
typedef PlayerRemoveDart = void Function(int id, int index);

/// Following typedef is used for:
/// Player::jump
typedef PlayerInsertCXX = Void Function(
    Int32 id, Int32 index, Pointer<Utf8> type, Pointer<Utf8> resource);
typedef PlayerInsertDart = void Function(
    int id, int index, Pointer<Utf8> type, Pointer<Utf8> resource);

/// Following typedef is used for:
/// Player::move
typedef PlayerMoveCXX = Void Function(
    Int32 id, Int32 initialIndex, Int32 finalIndex);
typedef PlayerMoveDart = void Function(
    int id, int initialIndex, int finalIndex);
