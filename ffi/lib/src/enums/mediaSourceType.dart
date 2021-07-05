/// Enum to specify the type of [MediaSource] passed in [Player.open].
enum MediaSourceType {
  /// A single [Media].
  media,

  /// A [Playlist] containing multiple [Media] to play sequencially.
  playlist,
}
