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
