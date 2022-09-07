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

#include "chromecast/chromecast.h"

#include <sstream>
#include <string>

// TODO: Finalize |Chromecast| API.
// Current API is very bad & doesn't allow proper change in |Media| once the
// |Chromecast| has been started.

Chromecast::Chromecast(std::shared_ptr<Media> media, std::string ip_address)
    : media_(media), ip_address_(ip_address) {}

void Chromecast::Start() {
  std::stringstream sout;
  sout << "#chromecast{ip=" << this->ip_address_
       << ", demux-filter=demux_chromecast, conversion-quality=0}";
  libvlc_vlm_add_broadcast(vlc_instance_.get(), media_->location().c_str(),
                           media_->location().c_str(), sout.str().c_str(), 0,
                           nullptr, true, false);
  libvlc_vlm_play_media(vlc_instance_.get(), media_->location().c_str());
}

Chromecast::~Chromecast() { libvlc_vlm_release(vlc_instance_.get()); }
