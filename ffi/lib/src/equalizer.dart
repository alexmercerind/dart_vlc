import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/enums/equalizerMode.dart';


/// Internally used class to avoid direct creation of the object of a [Equalizer] class.
class _Equalizer extends Equalizer {}


/// Defines an [Equalizer] instance for usage in a [Player].
/// 
/// Use [Equalizer.createEmpty] for creating a default equalizer & [Equalizer.createMode] for creating an equalizer using a preset from [EqualizerMode].
/// 
/// Example
/// 
/// ```dart
/// Player player = new Player(id: 0);
/// Equalizer equalizer = Equalizer.createMode(EqualizerMode.party);
/// player.setEqualizer(equalizer);
/// ```
/// 
class Equalizer {
  /// Unique Id associated with this [Equalizer].
  late int id;
  /// Preamp value of the [Equalizer]. Use [Equalizer.setPreAmp] to change value.
  double preAmp = 0.0;
  /// Values of amps of various bands in [Equalizer]. Use [Equalizer.setBandAmp] to change values.
  Map<double, double> bandAmps = {};
  /// Preset if [Equalizer] is initialized using [Equalizer.createMode], else `null`.
  EqualizerMode? mode;

  /// Creates a default [Equalizer] instance with all values set to `0.0`.
  static Equalizer createEmpty() {
    Equalizer equalizer = new _Equalizer();
    Pointer<Pointer<Utf8>> _equalizer = EqualizerFFI.createEmpty();
    equalizer.id = int.parse(_equalizer.elementAt(0).value.toDartString());
    equalizer.preAmp = double.parse(_equalizer.elementAt(1).value.toDartString());
    equalizer.mode = null;
    equalizer.bandAmps = {
      double.parse(_equalizer.elementAt(2).value.toDartString()): double.parse(_equalizer.elementAt(3).value.toDartString()),
      double.parse(_equalizer.elementAt(4).value.toDartString()): double.parse(_equalizer.elementAt(5).value.toDartString()),
      double.parse(_equalizer.elementAt(6).value.toDartString()): double.parse(_equalizer.elementAt(7).value.toDartString()),
      double.parse(_equalizer.elementAt(8).value.toDartString()): double.parse(_equalizer.elementAt(9).value.toDartString()),
      double.parse(_equalizer.elementAt(10).value.toDartString()): double.parse(_equalizer.elementAt(11).value.toDartString()),
    };
    return equalizer;
  }

  /// Creates an [Equalizer] instance with any preset from [EqualizerMode].
  static Equalizer createMode(EqualizerMode mode) {
    Equalizer equalizer = new _Equalizer();
    Pointer<Pointer<Utf8>> _equalizer = EqualizerFFI.createMode(mode.index);
    equalizer.id = int.parse(_equalizer.elementAt(0).value.toDartString());
    equalizer.preAmp = double.parse(_equalizer.elementAt(1).value.toDartString());
    equalizer.mode = mode;
    equalizer.bandAmps = {
      double.parse(_equalizer.elementAt(2).value.toDartString()): double.parse(_equalizer.elementAt(3).value.toDartString()),
      double.parse(_equalizer.elementAt(4).value.toDartString()): double.parse(_equalizer.elementAt(5).value.toDartString()),
      double.parse(_equalizer.elementAt(6).value.toDartString()): double.parse(_equalizer.elementAt(7).value.toDartString()),
      double.parse(_equalizer.elementAt(8).value.toDartString()): double.parse(_equalizer.elementAt(9).value.toDartString()),
      double.parse(_equalizer.elementAt(10).value.toDartString()): double.parse(_equalizer.elementAt(11).value.toDartString()),
    };
    return equalizer;
  }

  /// Sets value of [Equalizer.preAmp].
  /// 
  /// Constraints:
  /// `-20.0 < amp < 20.0`
  /// 
  void setPreAmp(double amp) {
    EqualizerFFI.setPreAmp(this.id, amp);
    this.preAmp = amp;
  }

  /// Sets value of any particular band from [Equalizer.bandAmps].
  /// Band whose `amp` needs to be changed, must be present in [Equalizer.bandAmps].
  /// 
  /// Constraints:
  /// `-20.0 < amp < 20.0`
  /// 
  void setBandAmp(double band, double amp) {
    EqualizerFFI.setBandAmp(this.id, band, amp);
    this.bandAmps[band] = amp;
  }
}
