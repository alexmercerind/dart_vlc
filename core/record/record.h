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

#ifndef RECORD_RECORD_H_
#define RECORD_RECORD_H_

#include <string>
#include <vlcpp/vlc.hpp>

#include "media_source/media.h"

// TODO: Finalize |Record| API.
// Current API is very bad & doesn't allow proper change in |Media| once the
// |Record| has been started.

class Record {
 public:
  Record(std::shared_ptr<Media> media, std::string saving_file);

  void Start();

  ~Record();

 private:
  std::string saving_file_;
  VLC::Instance vlc_instance_;
  std::shared_ptr<Media> media_;
};

#endif  // RECORD_RECORD_H_
