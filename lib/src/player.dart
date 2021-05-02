import 'dart:async';
import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/channel.dart';
import 'package:dart_vlc/src/playerState/playerState.dart';
import 'package:dart_vlc/src/mediaSource/media.dart';
import 'package:dart_vlc/src/mediaSource/mediaSource.dart';
import 'package:dart_vlc/src/device.dart';


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
  late int id;

  /// Width of the [Video] frames to be extracted. Higher value may lead to degraded performance.
  late int videoWidth;

  /// Height of the [Video] frames to be extracted. Higher value may lead to degraded performance.
  late int videoHeight;

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
  late Stream<GeneralState> generateStream;

  /// Creates a new [Player] instance.
  ///
  /// Takes unique id as parameter.
  ///
  /// ```dart
  /// Player player = await Player(id: 0);
  /// ```
  ///
  Player({required int id, Device? device, int videoWidth: 0, int videoHeight: 0}) {
    this._isInstanceCreated = new Completer<bool>();
    if( device is Device ){
      this.setDevice( device );
    }
    this.id = id;
    this.videoWidth = videoWidth;
    this.videoHeight = videoHeight;
    this.currentController = StreamController<CurrentState>.broadcast();
    this.currentStream = this.currentController.stream;
    this.positionController = StreamController<PositionState>.broadcast();
    this.positionStream = this.positionController.stream;
    this.playbackController = StreamController<PlaybackState>.broadcast();
    this.playbackStream = this.playbackController.stream;
    this.generalController = StreamController<GeneralState>.broadcast();
    this.generateStream = this.generalController.stream;
    players[id] = this;
    this._create();
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
  Future<void> open(MediaSource source, {bool autoStart: true}) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.open',
      {
        'id': this.id,
        'autoStart': autoStart,
        'source': source.toMap(),
      },
    );
    if(!autoStart){
      await Future.delayed(Duration(milliseconds: 350), () => this.pause());
    }
  }

  /// Plays opened [MediaSource],
  Future<void> play() async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.play',
      {
        'id': this.id,
      },
    );
  }

  /// Pauses opened [MediaSource],
  Future<void> pause() async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.pause',
      {
        'id': this.id,
      },
    );
  }

  /// Play or Pause opened [MediaSource],
  Future<void> playOrPause() async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.playOrPause',
      {
        'id': this.id,
      },
    );
  }

  /// Stops the [Player].
  ///
  /// Also resets the [Device] set using [Player.setDevice].
  /// A new instance must be created, once this method is called.
  ///
  Future<void> stop() async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.stop',
      {
        'id': this.id,
      },
    );
  }

  /// Jumps to the next [Media] in the [Playlist] opened.
  Future<void> next() async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.next',
      {
        'id': this.id,
      },
    );
  }

  /// Jumps to the previous [Media] in the [Playlist] opened.
  Future<void> back() async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.back',
      {
        'id': this.id,
      },
    );
  }

  /// Jumps to [Media] at specific index in the [Playlist] opened.
  /// Pass index as parameter.
  Future<void> jump(int index) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.back',
      {
        'id': this.id,
        'index': index,
      },
    );
  }

  /// Seeks the [Media] currently playing in the [Player] instance, to the provided [Duration].
  Future<void> seek(Duration duration) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.seek',
      {
        'id': this.id,
        'duration': duration.inMilliseconds,
      },
    );
  }

  /// Sets volume of the [Player] instance.
  Future<void> setVolume(double volume) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.setVolume',
      {
        'id': this.id,
        'volume': volume,
      },
    );
  }

  /// Sets playback rate of the [Media] currently playing in the [Player] instance.
  Future<void> setRate(double rate) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.setRate',
      {
        'id': this.id,
        'rate': rate,
      },
    );
  }

  /// Changes [Playlist] playback mode.
  Future<void> setPlaylistMode(PlaylistMode playlistMode) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.setPlaylistMode',
      {
        'id': this.id,
        'playlistMode': playlistMode.toString()
      },
    );
  }

  /// Appends [Media] to the [Playlist] of the [Player] instance.
  Future<void> add(Media source) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.add',
      {
        'id': this.id,
        'source': source.toMap(),
      },
    );
  }

  /// Removes [Media] from the [Playlist] at a specific index.
  Future<void> remove(int index) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.remove',
      {
        'id': this.id,
        'index': index,
      },
    );
  }

  /// Inserts [Media] to the [Playlist] of the [Player] instance at specific index.
  Future<void> insert(int index, Media source) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.insert',
      {
        'id': this.id,
        'index': index,
        'source': source.toMap(),
      },
    );
  }

  /// Moves [Media] already present in the [Playlist] of the [Player] from [initialIndex] to [finalIndex].
  Future<void> move(int initialIndex, int finalIndex) async {
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.move',
      {'id': this.id, 'initial': initialIndex, 'final': finalIndex},
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
    await this._isInstanceCreated.future;
    await channel.invokeMethod(
      'Player.setDevice',
      {
        'id': this.id,
        'device': device.toMap(),
      },
    );
  }

  /// Destroys the instance of [Player] & closes all [StreamController]s in it.
  Future<void> dispose() async {
    await this.currentController.close();
    await this.positionController.close();
    await this.playbackController.close();
    await this.generalController.close();
  }

  /// Internally used [StreamController]s,
  late StreamController<CurrentState> currentController;
  late StreamController<PositionState> positionController;
  late StreamController<PlaybackState> playbackController;
  late StreamController<GeneralState> generalController;

  late Completer<bool> _isInstanceCreated;

  /// Creates new instance of [Player] from constructor.
  Future<void> _create() async {
    await channel.invokeMethod(
      'Player.create',
      {
        'id': id,
        'videoWidth': videoWidth,
        'videoHeight': videoHeight,
      },
    );
    this._isInstanceCreated.complete(true);
  }
}
