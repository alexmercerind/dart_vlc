/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
*/

#ifndef Chromecast_HEADER
#define Chromecast_HEADER

#include <string>
#include <sstream>

#include "mediasource/media.hpp"

class Chromecast {
public:
    int id;
    std::string ipAddress;

    Chromecast(int id, std::unique_ptr<Media> media, std::string ipAddress) : media_(std::move(media)) {
        id = id;
        ipAddress = ipAddress;
        instance = VLC::Instance(0, nullptr);
    }

    void start() {
        std::stringstream sout;
        sout << "#chromecast{ip="<< this->ipAddress <<", demux-filter=demux_chromecast, conversion-quality=0}";
        libvlc_vlm_add_broadcast(
            instance_.get(),
            media_->location.c_str(),
            media_->location.c_str(),
            sout.str().c_str(),
            0,
            nullptr,
            true,
            false
        );
        libvlc_vlm_play_media(
            instance_.get(),
            media_->location.c_str()
        );
    }

    ~Chromecast() {
        libvlc_vlm_release(instance_.get());
    }

private:
    VLC::Instance instance_;
    std::unique_ptr<Media> media_;
};


class Chromecasts {
public:
    // TODO: The id should be determined automatically.
    Chromecast* create(int id, std::unique_ptr<Media> media, std::string ipAddress) {
		if (chromecasts_.find(id) == chromecasts_.end()) {
			chromecasts_[id] = std::make_unique<Chromecast>(id, std::move(media), ipAddress);
		}
		return chromecasts_[id].get();
    }

	Chromecast* get(int id) {
        auto it = chromecasts_.find(id);
        if(it != chromecasts_.end()) {
            return it->second.get();
        }
        return nullptr;
	}

    void dispose(int id, std::function<void()> callback = []() -> void {}) {
        chromecasts_.erase(id);
        callback();
    }

private:
	std::map<int, std::unique_ptr<Chromecast>> chromecasts_;
};


Chromecasts* chromecasts = new Chromecasts();

#endif