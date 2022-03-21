import 'package:dart_vlc_ffi/src/enums/mediaSourceType.dart';

/// Parent abstract class of [Media] and [Playlist].
abstract class MediaSource {
  MediaSourceType get mediaSourceType;
}
