/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include <vector>
#include <map>

#include <vlcpp/vlc.hpp>

#ifndef Device_HEADER
#define Device_HEADER


class Device {
public:
	std::string id;
	std::string name;

	Device(std::string id, std::string name) {
		this->id = id;
		this->name = name;
	}

    std::map<std::string, std::string> get() {
		std::map<std::string, std::string> device;
		device["id"] = this->id;
		device["name"] = this->name;
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


Devices* devices = new Devices();

#endif
