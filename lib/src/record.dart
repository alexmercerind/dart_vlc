
import 'package:dart_vlc/dart_vlc.dart';
import 'package:dart_vlc/src/channel.dart';

class Record {

  /// ID for this record.
  late int id;

  /// Recording from [Media].
  late Media media;

  /// Path where the recording is saved example: `C:\\audioTest.mp3`
  late String pathFile;

  Record({
    required this.id,
    required this.media,
    required this.pathFile
  });

  /// Creates a new [Record] instance.
  static Future<Record> create({ required int id, required Media media, required String pathFile }) async {
    await channel.invokeMethod(
      'Record.create',
      {
        'id': id,
        'media': media.toMap(),
        'pathFile': pathFile,
      },
    );
    return new Record(id: id, media: media, pathFile: pathFile);
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