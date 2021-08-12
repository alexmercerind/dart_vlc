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

#include "events.hpp"
#include "../device.hpp"
#include "../mediasource/mediasource.hpp"
#include "../mediasource/media.hpp"
#include "../mediasource/playlist.hpp"

class PlayerSetters : public PlayerEvents {
 public:
  void Open(std::shared_ptr<MediaSource> media_source, bool autoStart = true) {
    state_->is_started_ = false;
    state_->Reset();
    Stop();
    state_->medias_->medias_ = {};
    vlc_media_list_ = VLC::MediaList(vlc_instance_);
    if (media_source->Type() == "MediaSourceType.media") {
      std::shared_ptr<Media> media =
          std::dynamic_pointer_cast<Media>(media_source);
      VLC::Media vlc_media =
          VLC::Media(vlc_instance_, media->location_, VLC::Media::FromLocation);
      vlc_media_list_.addMedia(vlc_media);
      vlc_media_list_player_.setMediaList(vlc_media_list_);
      state_->medias_->medias_ = {media};
      state_->is_playlist_ = false;
    } else if (media_source->Type() == "MediaSourceType.playlist") {
      std::shared_ptr<Playlist> playlist =
          std::dynamic_pointer_cast<Playlist>(media_source);
      if (playlist->medias_.empty()) return;
      for (std::shared_ptr<Media> media : playlist->medias_) {
        VLC::Media vlc_media = VLC::Media(vlc_instance_, media->location_,
                                          VLC::Media::FromLocation);
        vlc_media_list_.addMedia(vlc_media);
        state_->medias_->medias_.emplace_back(media);
      }
      vlc_media_list_player_.setMediaList(vlc_media_list_);
      state_->is_playlist_ = true;
    }
    OnOpenCallback(vlc_media_list_.itemAtIndex(0));
    if (autoStart) Play();
  }

  void Play() {
    if (!state_->is_started_ && !state_->medias_->medias_.empty()) {
      vlc_media_list_player_.playItemAtIndex(0);
      state_->is_started_ = true;
    } else {
      vlc_media_list_player_.play();
    }
  }

  void Pause() {
    if (vlc_media_list_player_.isPlaying()) {
      vlc_media_list_player_.pause();
    }
  }

  void PlayOrPause() {
    if (!state_->is_started_ && !state_->medias_->medias_.empty()) {
      vlc_media_list_player_.playItemAtIndex(0);
      state_->is_started_ = true;
    } else {
      vlc_media_list_player_.pause();
    }
  }

  void Stop() { vlc_media_list_player_.stop(); }

  void Next() {
    OnPlaylistCallback();
    if (state_->index_ < vlc_media_list_.count())
      vlc_media_list_player_.playItemAtIndex(++state_->index_);
  }

  void Back() {
    OnPlaylistCallback();
    if (state_->index_ > 0)
      vlc_media_list_player_.playItemAtIndex(--state_->index_);
  }

  void Jump(int32_t index) {
    OnPlaylistCallback();
    if (index >= 0 && index < vlc_media_list_.count())
      vlc_media_list_player_.playItemAtIndex(index);
  }

  void Seek(int32_t position) {
    float relativePosition =
        static_cast<float>(position) / static_cast<float>(Duration());
    vlc_media_player_.setPosition(relativePosition);
  }

  void SetVolume(float volume) {
    vlc_media_player_.setVolume(static_cast<int32_t>(volume * 100));
    state_->volume_ = volume;
    volume_callback_(volume);
  }

  void SetRate(float rate) {
    vlc_media_player_.setRate(rate);
    state_->rate_ = rate;
    rate_callback_(rate);
  }

  void SetDevice(std::shared_ptr<Device> device) {
    if (device->id != "")
      state_->device_ = device;
    else
      state_->device_ = nullptr;
    vlc_media_player_.outputDeviceSet(device->id);
  }

  void SetPlaylistMode(PlaylistMode mode) {
    vlc_media_list_player_.setPlaybackMode(
        static_cast<libvlc_playback_mode_t>(mode));
  }

  void SetEqualizer(Equalizer* equalizer) {
    vlc_media_player_.setEqualizer(equalizer->equalizer);
  }

  void SetUserAgent(std::string userAgent) {
    vlc_instance_.setUserAgent("dart_vlc", userAgent);
  }

  void Add(std::unique_ptr<Media> media) {
    is_playlist_modified_ = true;
    VLC::Media _media =
        VLC::Media(vlc_instance_, media->location_, VLC::Media::FromLocation);
    vlc_media_list_.addMedia(_media);
    state_->medias_->medias_.emplace_back(std::move(media));
    OnPlaylistCallback();
    state_->is_playlist_ = true;
  }

  void Remove(int32_t index) {
    if (index < 0 || index >= state_->medias_->medias_.size()) return;
    is_playlist_modified_ = true;
    state_->medias_->medias_.erase(state_->medias_->medias_.begin() + index);
    vlc_media_list_.removeIndex(index);
    OnPlaylistCallback();
    if (!state_->is_completed_ && state_->index_ == index) {
      if (state_->index_ == vlc_media_list_.count()) {
        vlc_media_list_player_.stop();
      } else
        Jump(index);
    }
    if (state_->index_ > index) state_->index_--;
    state_->is_playlist_ = true;
  }

  void Insert(int32_t index, std::unique_ptr<Media> media) {
    if (index < 0 || index >= state_->medias_->medias_.size()) return;
    is_playlist_modified_ = true;
    VLC::Media _media =
        VLC::Media(vlc_instance_, media->location_, VLC::Media::FromLocation);
    vlc_media_list_.insertMedia(_media, index);
    state_->medias_->medias_.insert(state_->medias_->medias_.begin() + index,
                                    std::move(media));
    OnPlaylistCallback();
    if (state_->index_ <= index) state_->index_++;
    state_->is_playlist_ = true;
  }

  void Move(int32_t initial, int32_t final) {
    if (initial < 0 || initial >= state_->medias_->medias_.size() ||
        final < 0 || final >= state_->medias_->medias_.size())
      return;
    if (initial == final) return;
    is_playlist_modified_ = true;
    std::shared_ptr<Media> media = state_->medias_->medias_[initial];
    VLC::Media _media = VLC::Media(
        vlc_instance_, vlc_media_list_.itemAtIndex(initial).get()->mrl(),
        VLC::Media::FromLocation);
    state_->medias_->medias_.erase(state_->medias_->medias_.begin() + initial);
    vlc_media_list_.removeIndex(initial);
    state_->medias_->medias_.insert(state_->medias_->medias_.begin() + final,
                                    std::move(media));
    vlc_media_list_.insertMedia(_media, final);
    if (initial == state_->index_) {
      state_->index_ = final;
    } else if (final == state_->index_) {
      state_->index_++;
    } else if (!((initial < state_->index_ && final < state_->index_) ||
                 (initial > state_->index_ && final > state_->index_))) {
      if (initial > final)
        state_->index_++;
      else
        state_->index_--;
    }
    OnPlaylistCallback();
  }
};
