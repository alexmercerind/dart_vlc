import 'package:dart_vlc/src/enums/mediaSourceType.dart';

/// Parent abstract class of [Media] and [Playlist].
abstract class MediaSource {
  late MediaSourceType mediaSourceType;

  /// Internally used method to easily transform data for sending through Platform channel.
  Map<String, dynamic> toMap();
}
