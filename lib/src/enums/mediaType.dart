/// Enum to specify the type of [Media].
enum MediaType {
  /// A [Media] opened from a [File].
  file,

  /// A [Media] opened from a [Uri].
  network,

  /// A [Media] opened from assets.
  asset,

  /// A [Media] created from direct show.
  directShow,
}
