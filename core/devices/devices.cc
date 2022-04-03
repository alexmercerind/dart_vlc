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

#include "devices/devices.h"

std::vector<Device> Devices::All() {
  std::vector<Device> devices{};
  VLC::Instance vlc_instance = VLC::Instance(0, nullptr);
  VLC::MediaPlayer vlc_media_player = VLC::MediaPlayer(vlc_instance);
  std::vector<VLC::AudioOutputDeviceDescription> vlc_devices =
      vlc_media_player.outputDeviceEnum();
  for (VLC::AudioOutputDeviceDescription vlc_device : vlc_devices) {
    devices.emplace_back(Device(vlc_device.device(), vlc_device.description()));
  }
  return devices;
}
