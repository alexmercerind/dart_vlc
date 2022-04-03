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

#ifndef BROADCAST_BROADCAST_H_
#define BROADCAST_BROADCAST_H_

#include <sstream>
#include <string>
#include <vlcpp/vlc.hpp>

#include "broadcast/broadcast_configuration.h"
#include "media_source/media.h"

// TODO: Finalize |Broadcast| API.
// Current API is very bad & doesn't allow proper change in |Media| once the
// |Broadcast| has been started.

class Broadcast {
 public:
  BroadcastConfiguration* configuration() const { return configuration_.get(); }

  Broadcast(std::shared_ptr<Media> media,
            std::unique_ptr<BroadcastConfiguration> configuration);

  void Start();

  ~Broadcast();

 private:
  VLC::Instance vlc_instance_ = VLC::Instance(0, nullptr);
  std::shared_ptr<Media> media_ = nullptr;
  std::unique_ptr<BroadcastConfiguration> configuration_ = nullptr;
};

#endif  // BROADCAST_BROADCAST_H_
