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

#ifndef CHROMECAST_H_
#define CHROMECAST_H_

#include <sstream>
#include <string>

#include "mediasource/media.h"

class Chromecast {
 public:
  Chromecast(std::shared_ptr<Media> media, std::string ip_address)
      : media_(media), ip_address_(ip_address) {}

  void Start() {
    std::stringstream sout;
    sout << "#chromecast{ip=" << this->ip_address_
         << ", demux-filter=demux_chromecast, conversion-quality=0}";
    libvlc_vlm_add_broadcast(vlc_instance_.get(), media_->location().c_str(),
                             media_->location().c_str(), sout.str().c_str(), 0,
                             nullptr, true, false);
    libvlc_vlm_play_media(vlc_instance_.get(), media_->location().c_str());
  }

  ~Chromecast() { libvlc_vlm_release(vlc_instance_.get()); }

 private:
  std::string ip_address_;
  VLC::Instance vlc_instance_ = VLC::Instance(0, nullptr);
  std::shared_ptr<Media> media_;
};

class Chromecasts {
 public:
  Chromecast* Get(int32_t id, std::shared_ptr<Media> media,
                  std::string ip_address) {
    auto it = chromecasts_.find(id);
    if (it == chromecasts_.end()) {
      chromecasts_[id] = std::make_unique<Chromecast>(media, ip_address);
    }
    return chromecasts_[id].get();
  }

  void Dispose(int32_t id) { chromecasts_.erase(id); }

 private:
  std::map<int32_t, std::unique_ptr<Chromecast>> chromecasts_;
};

Chromecasts* chromecasts = new Chromecasts();

#endif