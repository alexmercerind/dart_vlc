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

#ifndef DEVICE_H_
#define DEVICE_H_

#include <map>
#include <vector>
#include <vlcpp/vlc.hpp>

class Device {
 public:
  const std::string& id() const { return id_; }
  const std::string& name() const { return name_; }

  Device(std::string id, std::string name) : id_(id), name_(name) {}

  std::map<std::string, std::string> get() {
    std::map<std::string, std::string> device;
    device["id"] = id_;
    device["name"] = name_;
    return device;
  }

 private:
  std::string id_;
  std::string name_;
};

class Devices {
 public:
  static std::vector<Device> All() {
    std::vector<Device> devices{};
    VLC::Instance vlc_instance = VLC::Instance(0, nullptr);
    VLC::MediaPlayer vlc_media_player = VLC::MediaPlayer(vlc_instance);
    std::vector<VLC::AudioOutputDeviceDescription> vlc_devices =
        vlc_media_player.outputDeviceEnum();
    for (VLC::AudioOutputDeviceDescription vlc_device : vlc_devices) {
      devices.emplace_back(
          Device(vlc_device.device(), vlc_device.description()));
    }
    return devices;
  }

 private:
};

#endif
