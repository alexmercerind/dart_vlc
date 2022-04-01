// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
