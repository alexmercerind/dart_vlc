import 'dart:ffi';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/enums/equalizer_mode.dart';

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
  int id = 0;

  /// Preamp value of the [Equalizer]. Use [Equalizer.setPreAmp] to change value.
  double preAmp = 0.0;

  /// Values of amps of various bands in [Equalizer]. Use [Equalizer.setBandAmp] to change values.
  Map<double, double> bandAmps = {};

  /// Preset if [Equalizer] is initialized using [Equalizer.createMode], else `null`.
  EqualizerMode? mode;

  Equalizer._();

  /// Creates a default [Equalizer] instance with all values set to `0.0`.
  factory Equalizer.createEmpty() {
    Equalizer equalizer = Equalizer._();
    final equalizerPtr = EqualizerFFI.createEmpty(
      equalizer,
    );
    equalizer.id = equalizerPtr.ref.id;
    equalizer.preAmp = equalizer.preAmp;
    equalizer.mode = null;
    equalizer.bandAmps = {};
    for (int i = 0; i < equalizerPtr.ref.size; i++) {
      equalizer.bandAmps[equalizerPtr.ref.bands[i]] = equalizerPtr.ref.amps[i];
    }
    return equalizer;
  }

  /// Creates an [Equalizer] instance with any preset from [EqualizerMode].
  factory Equalizer.createMode(EqualizerMode mode) {
    Equalizer equalizer = Equalizer._();
    final equalizerPtr = EqualizerFFI.createMode(
      equalizer,
      mode.index,
    );
    equalizer.id = equalizerPtr.ref.id;
    equalizer.preAmp = equalizer.preAmp;
    equalizer.mode = mode;
    equalizer.bandAmps = {};
    for (int i = 0; i < equalizerPtr.ref.size; i++) {
      equalizer.bandAmps[equalizerPtr.ref.bands[i]] = equalizerPtr.ref.amps[i];
    }
    return equalizer;
  }

  /// Sets value of [Equalizer.preAmp].
  ///
  /// Constraints:
  /// `-20.0 < amp < 20.0`
  ///
  void setPreAmp(double amp) {
    EqualizerFFI.setPreAmp(id, amp);
    preAmp = amp;
  }

  /// Sets value of any particular band from [Equalizer.bandAmps].
  /// Band whose `amp` needs to be changed, must be present in [Equalizer.bandAmps].
  ///
  /// Constraints:
  /// `-20.0 < amp < 20.0`
  ///
  void setBandAmp(double band, double amp) {
    EqualizerFFI.setBandAmp(id, band, amp);
    bandAmps[band] = amp;
  }
}
