/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include <vector>
#include <map>

#include <vlcpp/vlc.hpp>


class Device {
public:
	std::string name;
	std::string description;

	Device(std::string name, std::string description) {
		this->name = name;
		this->description = description;
	}

    std::map<std::string, std::string> get() {
		std::map<std::string, std::string> device;
		device["name"] = this->name;
		device["description"] = this->description;
		return device;
	}
};


class Devices {
public:
	std::vector<Device*> all;

    void refresh() {
		this->all.clear();
        VLC::Instance _ = VLC::Instance(0, nullptr);
		VLC::MediaPlayer __ = VLC::MediaPlayer(_);
		std::vector<VLC::AudioOutputDeviceDescription> devices = __.outputDeviceEnum();
		for (VLC::AudioOutputDeviceDescription device: devices) {
			this->all.emplace_back(
                new Device(
                    device.device(),
                    device.description()
                )
            );
		}
    }

    std::vector<std::map<std::string, std::string>> get() {
		this->refresh();
        std::vector<std::map<std::string, std::string>> devices;
        for (Device* device: this->all)
			devices.emplace_back(device->get());
		return devices;
    }
};
