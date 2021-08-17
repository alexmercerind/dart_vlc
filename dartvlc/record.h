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

#ifndef RECORD_H_
#define RECORD_H_

#include <memory>
#include <sstream>
#include <string>

#include "mediasource/media.h"

class Record {
 public:
  Record(std::shared_ptr<Media> media, std::string saving_file)
      : media_(media), saving_file_(saving_file) {
    ;
    vlc_instance_ = VLC::Instance(0, nullptr);
  }

  void Start() {
    std::stringstream sout;
    sout << "#std{access=file,mux=raw,dst=" << saving_file_ << "}";
    libvlc_vlm_add_broadcast(vlc_instance_.get(), media_->location().c_str(),
                             media_->location().c_str(), sout.str().c_str(), 0,
                             nullptr, true, false);
    libvlc_vlm_play_media(vlc_instance_.get(), media_->location().c_str());
  }

  ~Record() { libvlc_vlm_release(vlc_instance_.get()); }

 private:
  std::string saving_file_;
  VLC::Instance vlc_instance_;
  std::shared_ptr<Media> media_;
};

class Records {
 public:
  Record* Get(int id, std::shared_ptr<Media> media, std::string saving_file) {
    auto it = records_.find(id);
    if (it == records_.end()) {
      records_[id] = std::make_unique<Record>(media, saving_file);
    }
    return records_[id].get();
  }

  void Dispose(int id) { records_.erase(id); }

 private:
  std::map<int, std::unique_ptr<Record>> records_;
};

extern std::unique_ptr<Records> g_records;

#endif
