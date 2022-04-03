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

#ifndef BROADCAST_BROADCAST_CONFIGURATION_H_
#define BROADCAST_BROADCAST_CONFIGURATION_H_

#include <cstdint>
#include <string>

class BroadcastConfiguration {
 public:
  BroadcastConfiguration(std::string access, std::string mux, std::string dst,
                         std::string vcodec, int32_t vb, std::string acodec,
                         int32_t ab)
      : access_(access),
        mux_(mux),
        dst_(dst),
        vcodec_(vcodec),
        acodec_(acodec),
        vb_(vb),
        ab_(ab) {}

  const std::string& access() const { return access_; }
  const std::string& mux() const { return mux_; }
  const std::string& dst() const { return dst_; }
  const std::string& vcodec() const { return vcodec_; }
  const std::string& acodec() const { return acodec_; }
  int32_t vb() const { return vb_; }
  int32_t ab() const { return ab_; }

 private:
  const std::string access_;
  const std::string mux_;
  const std::string dst_;
  const std::string vcodec_;
  const std::string acodec_;
  int32_t vb_;
  int32_t ab_;
};

#endif  // BROADCAST_BROADCAST_CONFIGURATION_H_
