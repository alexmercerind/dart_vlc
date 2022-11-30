import 'dart:io';
import 'package:ffi/ffi.dart';
import 'package:dart_vlc_ffi/dart_vlc_ffi.dart';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';

/// [Record] class may be used to record a [Media] to a local [File] on the disk.
class Record {
  /// Unique identifier for this [Record] instance.
  final int id;

  /// Recording from [Media].
  final Media media;

  /// Path where the recording is saved example: `/home/alexmercerind/recording.mp3`
  final File savingFile;

  /// [Record] class may be used to record a [Media] to a local [File] on the disk.
  Record._(
    this.id,
    this.media,
    this.savingFile,
  );

  /// Creates a new [Record] instance.
  factory Record.create({
    required int id,
    required Media media,
    required File savingFile,
  }) {
    Record record = Record._(
      id,
      media,
      savingFile,
    );
    final savingFileCStr = savingFile.path.toNativeUtf8();
    final mediaTypeCStr = media.mediaType.toString().toNativeUtf8();
    final mediaResourceCStr = media.resource.toNativeUtf8();
    RecordFFI.create(
      record.id,
      savingFileCStr,
      mediaTypeCStr,
      mediaResourceCStr,
    );
    calloc.free(savingFileCStr);
    calloc.free(mediaTypeCStr);
    calloc.free(mediaResourceCStr);
    return record;
  }

  /// Starts recording the [Media].
  void start() {
    RecordFFI.start(id);
  }

  /// Disposes this instance of [Record].
  void dispose() {
    RecordFFI.dispose(id);
  }
}
