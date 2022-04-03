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

#ifndef VIDEO_OUTLET_H_
#define VIDEO_OUTLET_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <mutex>

class VideoOutlet {
 public:
  VideoOutlet(flutter::TextureRegistrar* texture_registrar);

  int64_t texture_id() const { return texture_id_; }

  void MarkVideoFrameAvailable(uint8_t* buffer, int32_t width, int32_t height);

  ~VideoOutlet();

 private:
  FlutterDesktopPixelBuffer flutter_pixel_buffer_{};
  flutter::TextureRegistrar* texture_registrar_ = nullptr;
  std::unique_ptr<flutter::TextureVariant> texture_ = nullptr;
  int64_t texture_id_;
  mutable std::mutex mutex_;
};

#endif
