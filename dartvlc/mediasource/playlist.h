/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

#ifndef MEDIASOURCE_PLAYLIST_H_
#define MEDIASOURCE_PLAYLIST_H_

#include <map>
#include <string>
#include <vector>

#include "mediasource/media.h"
#include "mediasource/mediasource.h"

enum PlaylistMode { single, loop, repeat };

class Playlist : public MediaSource {
 public:
  std::vector<std::shared_ptr<Media>>& medias() { return medias_; }

  PlaylistMode& playlist_mode() { return playlist_mode_; }

  Playlist(std::vector<std::shared_ptr<Media>> medias,
           PlaylistMode playlist_mode = PlaylistMode::single)
      : medias_(medias), playlist_mode_(playlist_mode){};

  std::string Type() { return "MediaSourceType.playlist"; }

 protected:
  std::vector<std::shared_ptr<Media>> medias_;
  PlaylistMode playlist_mode_;
};

#endif
