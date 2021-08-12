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
    BroadcastConfiguration* configuration;

    Media* media() const { return media_.get(); }

    Broadcast(int id, std::unique_ptr<Media> media, BroadcastConfiguration* configuration) : media_(std::move(media)) {
        this->id = id;
        this->configuration = configuration;
        instance_ = VLC::Instance(0, nullptr);
    }

    void start() {
        std::stringstream sout;
        sout << "#transcode{vcodec=" << configuration->vcodec << ", vb=" << configuration->vb << ", acodec=" << configuration->acodec << ", ab=" << configuration->ab << "}:std{access=" << configuration->access << ", mux=" << configuration->mux << ", dst=" << configuration->dst << "}";
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
            instance.get(),
            media_->location.c_str()
        );
    }

    ~Broadcast() {
        libvlc_vlm_release(instance_.get());
    }

private:
    VLC::Instance instance_;
    std::unique_ptr<Media> media_;
};


class Broadcasts {
public:
    // TODO: The id should be determined automatically.
    Broadcast* create(int id, std::unique_ptr<Media> media, BroadcastConfiguration* configuration) {
		if (broadcasts_.find(id) == broadcasts_.end()) {
			broadcasts_[id] = std::make_unique<Broadcast>(id, std::move(media), configuration);
		}
		return broadcasts_[id].get();
    }

	Broadcast* get(int id) const {
        auto it = broadcasts_.find(id);
        if(it != broadcasts_.end()) {
            return it->second.get();
        }
        return nullptr;
	}

    void dispose(int id, std::function<void()> callback = []() -> void {}) {
        broadcasts_.erase(id);
        callback();
    }

private:
	std::map<int, std::unique_ptr<Broadcast>> broadcasts_;
};


Broadcasts* broadcasts = new Broadcasts();


#endif
