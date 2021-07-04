import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Following typedef is used for:
/// Equalizer::createEmpty
typedef EqualizerCreateEmptyCXX = Pointer<Pointer<Utf8>> Function();
typedef EqualizerCreateEmptyDart = Pointer<Pointer<Utf8>> Function();

/// Following typedef is used for:
/// Equalizer::createMode
typedef EqualizerCreateModeCXX = Pointer<Pointer<Utf8>> Function();
typedef EqualizerCreateModeDart = Pointer<Pointer<Utf8>> Function();

/// Following typedef is used for:
/// Equalizer::setBandAmp
typedef EqualizerSetBandAmpCXX = Void Function(Int32 id, Float band, Float amp);
typedef EqualizerSetBandAmpDart = void Function(int id, double band, double amp);

/// Following typedef is used for:
/// Equalizer::setPreAmp
typedef EqualizerSetPreAmpCXX = Void Function(Int32 id, Float amp);
typedef EqualizerSetPreAmpDart = void Function(int id, double amp);
