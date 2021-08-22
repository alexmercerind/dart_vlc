import 'dart:io';

import 'package:dart_vlc_ffi/dart_vlc_ffi.dart';
import 'package:test/test.dart';

void main() {
  group('Media equality', () {
    test('media files with same url should be equal', () {
      final m1 = Media.network('https://foo.com/bar.mp4');
      final m2 = Media.network('https://foo.com/bar.mp4');

      expect(m1, equals(m2));
    });

    test('media files with different urls should not be equal', () {
      final m1 = Media.network('https://foo.com/bar.mp4');
      final m2 = Media.network('https://foo.com/baz.mp4');

      expect(m1, isNot(equals(m2)));
    });

    test('directshow media can be reconstructed from raw url', () {
      final m1 = Media.directShow(args: {'dshow-vdev': 'myDevice'});
      final m2 = Media.directShow(rawUrl: m1.resource);

      expect(m1, equals(m2));
    });
  });

  group('Playlist equality', () {
    test('playlists with same contents should be equal', () {
      final pl1 = Playlist(medias: [
        Media.network('https://foo.com/bar.mp4'),
        Media.file(File('hello.mp4'))
      ], playlistMode: PlaylistMode.repeat);
      final pl2 = Playlist(medias: [
        Media.network('https://foo.com/bar.mp4'),
        Media.file(File('hello.mp4'))
      ], playlistMode: PlaylistMode.repeat);

      expect(pl1, equals(pl2));
    });

    test('playlists with same different contents should not be equal', () {
      final pl1 = Playlist(medias: [
        Media.network('https://foo.com/bar.mp4'),
        Media.file(File('hello.mp4'))
      ], playlistMode: PlaylistMode.repeat);
      final pl2 = Playlist(medias: [
        Media.file(File('hello.mp4')),
        Media.network('https://foo.com/bar.mp4'),
      ], playlistMode: PlaylistMode.repeat);

      expect(pl1, isNot(equals(pl2)));
    });

    test('playlists with different modes should not be equal', () {
      final pl1 = Playlist(medias: [
        Media.network('https://foo.com/bar.mp4'),
      ], playlistMode: PlaylistMode.repeat);
      final pl2 = Playlist(medias: [
        Media.network('https://foo.com/bar.mp4'),
      ], playlistMode: PlaylistMode.single);

      expect(pl1, isNot(equals(pl2)));
    });
  });
}
