import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef EqualizerCreateEmptyCXX = Pointer<Pointer<Utf8>> Function();
typedef EqualizerCreateEmptyDart = Pointer<Pointer<Utf8>> Function();
typedef EqualizerCreateModeCXX = Pointer<Pointer<Utf8>> Function(Int32 mode);
typedef EqualizerCreateModeDart = Pointer<Pointer<Utf8>> Function(int mode);
typedef EqualizerSetBandAmpCXX = Void Function(Int32 id, Float band, Float amp);
typedef EqualizerSetBandAmpDart = void Function(
    int id, double band, double amp);
typedef EqualizerSetPreAmpCXX = Void Function(Int32 id, Float amp);
typedef EqualizerSetPreAmpDart = void Function(int id, double amp);
typedef EqualizerClearCXX = Void Function();
typedef EqualizerClearDart = void Function();
