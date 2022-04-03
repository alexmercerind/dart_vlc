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

#ifndef MEDIA_SOURCE_MEDIA_H_
#define MEDIA_SOURCE_MEDIA_H_

#include <map>
#include <memory>

#include "media_source/media_source.h"

class Media : public MediaSource {
 public:
  static constexpr auto kMediaTypeFile = "MediaType.file";
  static constexpr auto kMediaTypeNetwork = "MediaType.network";
  static constexpr auto kMediaTypeDirectShow = "MediaType.direct_show";

  std::string& media_type() { return media_type_; };
  std::string& resource() { return resource_; };
  std::string& location() { return location_; };
  std::string& start_time() { return start_time_; };
  std::string& stop_time() { return stop_time_; };
  std::map<std::string, std::string>& metas() { return metas_; };

  static std::shared_ptr<Media> Create(std::string_view type,
                                       const std::string& url,
                                       bool parse = false,
                                       int32_t timeout = 10000);

  static std::shared_ptr<Media> File(std::string path, bool parse = false,
                                     int32_t timeout = 10000,
                                     std::string start_time = "",
                                     std::string stop_time = "");

  static std::shared_ptr<Media> Network(std::string url, bool parse = false,
                                        int32_t timeout = 10000,
                                        std::string start_time = "",
                                        std::string stop_time = "");

  static std::shared_ptr<Media> DirectShow(std::string resource);

  void Parse(int32_t timeout);

  std::string Type();

 private:
  std::string media_type_ = "";
  std::string resource_ = "";
  std::string location_ = "";
  std::string start_time_ = "";
  std::string stop_time_ = "";
  std::map<std::string, std::string> metas_ = {};
};

#endif
