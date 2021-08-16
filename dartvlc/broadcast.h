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

#ifndef BROADCAST_H_
#define BROADCAST_H_

#include <sstream>
#include <string>

#include "mediasource/media.h"

class BroadcastConfiguration {
 public:
  BroadcastConfiguration(std::string access, std::string mux, std::string dst,
                         std::string vcodec, int32_t vb, std::string acodec,
                         int32_t ab)
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
  int32_t vb() const { return vb_; }
  int32_t ab() const { return ab_; }

 private:
  const std::string access_;
  const std::string mux_;
  const std::string dst_;
  const std::string vcodec_;
  const std::string acodec_;
  int32_t vb_;
  int32_t ab_;
};

class Broadcast {
 public:
  Broadcast(std::shared_ptr<Media> media,
            std::unique_ptr<BroadcastConfiguration> configuration)
      : media_(media), configuration_(std::move(configuration)) {}

  void Start() {
    std::stringstream sout;
    sout << "#transcode{vcodec=" << configuration_->vcodec()
         << ", vb=" << configuration_->vb()
         << ", acodec=" << configuration_->acodec()
         << ", ab=" << configuration_->ab()
         << "}:std{access=" << configuration_->access()
         << ", mux=" << configuration_->mux()
         << ", dst=" << configuration_->dst();
    libvlc_vlm_add_broadcast(vlc_instance_.get(), media_->location().c_str(),
                             media_->location().c_str(), sout.str().c_str(), 0,
                             nullptr, true, false);
    libvlc_vlm_play_media(vlc_instance_.get(), media_->location().c_str());
  }

  ~Broadcast() { libvlc_vlm_release(vlc_instance_.get()); }

 private:
  VLC::Instance vlc_instance_ = VLC::Instance(0, nullptr);
  std::shared_ptr<Media> media_;
  std::unique_ptr<BroadcastConfiguration> configuration_;
};

class Broadcasts {
 public:
  Broadcast* Get(int32_t id, std::shared_ptr<Media> media,
                 std::unique_ptr<BroadcastConfiguration> configuration) {
    auto it = broadcasts_.find(id);
    if (it == broadcasts_.end()) {
      broadcasts_[id] =
          std::make_unique<Broadcast>(media, std::move(configuration));
    }
    return broadcasts_[id].get();
  }

  void Dispose(int32_t id) { broadcasts_.erase(id); }

 private:
  std::map<int32_t, std::unique_ptr<Broadcast>> broadcasts_;
};

extern std::unique_ptr<Broadcasts> g_broadcasts;

#endif
