import 'dart:ffi';
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
  String toString() => 'VideoDimensions($width, $height)';
}

/// Keeps various [Player] instances to manage event callbacks.
Map<int, Player> players = {};

/// A [Player] to open & play a [Media] or [Playlist] from file, network or asset.
///
/// Use [Player] constructor to create a new instance of a [Player].
/// Provide a unique [id] while instanciating.
///
/// ```dart
/// Player player = Player(id: 0);
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
  CurrentState current = CurrentState();

  /// Stream to listen to current & opened [MediaSource] state of the [Player] instance.
  late Stream<CurrentState> currentStream;

  /// Position & duration state of the [Player] instance.
  PositionState position = PositionState();

  /// Stream to listen to position & duration state of the [Player] instance.
  late Stream<PositionState> positionStream;

  /// Playback state of the [Player] instance.
  PlaybackState playback = PlaybackState();

  /// Stream to listen to playback state of the [Player] instance.
  late Stream<PlaybackState> playbackStream;

  /// Volume & Rate state of the [Player] instance.
  GeneralState general = GeneralState();

  /// Stream to listen to volume & rate state of the [Player] instance.
  late Stream<GeneralState> generalStream;

  /// Explicit video dimensions according to which the pixel buffer will be retrieved & rendered inside the [Video] widget.
  VideoDimensions? preferredVideoDimensions;

  /// Dimensions of the currently playing video.
  VideoDimensions videoDimensions = VideoDimensions(0, 0);

  /// Stream to listen to dimensions of currently playing video.
  late Stream<VideoDimensions> videoDimensionsStream;

  /// Current buffering progress of the [Media].
  double bufferingProgress = 0.0;

  /// Stream to listen to current buffering progress of the [Media].
  late Stream<double> bufferingProgressStream;

  /// Last error received.
  String error = '';

  /// Stream to listen to error events.
  late Stream<String> errorStream;

  /// Creates a new [Player] instance.
  ///
  /// Takes unique id as parameter.
  ///
  /// ```dart
  /// Player player = Player(id: 0);
  /// ```
  ///
  Player(
      {required this.id,
      VideoDimensions? videoDimensions,
      List<String>? commandlineArguments}) {
    if (commandlineArguments != null) {
      this.commandlineArguments = commandlineArguments;
    }
    currentController = StreamController<CurrentState>.broadcast();
    currentStream = currentController.stream;
    positionController = StreamController<PositionState>.broadcast();
    positionStream = positionController.stream;
    playbackController = StreamController<PlaybackState>.broadcast();
    playbackStream = playbackController.stream;
    generalController = StreamController<GeneralState>.broadcast();
    generalStream = generalController.stream;
    bufferingProgressController = StreamController<double>.broadcast();
    bufferingProgressStream = bufferingProgressController.stream;
    errorController = StreamController<String>.broadcast();
    errorStream = errorController.stream;
    if (videoDimensions != null) {
      preferredVideoDimensions = videoDimensions;
    }
    videoDimensionsController =
        StreamController<VideoDimensions>.broadcast();
    videoDimensionsStream = videoDimensionsController.stream;
    players[id] = this;
    // Parse [commandlineArguments] & convert to `char*[]`.
    final List<Pointer<Utf8>> pointers =
        this.commandlineArguments.map<Pointer<Utf8>>((e) {
      return e.toNativeUtf8();
    }).toList();
    final Pointer<Pointer<Utf8>> arr =
        calloc.allocate(this.commandlineArguments.join().length);
    for (int i = 0; i < this.commandlineArguments.length; i++) {
      arr[i] = pointers[i];
    }
    PlayerFFI.create(
      id,
      preferredVideoDimensions?.width ?? 0,
      preferredVideoDimensions?.height ?? 0,
      this.commandlineArguments.length,
      arr,
    );
    // Freed the memory allocated for `char*[]`.
    calloc.free(arr);
    pointers.forEach(calloc.free);
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
  ///     File('C:/music.ogg'),
  ///   ),
  ///   autoStart: false,
  /// );
  /// ```
  ///
  /// * Open a new [Playlist].
  ///
  /// ```dart
  /// player.open(
  ///   Playlist(
  ///     medias: [
  ///       Media.file(
  ///         File('C:/music.mp3'),
  ///       ),
  ///       Media.file(
  ///         File('C:/audio.mp3'),
  ///       ),
  ///       Media.network('https://alexmercerind.github.io/music.mp3'),
  ///     ],
  ///   ),
  /// );
  /// ```
  ///
  void open(MediaSource source, {bool autoStart = true}) {
    if (source is Media) {
      final args = <String>[
        source.mediaType.toString(),
        source.resource,
        source.startTime.argument('start-time'),
        source.stopTime.argument('stop-time'),
      ];
      // Parse [commandlineArguments] & convert to `char*[]`.
      final List<Pointer<Utf8>> pointers = args.map<Pointer<Utf8>>((e) {
        return e.toNativeUtf8();
      }).toList();
      final Pointer<Pointer<Utf8>> arr = calloc.allocate(args.join().length);
      for (int i = 0; i < args.length; i++) {
        arr[i] = pointers[i];
      }
      PlayerFFI.open(
        id,
        autoStart ? 1 : 0,
        arr,
        1,
      );
      // Freed the memory allocated for `char*[]`.
      calloc.free(arr);
      pointers.forEach(calloc.free);
    }
    if (source is Playlist) {
      List<String> medias = <String>[];
      for (var media in source.medias) {
        medias.addAll(
          <String>[
            media.mediaType.toString(),
            media.resource,
            media.startTime.argument('start-time'),
            media.stopTime.argument('stop-time'),
          ],
        );
      }
      // Parse [commandlineArguments] & convert to `char*[]`.
      final List<Pointer<Utf8>> pointers = medias.map<Pointer<Utf8>>((e) {
        return e.toNativeUtf8();
      }).toList();
      final Pointer<Pointer<Utf8>> arr = calloc.allocate(medias.join().length);
      for (int i = 0; i < medias.length; i++) {
        arr[i] = pointers[i];
      }
      PlayerFFI.open(
        id,
        autoStart ? 1 : 0,
        arr,
        source.medias.length,
      );
      // Freed the memory allocated for `char*[]`.
      calloc.free(arr);
      pointers.forEach(calloc.free);
    }
  }

  /// Plays opened [MediaSource],
  void play() {
    PlayerFFI.play(id);
  }

  /// Pauses opened [MediaSource],
  void pause() {
    PlayerFFI.pause(id);
  }

  /// Play or Pause opened [MediaSource],
  void playOrPause() {
    PlayerFFI.playOrPause(id);
  }

  /// Stops the [Player].
  ///
  /// Also resets the [Device] set using [Player.setDevice].
  /// A new instance must be created, once this method is called.
  ///
  void stop() {
    PlayerFFI.stop(id);
  }

  /// Jumps to the next [Media] in the [Playlist] opened.
  void next() {
    PlayerFFI.next(id);
  }

  /// Jumps to the previous [Media] in the [Playlist] opened.
  void previous() {
    PlayerFFI.previous(id);
  }

  /// Jumps to [Media] at specific index in the [Playlist] opened.
  /// Pass index as parameter.
  void jumpToIndex(int index) {
    PlayerFFI.jumpToIndex(
      id,
      index,
    );
  }

  /// Seeks the [Media] currently playing in the [Player] instance, to the provided [Duration].
  void seek(Duration duration) {
    PlayerFFI.seek(
      id,
      duration.inMilliseconds,
    );
  }

  /// Sets volume of the [Player] instance.
  void setVolume(double volume) {
    PlayerFFI.setVolume(
      id,
      volume,
    );
  }

  /// Sets playback rate of the [Media] currently playing in the [Player] instance.
  void setRate(double rate) {
    PlayerFFI.setRate(
      id,
      rate,
    );
  }

  /// Sets user agent for dart_vlc player.
  void setUserAgent(String userAgent) {
    final userAgentCStr = userAgent.toNativeUtf8();
    PlayerFFI.setUserAgent(
      id,
      userAgentCStr,
    );
    calloc.free(userAgentCStr);
  }

  /// Changes [Playlist] playback mode.
  void setPlaylistMode(PlaylistMode playlistMode) {
    final playlistModeCStr = playlistMode.toString().toNativeUtf8();
    PlayerFFI.setPlaylistMode(
      id,
      playlistModeCStr,
    );
    calloc.free(playlistModeCStr);
  }

  /// Appends [Media] to the [Playlist] of the [Player] instance.
  void add(Media source) {
    final sourceMediaTypeCStr = source.mediaType.toString().toNativeUtf8();
    final sourceResourceCStr = source.resource.toString().toNativeUtf8();
    PlayerFFI.add(
      id,
      sourceMediaTypeCStr,
      sourceResourceCStr,
    );
    calloc.free(sourceMediaTypeCStr);
    calloc.free(sourceResourceCStr);
  }

  /// Removes [Media] from the [Playlist] at a specific index.
  void remove(int index) {
    PlayerFFI.remove(
      id,
      index,
    );
  }

  /// Inserts [Media] to the [Playlist] of the [Player] instance at specific index.
  void insert(int index, Media source) {
    final sourceMediaTypeCStr = source.mediaType.toString().toNativeUtf8();
    final sourceResourceCStr = source.resource.toString().toNativeUtf8();
    PlayerFFI.insert(
      id,
      index,
      sourceMediaTypeCStr,
      sourceResourceCStr,
    );
    calloc.free(sourceMediaTypeCStr);
    calloc.free(sourceResourceCStr);
  }

  /// Moves [Media] already present in the [Playlist] of the [Player] from [initialIndex] to [finalIndex].
  void move(int initialIndex, int finalIndex) {
    PlayerFFI.move(
      id,
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
    final deviceIdCStr = device.id.toNativeUtf8();
    final deviceNameCStr = device.name.toNativeUtf8();
    PlayerFFI.setDevice(
      id,
      deviceIdCStr,
      deviceNameCStr,
    );
  }

  /// Sets [Equalizer] for the [Player].
  void setEqualizer(Equalizer equalizer) {
    PlayerFFI.setEqualizer(id, equalizer.id);
  }

  /// Saves snapshot of a video to a desired [File] location.
  void takeSnapshot(File file, int width, int height) {
    final filePathCStr = file.path.toNativeUtf8();
    PlayerFFI.takeSnapshot(
      id,
      filePathCStr,
      width,
      height,
    );
    calloc.free(filePathCStr);
  }

  /// Sets Current Audio Track for the current [MediaSource]
  void setAudioTrack(int track) {
    return PlayerFFI.setAudioTrack(id, track);
  }

  /// Gets audio track count from current [MediaSource]
  int get audioTrackCount {
    int count = PlayerFFI.getAudioTrackCount(id);
    // for some reason this value returns 0 when no tracks exists
    // and 2 or more if there's 1 or more audio tracks for this [MediaSource].
    return count > 1 ? count - 1 : count;
  }

  void setHWND(int hwnd) {
    PlayerFFI.setHWND(id, hwnd);
  }

  /// Destroys the instance of [Player] & closes all [StreamController]s in it.
  void dispose() {
    currentController.close();
    positionController.close();
    playbackController.close();
    generalController.close();
    videoDimensionsController.close();
    bufferingProgressController.close();
    errorController.close();
    PlayerFFI.dispose(id);
  }

  /// Internally used [StreamController]s,
  late StreamController<CurrentState> currentController;
  late StreamController<PositionState> positionController;
  late StreamController<PlaybackState> playbackController;
  late StreamController<GeneralState> generalController;
  late StreamController<VideoDimensions> videoDimensionsController;
  late StreamController<double> bufferingProgressController;
  late StreamController<String> errorController;
}
