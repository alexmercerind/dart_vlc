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

#ifndef Broadcast_HEADER
#define Broadcast_HEADER


class BroadcastConfiguration {
public:
    std::string access;
    std::string mux;
    std::string dst;
    std::string vcodec;
    int vb;
    std::string acodec;
    int ab;

    BroadcastConfiguration(std::string access, std::string mux, std::string dst, std::string vcodec, int vb, std::string acodec, int ab) {
        this->access = access;
        this->mux = mux;
        this->dst = dst;
        this->vcodec = vcodec;
        this->vb = vb;
        this->acodec = acodec;
        this->ab = ab;
    }
};


class Broadcast {
public:
    int id;
    Media* media;
    BroadcastConfiguration* configuration;

    Broadcast(int id, Media* media, BroadcastConfiguration* configuration) {
        this->id = id;
        this->media = media;
        this->configuration = configuration;
        this->instance = VLC::Instance(0, nullptr);
    }

    void start() {
        std::stringstream sout;
        sout << "#transcode{vcodec=" << configuration->vcodec << ", vb=" << configuration->vb << ", acodec=" << configuration->acodec << ", ab=" << configuration->ab << "}:std{access=" << configuration->access << ", mux=" << configuration->mux << ", dst=" << configuration->dst << "}";
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


class Broadcasts {
public:
	Broadcast* get(int id, Media* media, BroadcastConfiguration* configuration) {
		if (this->broadcasts.find(id) == this->broadcasts.end()) {
			this->broadcasts[id] = new Broadcast(id, media, configuration);
		}
		return this->broadcasts[id];
	}

private:
	std::map<int, Broadcast*> broadcasts;
};


Broadcasts* broadcasts = new Broadcasts();


#endif
