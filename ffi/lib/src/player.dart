import 'dart:io';
import 'dart:async';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/dart_vlc_ffi.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';

/// Represents dimensions of a video.
class VideoDimensions {
  /// Width of the video.
  final int width;

  /// Height of the video.
  final int height;
  const VideoDimensions(this.width, this.height);

  @override
  String toString() => '($width, $height)';
}

/// Keeps various [Player] instances to manage event callbacks.
Map<int, Player> players = {};

/// A [Player] to open & play a [Media] or [Playlist] from file, network or asset.
///
/// Use [Player] constructor to create a new instance of a [Player].
/// Provide a unique [id] while instanciating.
///
/// ```dart
/// Player player = new Player(id: 0);
/// ```
///
/// Use various methods & event streams available to control & listen to events of the playback.
///
class Player {
  /// Id associated with the [Player] instance.
  int id;

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

  /// Dimensions of the currently playing video.
  VideoDimensions videoDimensions = new VideoDimensions(0, 0);

  /// Stream to listen to dimensions of currently playing video.
  late Stream<VideoDimensions> videoDimensionsStream;

  /// Current buffering progress of the [Media].
  double bufferingProgress = 0.0;

  /// Stream to listen to current buffering progress of the [Media].
  late Stream<double> bufferingProgressStream;

  /// Creates a new [Player] instance.
  ///
  /// Takes unique id as parameter.
  ///
  /// ```dart
  /// Player player = new Player(id: 0);
  /// ```
  ///
  Player(
      {required this.id,
      VideoDimensions? videoDimensions,
      List<String>? commandlineArguments}) {
    if (commandlineArguments != null)
      this.commandlineArguments = commandlineArguments;
    this.currentController = StreamController<CurrentState>.broadcast();
    this.currentStream = this.currentController.stream;
    this.positionController = StreamController<PositionState>.broadcast();
    this.positionStream = this.positionController.stream;
    this.playbackController = StreamController<PlaybackState>.broadcast();
    this.playbackStream = this.playbackController.stream;
    this.generalController = StreamController<GeneralState>.broadcast();
    this.generalStream = this.generalController.stream;
    this.bufferingProgressController = StreamController<double>.broadcast();
    this.bufferingProgressStream = this.bufferingProgressController.stream;
    if (videoDimensions != null) {
      this.videoDimensions = videoDimensions;
    }
    this.videoDimensionsController =
        StreamController<VideoDimensions>.broadcast();
    this.videoDimensionsStream = this.videoDimensionsController.stream;
    players[this.id] = this;
    PlayerFFI.create(
      this.id,
      this.videoDimensions.width,
      this.videoDimensions.height,
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
          autoStart ? 1 : 0,
          <String>[
            source.mediaType.toString(),
            source.resource,
            source.startTime.argument('start-time'),
            source.stopTime.argument('stop-time'),
          ].toNativeUtf8Array(),
          1);
    }
    if (source is Playlist) {
      List<String> medias = <String>[];
      source.medias.forEach((media) {
        medias.addAll(
          <String>[
            media.mediaType.toString(),
            media.resource,
            media.startTime.argument('start-time'),
            media.stopTime.argument('stop-time'),
          ],
        );
      });
      print(medias);
      PlayerFFI.open(
        this.id,
        autoStart ? 1 : 0,
        medias.toNativeUtf8Array(),
        source.medias.length,
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
    PlayerFFI.jump(
      this.id,
      index,
    );
  }

  /// Seeks the [Media] currently playing in the [Player] instance, to the provided [Duration].
  void seek(Duration duration) {
    PlayerFFI.seek(
      this.id,
      duration.inMilliseconds,
    );
  }

  /// Sets volume of the [Player] instance.
  void setVolume(double volume) {
    PlayerFFI.setVolume(
      this.id,
      volume,
    );
  }

  /// Sets playback rate of the [Media] currently playing in the [Player] instance.
  void setRate(double rate) {
    PlayerFFI.setRate(
      this.id,
      rate,
    );
  }

  /// Sets user agent for dart_vlc player.
  void setUserAgent(String userAgent) {
    PlayerFFI.setUserAgent(
      this.id,
      userAgent.toNativeUtf8(),
    );
  }

  /// Changes [Playlist] playback mode.
  void setPlaylistMode(PlaylistMode playlistMode) {
    PlayerFFI.setPlaylistMode(
      this.id,
      playlistMode.toString().toNativeUtf8(),
    );
  }

  /// Appends [Media] to the [Playlist] of the [Player] instance.
  void add(Media source) {
    PlayerFFI.add(
      this.id,
      source.mediaType.toString().toNativeUtf8(),
      source.resource.toString().toNativeUtf8(),
    );
  }

  /// Removes [Media] from the [Playlist] at a specific index.
  void remove(int index) {
    PlayerFFI.remove(
      this.id,
      index,
    );
  }

  /// Inserts [Media] to the [Playlist] of the [Player] instance at specific index.
  void insert(int index, Media source) {
    PlayerFFI.insert(
      this.id,
      index,
      source.mediaType.toString().toNativeUtf8(),
      source.resource.toString().toNativeUtf8(),
    );
  }

  /// Moves [Media] already present in the [Playlist] of the [Player] from [initialIndex] to [finalIndex].
  void move(int initialIndex, int finalIndex) {
    PlayerFFI.move(
      this.id,
      initialIndex,
      finalIndex,
    );
  }

  /// Sets playback [Device] for the instance of [Player].
  ///
  /// Use [Devices.all] getter to get [List] of all [Device].
  ///
  /// A playback [Device] for a [Player] instance cannot be changed in the middle of playback.
  /// Device will be switched once a new [Media] is played.
  ///
  void setDevice(Device device) {
    PlayerFFI.setDevice(
      this.id,
      device.id.toNativeUtf8(),
      device.name.toNativeUtf8(),
    );
  }

  /// Sets [Equalizer] for the [Player].
  void setEqualizer(Equalizer equalizer) {
    PlayerFFI.setEqualizer(this.id, equalizer.id);
  }

  /// Saves snapshot of a video to a desired [File] location.
  void takeSnapshot(File file, int width, int height) {
    PlayerFFI.takeSnapshot(
      this.id,
      file.path.toNativeUtf8(),
      width,
      height,
    );
  }

  /// Destroys the instance of [Player] & closes all [StreamController]s in it.
  void dispose() {
    this.currentController.close();
    this.positionController.close();
    this.playbackController.close();
    this.generalController.close();
    this.videoDimensionsController.close();
    this.bufferingProgressController.close();
    PlayerFFI.dispose(this.id);
  }

  /// Internally used [StreamController]s,
  late StreamController<CurrentState> currentController;
  late StreamController<PositionState> positionController;
  late StreamController<PlaybackState> playbackController;
  late StreamController<GeneralState> generalController;
  late StreamController<VideoDimensions> videoDimensionsController;
  late StreamController<double> bufferingProgressController;
}
