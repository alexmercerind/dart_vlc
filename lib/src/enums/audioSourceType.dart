/// Enum to specify the type of [AudioSource] passed in [AudioPlayer.open].
enum AudioSourceType {
  /// A single [Audio].
  audio,
  /// A [Playlist] containing multiple [Audio] to play sequencially.
  playlist,
}
