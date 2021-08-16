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