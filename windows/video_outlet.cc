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

#include "include/dart_vlc/video_outlet.h"

VideoOutlet::VideoOutlet(flutter::TextureRegistrar* texture_registrar)
    : texture_registrar_(texture_registrar) {
  texture_ =
      std::make_unique<flutter::TextureVariant>(flutter::PixelBufferTexture(
          [=](size_t width, size_t height) -> const FlutterDesktopPixelBuffer* {
            const std::lock_guard<std::mutex> lock(mutex_);
            return &flutter_pixel_buffer_;
          }));
  texture_id_ = texture_registrar_->RegisterTexture(texture_.get());
}

void VideoOutlet::MarkVideoFrameAvailable(uint8_t* buffer, int32_t width,
                                          int32_t height) {
  const std::lock_guard<std::mutex> lock(mutex_);
  flutter_pixel_buffer_.buffer = buffer;
  flutter_pixel_buffer_.width = width;
  flutter_pixel_buffer_.height = height;
  texture_registrar_->MarkTextureFrameAvailable(texture_id_);
}

VideoOutlet::~VideoOutlet() {
  texture_registrar_->UnregisterTexture(texture_id_);
}
