import 'dart:io';

import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/channel.dart';


/// Internally used class to avoid direct creation of the object of a [Record] class.
class _Record extends Record {}


class Record {
  /// ID for this record.
  late int id;

  /// Recording from [Media].
  late Media media;

  /// Path where the recording is saved example: `/home/alexmercerind/recording.mp3`
  late File savingFile;

  /// Creates a new [Record] instance.
  static Future<Record> create(
      {required int id, required Media media, required File savingFile}) async {
    Record record = new _Record();
    record.id = id;
    record.media = media;
    record.savingFile = savingFile;
    await channel.invokeMethod(
      'Record.create',
      {
        'id': id,
        'media': media.toMap(),
        'savingFile': savingFile.path,
      },
    );
    return record;
  }

  /// Starts recording the [Media].
  Future<void> start() async {
    await channel.invokeMethod(
      'Record.start',
      {
        'id': id,
      },
    );
  }

  /// Disposes this instance of [Record].
  Future<void> stop() async {
    await channel.invokeMethod(
      'Record.dispose',
      {
        'id': id,
      },
    );
  }
}
