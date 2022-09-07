import 'package:dart_vlc_ffi/src/enums/media_source_type.dart';

/// Parent abstract class of [Media] and [Playlist].
abstract class MediaSource {
  MediaSourceType get mediaSourceType;
}
