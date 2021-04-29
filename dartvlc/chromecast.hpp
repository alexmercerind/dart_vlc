/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
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

    void send() {
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

    void dispose() {
        libvlc_vlm_release(this->instance.get());
    }

private:
    VLC::Instance instance;
};


class Chromecasts {
public:
	Chromecast* get(int id, Media* media, std::string pathFile) {
		if (this->chromecasts.find(id) == this->chromecasts.end()) {
			this->chromecasts[id] = new Chromecast(id, media, pathFile);
		}
		return this->chromecasts[id];
	}

private:
	std::map<int, Chromecast*> chromecasts;
};


Chromecasts* chromecasts = new Chromecasts();

#endif