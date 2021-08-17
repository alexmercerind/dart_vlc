import 'dart:ffi';

/// Struct received from C with equalizer data.
class EqualizerStruct extends Struct {
  @Int32()
  external int id;

  @Float()
  // ignore: non_constant_identifier_names
  external double pre_amp;

  external Pointer<Float> bands;

  external Pointer<Float> amps;

  @Int32()
  external int size;
}

typedef EqualizerCreateEmptyCXX = EqualizerStruct Function(Handle object);
typedef EqualizerCreateEmptyDart = EqualizerStruct Function(Object object);
typedef EqualizerCreateModeCXX = EqualizerStruct Function(
    Handle object, Int32 mode);
typedef EqualizerCreateModeDart = EqualizerStruct Function(
    Object object, int mode);
typedef EqualizerSetBandAmpCXX = Void Function(Int32 id, Float band, Float amp);
typedef EqualizerSetBandAmpDart = void Function(
    int id, double band, double amp);
typedef EqualizerSetPreAmpCXX = Void Function(Int32 id, Float amp);
typedef EqualizerSetPreAmpDart = void Function(int id, double amp);
