import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/services.dart';
import 'package:path/path.dart' as _path;
import 'package:path_provider/path_provider.dart' as _path;
import 'package:flutter_vlc/src/audioSource/audioSource.dart';
import 'package:flutter_vlc/src/enums/audioSourceType.dart';
import 'package:flutter_vlc/src/enums/audioType.dart';


/// An audio object to open inside an [AudioPlayer].
/// 
/// * An [Audio] from a [File].
/// 
/// ```dart
/// Audio audio = Audio.file(new File('C:/music.ogg'));
/// ```
/// 
/// * An [Audio] from a [Uri].
/// 
/// ```dart
/// Audio audio = Audio.network('http://www.alexmercerind.github.io/music.mp3');
/// ```
/// 
/// * An [Audio] from assets.
/// 
/// ```dart
/// Audio audio = Audio.asset('asset/audio/music.aac');
/// ```
/// 
class Audio extends AudioSource {
  AudioSourceType audioSourceType = AudioSourceType.audio;
  AudioType audioType;
  String resource;

  /// Makes [Audio] object from a [File].
  static Audio file( File file ) {
    Audio audio = new Audio();
    audio.audioType = AudioType.file;
    audio.resource = file.path;
    return audio;
  }

  /// Makes [Audio] object from url.
  static Audio network( dynamic url ) {
    Audio audio = new Audio();
    audio.audioType = AudioType.network;
    if (url is Uri)
      audio.resource = url.toString();
    else
      audio.resource = url;
    return audio;
  }

  /// Makes [Audio] object from an asset.
  static Future<Audio> asset( String path ) async {
    Audio audio = new Audio();
    audio.audioType = AudioType.asset;
    audio.resource = path;
    ByteData audioByteData = await rootBundle.load(path);
    Uint8List audioBytes = audioByteData.buffer.asUint8List();
    File audioFile = new File(
      _path.join(
        (await _path.getTemporaryDirectory()).path,
        path,
      )
    );
    await audioFile.create(recursive: true);
    await audioFile.writeAsBytes(audioBytes);
    return audio;
  }

  /// Internally used method to easily transform data for sending through Platform channel.
  static Audio fromMap( dynamic map ) {
    Audio audio = new Audio();
    audio.audioType = {
      'AudioType.file':     AudioType.file,
      'AudioType.network':  AudioType.network,
      'AudioType.asset':    AudioType.asset,
    }[map['audioType']];
    audio.resource = map['resource'];
    return audio;
  }

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap() => {
    'audioSourceType':  this.audioSourceType.toString(),
    'audioType':        this.audioType.toString(),
    'resource':         this.resource
  };
}
