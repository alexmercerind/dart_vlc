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

#include "device.h"
#include "internal/events.h"
#include "mediasource/media.h"
#include "mediasource/mediasource.h"
#include "mediasource/playlist.h"

class PlayerSetters : public PlayerEvents {
 public:
  void Open(std::shared_ptr<MediaSource> media_source, bool auto_start = true) {
    state()->is_started_ = false;
    state()->Reset();
    Stop();
    state()->medias()->medias() = {};
    vlc_media_list_ = VLC::MediaList(vlc_instance_);
    if (media_source->Type() == "MediaSourceType.media") {
      std::shared_ptr<Media> media =
          std::dynamic_pointer_cast<Media>(media_source);
      VLC::Media vlc_media = VLC::Media(vlc_instance_, media->location(),
                                        VLC::Media::FromLocation);
      if (media->start_time() != "") {
        vlc_media.addOption(media->start_time());
      }
      if (media->stop_time() != "") {
        vlc_media.addOption(media->stop_time());
      }
      vlc_media_list_.addMedia(vlc_media);
      vlc_media_list_player_.setMediaList(vlc_media_list_);
      state()->medias()->medias() = {media};
      state()->is_playlist_ = false;
    } else if (media_source->Type() == "MediaSourceType.playlist") {
      std::shared_ptr<Playlist> playlist =
          std::dynamic_pointer_cast<Playlist>(media_source);
      if (playlist->medias().empty()) return;
      for (std::shared_ptr<Media>& media : playlist->medias()) {
        VLC::Media vlc_media = VLC::Media(vlc_instance_, media->location(),
                                          VLC::Media::FromLocation);
        if (media->start_time() != "") {
          vlc_media.addOption(media->start_time());
        }
        if (media->stop_time() != "") {
          vlc_media.addOption(media->stop_time());
        }
        vlc_media_list_.addMedia(vlc_media);
        state()->medias()->medias().emplace_back(media);
      }
      vlc_media_list_player_.setMediaList(vlc_media_list_);
      state()->is_playlist_ = true;
    }
    OnOpenCallback(vlc_media_list_.itemAtIndex(0));
    if (auto_start) Play();
  }

  void Play() {
    if (!state()->is_started_ && !state()->medias()->medias().empty()) {
      vlc_media_list_player_.playItemAtIndex(0);
      state()->is_started_ = true;
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
    if (!state()->is_started_ && !state()->medias()->medias().empty()) {
      vlc_media_list_player_.playItemAtIndex(0);
      state()->is_started_ = true;
    } else {
      vlc_media_list_player_.pause();
    }
  }

  void Stop() { vlc_media_list_player_.stop(); }

  void Next() {
    OnPlaylistCallback();
    if (state()->index_ < vlc_media_list_.count())
      vlc_media_list_player_.playItemAtIndex(++state()->index_);
  }

  void Back() {
    OnPlaylistCallback();
    if (state()->index_ > 0)
      vlc_media_list_player_.playItemAtIndex(--state()->index_);
  }

  void Jump(int32_t index) {
    OnPlaylistCallback();
    if (index >= 0 && index < vlc_media_list_.count())
      vlc_media_list_player_.playItemAtIndex(index);
  }

  void Seek(int32_t position) {
    vlc_media_player_.setPosition(static_cast<float>(position) /
                                  static_cast<float>(duration()));
  }

  void SetVolume(float volume) {
    vlc_media_player_.setVolume(static_cast<int32_t>(volume * 100));
    state()->volume_ = volume;
    volume_callback_(volume);
  }

  void SetRate(float rate) {
    vlc_media_player_.setRate(rate);
    state()->rate_ = rate;
    rate_callback_(rate);
  }

  void SetDevice(Device device) {
    vlc_media_player_.outputDeviceSet(device.id());
  }

  void SetPlaylistMode(PlaylistMode mode) {
    vlc_media_list_player_.setPlaybackMode(
        static_cast<libvlc_playback_mode_t>(mode));
  }

  void SetEqualizer(Equalizer equalizer) {
    vlc_media_player_.setEqualizer(equalizer.vlc_equalizer_);
  }

  void SetUserAgent(std::string userAgent) {
    vlc_instance_.setUserAgent("dart_vlc", userAgent);
  }

  void Add(std::shared_ptr<Media> media) {
    is_playlist_modified_ = true;
    VLC::Media vlc_media =
        VLC::Media(vlc_instance_, media->location(), VLC::Media::FromLocation);
    vlc_media_list_.addMedia(vlc_media);
    state()->medias()->medias().emplace_back(media);
    OnPlaylistCallback();
    state()->is_playlist_ = true;
  }

  void Remove(int32_t index) {
    if (index < 0 || index >= state()->medias()->medias().size()) return;
    is_playlist_modified_ = true;
    state()->medias()->medias().erase(state()->medias()->medias().begin() +
                                      index);
    vlc_media_list_.removeIndex(index);
    OnPlaylistCallback();
    if (!state()->is_completed_ && state()->index_ == index) {
      if (state()->index_ == vlc_media_list_.count()) {
        vlc_media_list_player_.stop();
      } else
        Jump(index);
    }
    if (state()->index_ > index) state()->index_--;
    state()->is_playlist_ = true;
  }

  void Insert(int32_t index, std::shared_ptr<Media> media) {
    if (index < 0 || index >= state()->medias()->medias().size()) return;
    is_playlist_modified_ = true;
    VLC::Media vlc_media =
        VLC::Media(vlc_instance_, media->location(), VLC::Media::FromLocation);
    vlc_media_list_.insertMedia(vlc_media, index);
    state()->medias()->medias().insert(
        state()->medias()->medias().begin() + index, media);
    OnPlaylistCallback();
    if (state()->index_ <= index) state()->index_++;
    state()->is_playlist_ = true;
  }

  void Move(int32_t initial, int32_t final) {
    if (initial < 0 || initial >= state()->medias()->medias().size() ||
        final < 0 || final >= state()->medias()->medias().size())
      return;
    if (initial == final) return;
    is_playlist_modified_ = true;
    std::shared_ptr<Media> media = state()->medias()->medias()[initial];
    VLC::Media _media = VLC::Media(
        vlc_instance_, vlc_media_list_.itemAtIndex(initial).get()->mrl(),
        VLC::Media::FromLocation);
    state()->medias()->medias().erase(state()->medias()->medias().begin() +
                                      initial);
    vlc_media_list_.removeIndex(initial);
    state()->medias()->medias().insert(
        state()->medias()->medias().begin() + final, std::move(media));
    vlc_media_list_.insertMedia(_media, final);
    if (initial == state()->index_) {
      state()->index_ = final;
    } else if (final == state()->index_) {
      state()->index_++;
    } else if (!((initial < state()->index_ && final < state()->index_) ||
                 (initial > state()->index_ && final > state()->index_))) {
      if (initial > final)
        state()->index_++;
      else
        state()->index_--;
    }
    OnPlaylistCallback();
  }

  void TakeSnapshot(std::string file_path, int32_t width, int32_t height) {
    vlc_media_player_.takeSnapshot(0, file_path, width, height);
  }

  void SetVideoWidth(int32_t video_width) {
    preferred_video_width_ = video_width;
  }

  void SetVideoHeight(int32_t video_height) {
    preferred_video_height_ = video_height;
  }
};
