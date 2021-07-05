import 'dart:io';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/dart_vlc_ffi.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';

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
  static Record create(
      {required int id, required Media media, required File savingFile}) {
    Record record = new _Record();
    record.id = id;
    record.media = media;
    record.savingFile = savingFile;
    RecordFFI.create(
        record.id,
        savingFile.path.toNativeUtf8(),
        media.mediaType.toString().toNativeUtf8(),
        media.resource.toNativeUtf8());
    return record;
  }

  /// Starts recording the [Media].
  void start() {
    RecordFFI.start(this.id);
  }

  /// Disposes this instance of [Record].
  void dispose() {
    RecordFFI.dispose(this.id);
  }
}
