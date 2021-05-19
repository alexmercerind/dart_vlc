import 'package:dart_vlc/src/channel.dart';
import 'package:dart_vlc/src/enums/equalizerMode.dart';


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
  static Future<Equalizer> createEmpty() async {
    Equalizer equalizer = new _Equalizer();
    dynamic _equalizer = await channel.invokeMethod(
      'Equalizer.createEmpty',
       {},
    );
    equalizer.id = _equalizer['id'];
    equalizer.preAmp = _equalizer['preAmp'];
    equalizer.bandAmps = Map<double, double>.from(_equalizer['bandAmps']);
    return equalizer;
  }

  /// Creates an [Equalizer] instance with any preset from [EqualizerMode].
  static Future<Equalizer> createMode(EqualizerMode mode) async {
    Equalizer equalizer = new _Equalizer();
    dynamic _equalizer = await channel.invokeMethod(
      'Equalizer.createMode',
       {
         'mode': mode.index,
       },
    );
    equalizer.id = _equalizer['id'];
    equalizer.mode = mode;
    equalizer.preAmp = _equalizer['preAmp'];
    equalizer.bandAmps = Map<double, double>.from(_equalizer['bandAmps']);
    return equalizer;
  }

  /// Sets value of [Equalizer.preAmp].
  /// 
  /// Constraints:
  /// `-20.0 < amp < 20.0`
  /// 
  Future<void> setPreAmp(double amp) async {
    await channel.invokeMethod(
      'Equalizer.setPreAmp',
       {
         'id': this.id,
         'preAmp': amp,
       },
    );
    this.preAmp = amp;
  }

  /// Sets value of any particular band from [Equalizer.bandAmps].
  /// Band whose `amp` needs to be changed, must be present in [Equalizer.bandAmps].
  /// 
  /// Constraints:
  /// `-20.0 < amp < 20.0`
  /// 
  Future<void> setBandAmp(double band, double amp) async {
    await channel.invokeMethod(
      'Equalizer.setBandAmp',
       {
         'id': this.id,
         'band': band,
         'amp': amp,
       },
    );
    this.bandAmps[band] = amp;
  }
}
