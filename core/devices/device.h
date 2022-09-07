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

#ifndef DEVICES_DEVICE_H_
#define DEVICES_DEVICE_H_

#include <map>
#include <vector>
#include <vlcpp/vlc.hpp>

class Device {
 public:
  const std::string& id() const { return id_; }
  const std::string& name() const { return name_; }

  Device(std::string id, std::string name) : id_(id), name_(name) {}

 private:
  std::string id_;
  std::string name_;
};

#endif
