/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#ifndef Broadcast_HEADER
#define Broadcast_HEADER

#include <string>
#include <sstream>

#include "mediasource/media.hpp"

class BroadcastConfiguration {
 public:
  BroadcastConfiguration(std::string access, std::string mux, std::string dst,
                         std::string vcodec, int vb, std::string acodec, int ab)
      : access_(access),
        mux_(mux),
        dst_(dst),
        vcodec_(vcodec),
        vb_(vb),
        acodec_(acodec),
        ab_(ab) {}

  const std::string& access() const { return access_; }
  const std::string& mux() const { return mux_; }
  const std::string& dst() const { return dst_; }
  const std::string& vcodec() const { return vcodec_; }
  const std::string& acodec() const { return acodec_; }
  int vb() const { return vb_; }
  int ab() const { return ab_; }

 private:
  const std::string access_;
  const std::string mux_;
  const std::string dst_;
  const std::string vcodec_;
  const std::string acodec_;
  int vb_;
  int ab_;
};

class Broadcast {
public:
    int id;

    Media* media() const { return media_.get(); }

    Broadcast(int id, std::unique_ptr<Media> media, std::unique_ptr<BroadcastConfiguration> configuration) : media_(std::move(media)), configuration_(std::move(configuration)) {
        this->id = id;
        instance_ = VLC::Instance(0, nullptr);
    }

    void start() {
        std::stringstream sout;
        sout << "#transcode{vcodec=" << configuration_->vcodec() << ", vb=" << configuration_->vb() << ", acodec=" << configuration_->acodec() << ", ab=" << configuration_->ab() << "}:std{access=" << configuration_->access() << ", mux=" << configuration_->mux() << ", dst=" << configuration_->dst() << "}";
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
    std::unique_ptr<BroadcastConfiguration> configuration_;
};


class Broadcasts {
public:
    // TODO: The id should be determined automatically.
    Broadcast* create(int id, std::unique_ptr<Media> media, std::unique_ptr<BroadcastConfiguration> configuration) {
		if (broadcasts_.find(id) == broadcasts_.end()) {
			broadcasts_[id] = std::make_unique<Broadcast>(id, std::move(media), std::move(configuration));
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
