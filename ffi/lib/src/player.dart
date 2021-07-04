import 'dart:async';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/dart_vlc.dart';
import 'package:dart_vlc_ffi/src/equalizer.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/playerState/playerState.dart';
import 'package:dart_vlc_ffi/src/mediaSource/media.dart';
import 'package:dart_vlc_ffi/src/mediaSource/mediaSource.dart';
import 'package:dart_vlc_ffi/src/device.dart';


/// A [Player] to open & play a [Media] or [Playlist] from file, network or asset.
///
/// Use [Player] constructor to create a new instance of a [Player].
/// Provide a unique [id] while instanciating.
///
/// ```dart
/// Player player = new Player(id: 0);
/// ```
///
/// If you wish to use this instance for [Video] playback, then provide [videoWidth] & [videoHeight] optional parameters.
/// Higher value may lead to degraded performance.
///
/// ```dart
/// Player player = new Player(
///   id: 0,
///   videoWidth: 1920,
///   videoHeight: 1080,
/// );
/// ```
///
/// Do not provide [videoWidth] & [videoHeight], if you wish to use the [Player] for only audio playback.
///
/// Use various methods & event streams avaiable to control & listen to events of the playback.
///
class Player {
  /// Id associated with the [Player] instance.
  final int id;

  /// Width of the [Video] frames to be extracted. Higher value may lead to degraded performance.
  late int videoWidth;

  /// Height of the [Video] frames to be extracted. Higher value may lead to degraded performance.
  late int videoHeight;

  /// Commandline arguments passed to this instance of [Player].
  List<String> commandlineArguments = <String>[];

  /// State of the current & opened [MediaSource] in [Player] instance.
  CurrentState current = new CurrentState();

  /// Stream to listen to current & opened [MediaSource] state of the [Player] instance.
  late Stream<CurrentState> currentStream;

  /// Position & duration state of the [Player] instance.
  PositionState position = new PositionState();

  /// Stream to listen to position & duration state of the [Player] instance.
  late Stream<PositionState> positionStream;

  /// Playback state of the [Player] instance.
  PlaybackState playback = new PlaybackState();

  /// Stream to listen to playback state of the [Player] instance.
  late Stream<PlaybackState> playbackStream;

  /// Volume & Rate state of the [Player] instance.
  GeneralState general = new GeneralState();

  /// Stream to listen to volume & rate state of the [Player] instance.
  late Stream<GeneralState> generalStream;

  /// Creates a new [Player] instance.
  ///
  /// Takes unique id as parameter.
  ///
  /// ```dart
  /// Player player = new Player(id: 0);
  /// ```
  ///
  Player({required this.id, int videoWidth: 0, int videoHeight: 0, List<String>? commandlineArguments}) {
    if (commandlineArguments != null) this.commandlineArguments = commandlineArguments;
    this.videoWidth = videoWidth;
    this.videoHeight = videoHeight;
    this.currentController = StreamController<CurrentState>.broadcast();
    this.currentStream = this.currentController.stream;
    this.positionController = StreamController<PositionState>.broadcast();
    this.positionStream = this.positionController.stream;
    this.playbackController = StreamController<PlaybackState>.broadcast();
    this.playbackStream = this.playbackController.stream;
    this.generalController = StreamController<GeneralState>.broadcast();
    this.generalStream = this.generalController.stream;
    PlayerFFI.create(
      this.id,
      this.videoWidth,
      this.videoHeight,
      this.commandlineArguments.length,
      this.commandlineArguments.toNativeUtf8Array(),
    );
  }

  /// Opens a new media source into the player.
  ///
  /// Takes a [Media] or [Playlist] to open in the player.
  ///
  /// Starts playback itself by default. Pass `autoStart: false` to stop this from happening.
  ///
  /// * Open a new [Media].
  ///
  /// ```dart
  /// player.open(
  ///   Media.file(
  ///     new File('C:/music.ogg'),
  ///   ),
  ///   autoStart: false,
  /// );
  /// ```
  ///
  /// * Open a new [Playlist].
  ///
  /// ```dart
  /// player.open(
  ///   new Playlist(
  ///     medias: [
  ///       Media.file(
  ///         new File('C:/music.mp3'),
  ///       ),
  ///       Media.file(
  ///         new File('C:/audio.mp3'),
  ///       ),
  ///       Media.network('https://alexmercerind.github.io/music.mp3'),
  ///     ],
  ///   ),
  /// );
  /// ```
  ///
  void open(MediaSource source, {bool autoStart: true}) {
    if (source is Media) {
      PlayerFFI.open(
        this.id,
        autoStart ? 1: 0,
        <String>[ source.mediaType.toString(), source.resource ].toNativeUtf8Array(),
        1
      );
    }
    if (source is Playlist) {
      List<String> medias = <String>[];
      source.medias.forEach((media) {
        medias.add(media.mediaType.toString());
        medias.add(media.resource);
      });
      PlayerFFI.open(
        this.id,
        autoStart ? 1: 0,
        medias.toNativeUtf8Array(),
        source.medias.length * 2
      );
    }
  }

  /// Plays opened [MediaSource],
  void play() {
    PlayerFFI.play(this.id);
  }

  /// Pauses opened [MediaSource],
  void pause() {
    PlayerFFI.pause(this.id);
  }

  /// Play or Pause opened [MediaSource],
  void playOrPause() {
    PlayerFFI.playOrPause(this.id);
  }

  /// Stops the [Player].
  ///
  /// Also resets the [Device] set using [Player.setDevice].
  /// A new instance must be created, once this method is called.
  ///
  void stop() {
    PlayerFFI.stop(this.id);
  }

  /// Jumps to the next [Media] in the [Playlist] opened.
  void next() {
    PlayerFFI.next(this.id);
  }

  /// Jumps to the previous [Media] in the [Playlist] opened.
  void back() {
    PlayerFFI.back(this.id);
  }

  /// Jumps to [Media] at specific index in the [Playlist] opened.
  /// Pass index as parameter.
  void jump(int index) {
    PlayerFFI.jump(this.id, index);
  }

  /// Seeks the [Media] currently playing in the [Player] instance, to the provided [Duration].
  void seek(Duration duration) {
    PlayerFFI.seek(this.id, duration.inMilliseconds);
  }

  /// Sets volume of the [Player] instance.
  void setVolume(double volume) {
    PlayerFFI.setVolume(this.id, volume);
  }

  /// Sets playback rate of the [Media] currently playing in the [Player] instance.
  void setRate(double rate) {
    PlayerFFI.setRate(this.id, rate);
  }


  /// Sets user agent for dart_vlc player.
  void setUserAgent(String userAgent) {
    PlayerFFI.setUserAgent(this.id, userAgent.toNativeUtf8());
  }

  /// Changes [Playlist] playback mode.
  void setPlaylistMode(PlaylistMode playlistMode) {
    PlayerFFI.setPlaylistMode(this.id, playlistMode.toString().toNativeUtf8());
  }

  /// Appends [Media] to the [Playlist] of the [Player] instance.
  void add(Media source) {
    PlayerFFI.add(
      this.id,
      source.mediaType.toString().toNativeUtf8(),
      source.resource.toString().toNativeUtf8()
    );
  }

  /// Removes [Media] from the [Playlist] at a specific index.
  void remove(int index) {
    PlayerFFI.remove(this.id, index);
  }

  /// Inserts [Media] to the [Playlist] of the [Player] instance at specific index.
  void insert(int index, Media source) {
    PlayerFFI.insert(
      this.id,
      index,
      source.mediaType.toString().toNativeUtf8(),
      source.resource.toString().toNativeUtf8()
    );
  }

  /// Moves [Media] already present in the [Playlist] of the [Player] from [initialIndex] to [finalIndex].
  void move(int initialIndex, int finalIndex) {
    PlayerFFI.move(
      this.id,
      initialIndex,
      finalIndex
    );
  }

  /// Sets playback [Device] for the instance of [Player].
  ///
  /// Use [Devices.all] getter to get [List] of all [Device].
  ///
  /// A playback [Device] for a [Player] instance cannot be changed in the middle of playback.
  /// Device will be switched once a new [Media] is played.
  ///
  Future<void> setDevice(Device device) async {
    PlayerFFI.setDevice(
      this.id,
      device.id.toNativeUtf8(),
      device.name.toNativeUtf8()
    );
  }

  /// Sets [Equalizer] for the [Player].
  void setEqualizer(Equalizer equalizer) {
    PlayerFFI.setEqualizer(this.id, equalizer.id);
  }

  /// Destroys the instance of [Player] & closes all [StreamController]s in it.
  void dispose() {
    this.currentController.close();
    this.positionController.close();
    this.playbackController.close();
    this.generalController.close();
  }

  /// Internally used [StreamController]s,
  late StreamController<CurrentState> currentController;
  late StreamController<PositionState> positionController;
  late StreamController<PlaybackState> playbackController;
  late StreamController<GeneralState> generalController;
}
