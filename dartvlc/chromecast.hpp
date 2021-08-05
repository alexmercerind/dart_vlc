/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
*/

#include <string>
#include <sstream>

#include "mediasource/media.hpp"

#ifndef Chromecast_HEADER
#define Chromecast_HEADER

class Chromecast {
public:
    int id;
    Media* media;
    std::string ipAddress;

    Chromecast(int id, Media* media, std::string ipAddress) {
        this->id = id;
        this->media = media;
        this->ipAddress = ipAddress;
        this->instance = VLC::Instance(0, nullptr);
    }

    void start() {
        std::stringstream sout;
        sout << "#chromecast{ip="<< this->ipAddress <<", demux-filter=demux_chromecast, conversion-quality=0}";
        libvlc_vlm_add_broadcast(
            this->instance.get(),
            this->media->location.c_str(),
            this->media->location.c_str(),
            sout.str().c_str(),
            0,
            nullptr,
            true,
            false
        );
        libvlc_vlm_play_media(
            this->instance.get(),
            this->media->location.c_str()
        );
    }

    ~Chromecast() {
        libvlc_vlm_release(this->instance.get());
        delete this->media;
    }

private:
    VLC::Instance instance;
};


class Chromecasts {
public:
	Chromecast* get(int id, Media* media, std::string ipAddress) {
		if (this->chromecasts.find(id) == this->chromecasts.end()) {
			this->chromecasts[id] = new Chromecast(id, media, ipAddress);
		}
		return this->chromecasts[id];
	}

    void dispose(int id, std::function<void()> callback = []() -> void {}) {
        delete this->chromecasts[id];
        callback();
    }

private:
	std::map<int, Chromecast*> chromecasts;
};


Chromecasts* chromecasts = new Chromecasts();

#endif