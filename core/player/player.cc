// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "player/player.h"

#include <functional>

Player::Player(const std::vector<std::string>& cmd_arguments) {
  if (cmd_arguments.empty()) {
    vlc_instance_ = VLC::Instance(0, nullptr);
  } else {
    auto c_array =
        std::unique_ptr<const char*[]>(new const char*[cmd_arguments.size()]);
    for (auto i = 0; i < cmd_arguments.size(); i++) {
      c_array[i] = cmd_arguments[i].c_str();
    }
    vlc_instance_ = VLC::Instance(static_cast<int32_t>(cmd_arguments.size()),
                                  c_array.get());
  }
  vlc_media_player_ = VLC::MediaPlayer(vlc_instance_);
  vlc_media_list_player_ = VLC::MediaListPlayer(vlc_instance_);
  vlc_media_list_ = VLC::MediaList(vlc_instance_);
  vlc_media_list_player_.setMediaPlayer(vlc_media_player_);
  state_ = std::make_unique<PlayerState>();
  vlc_media_player_.setVolume(100);
}

void Player::Open(std::shared_ptr<MediaSource> media_source, bool auto_start) {
  state_->set_is_started(false);
  state_->Reset();
  Stop();
  state_->medias()->medias() = {};
  vlc_media_list_ = VLC::MediaList(vlc_instance_);
  if (media_source->Type() == "MediaSourceType.media") {
    std::shared_ptr<Media> media =
        std::dynamic_pointer_cast<Media>(media_source);
    VLC::Media vlc_media =
        VLC::Media(vlc_instance_, media->location(), VLC::Media::FromLocation);
    if (media->start_time() != "") {
      vlc_media.addOption(media->start_time());
    }
    if (media->stop_time() != "") {
      vlc_media.addOption(media->stop_time());
    }
    vlc_media_list_.lock();
    vlc_media_list_.addMedia(vlc_media);
    vlc_media_list_.unlock();
    vlc_media_list_player_.setMediaList(vlc_media_list_);
    state_->medias()->medias() = {media};
    state_->set_is_playlist(false);
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
      vlc_media_list_.lock();
      vlc_media_list_.addMedia(vlc_media);
      vlc_media_list_.unlock();
      state_->medias()->medias().emplace_back(media);
    }
    vlc_media_list_player_.setMediaList(vlc_media_list_);
    state_->set_is_playlist(false);
  }
  OnOpenCallback(vlc_media_list_.itemAtIndex(0));
  if (auto_start) Play();
}

void Player::Play() {
  if (!state_->is_started() && !state_->medias()->medias().empty()) {
    vlc_media_list_player_.playItemAtIndex(0);
    state_->set_is_started(true);
  } else {
    vlc_media_list_player_.play();
  }
}

void Player::Pause() {
  if (vlc_media_list_player_.isPlaying()) {
    vlc_media_list_player_.pause();
  }
}

void Player::PlayOrPause() {
  if (!state_->is_started() && !state_->medias()->medias().empty()) {
    vlc_media_list_player_.playItemAtIndex(0);
    state_->set_is_started(true);
  } else {
    vlc_media_list_player_.pause();
  }
}

void Player::Stop() { vlc_media_list_player_.stop(); }

void Player::Next() {
  OnPlaylistCallback();
  if (state_->index() < vlc_media_list_.count()) {
    state_->set_index(state_->index() + 1);
    vlc_media_list_player_.playItemAtIndex(state_->index());
  }
}

void Player::Previous() {
  OnPlaylistCallback();
  if (state_->index() > 0) {
    state_->set_index(state_->index() - 1);
    vlc_media_list_player_.playItemAtIndex(state_->index());
  }
}

void Player::JumpToIndex(int32_t index) {
  OnPlaylistCallback();
  if (index >= 0 && index < vlc_media_list_.count())
    vlc_media_list_player_.playItemAtIndex(index);
}

void Player::Seek(int32_t position) { vlc_media_player_.setTime(position); }

void Player::SetVolume(float volume) {
  vlc_media_player_.setVolume(static_cast<int32_t>(volume * 100));
  state_->set_volume(volume);
  volume_callback_(volume);
}

void Player::SetRate(float rate) {
  vlc_media_player_.setRate(rate);
  state_->set_rate(rate);
  rate_callback_(rate);
}

void Player::SetDevice(Device device) {
  vlc_media_player_.outputDeviceSet(device.id());
}

void Player::SetPlaylistMode(PlaylistMode playlist_mode) {
  vlc_media_list_player_.setPlaybackMode(
      static_cast<libvlc_playback_mode_t>(playlist_mode));
}

void Player::SetEqualizer(Equalizer* equalizer) {
  vlc_media_player_.setEqualizer(*equalizer->vlc_equalizer());
}

void Player::SetUserAgent(std::string user_agent) {
  vlc_instance_.setUserAgent("dart_vlc", user_agent);
}

void Player::Add(std::shared_ptr<Media> media) {
  is_playlist_modified_ = true;
  VLC::Media vlc_media =
      VLC::Media(vlc_instance_, media->location(), VLC::Media::FromLocation);
  vlc_media_list_.lock();
  vlc_media_list_.addMedia(vlc_media);
  vlc_media_list_.unlock();
  state_->medias()->medias().emplace_back(media);
  OnPlaylistCallback();
  state_->set_is_playlist(true);
}

void Player::Remove(int32_t index) {
  if (index < 0 || index >= state_->medias()->medias().size()) return;
  is_playlist_modified_ = true;
  state_->medias()->medias().erase(state_->medias()->medias().begin() + index);
  vlc_media_list_.lock();
  vlc_media_list_.removeIndex(index);
  vlc_media_list_.unlock();
  OnPlaylistCallback();
  if (!state_->is_completed() && state_->index() == index) {
    if (state_->index() == vlc_media_list_.count()) {
      vlc_media_list_player_.stop();
    } else {
      JumpToIndex(index);
    }
  }
  if (state_->index() > index) state_->set_index(state_->index() - 1);
  state_->set_is_playlist(true);
}

void Player::Insert(int32_t index, std::shared_ptr<Media> media) {
  if (index < 0 || index >= state_->medias()->medias().size()) return;
  is_playlist_modified_ = true;
  VLC::Media vlc_media =
      VLC::Media(vlc_instance_, media->location(), VLC::Media::FromLocation);
  vlc_media_list_.lock();
  vlc_media_list_.insertMedia(vlc_media, index);
  vlc_media_list_.unlock();
  state_->medias()->medias().insert(state_->medias()->medias().begin() + index,
                                    media);
  OnPlaylistCallback();
  if (state_->index() <= index) state_->set_index(state_->index() + 1);
  state_->set_is_playlist(true);
}

void Player::Move(int32_t from, int32_t to) {
  if (from < 0 || from >= state_->medias()->medias().size() || to < 0 ||
      to >= state_->medias()->medias().size())
    return;
  if (from == to) return;
  is_playlist_modified_ = true;
  std::shared_ptr<Media> media = state_->medias()->medias()[from];
  VLC::Media _media =
      VLC::Media(vlc_instance_, vlc_media_list_.itemAtIndex(from).get()->mrl(),
                 VLC::Media::FromLocation);
  state_->medias()->medias().erase(state_->medias()->medias().begin() + from);
  vlc_media_list_.lock();
  vlc_media_list_.removeIndex(from);
  vlc_media_list_.unlock();
  state_->medias()->medias().insert(state_->medias()->medias().begin() + to,
                                    std::move(media));
  vlc_media_list_.lock();
  vlc_media_list_.insertMedia(_media, to);
  vlc_media_list_.unlock();
  if (from == state_->index()) {
    state_->set_index(to);
  } else if (to == state_->index()) {
    state_->set_index(state_->index() + 1);
  } else if (!((from < state_->index() && to < state_->index()) ||
               (from > state_->index() && to > state_->index()))) {
    if (from > to)
      state_->set_index(state_->index() + 1);
    else
      state_->set_index(state_->index() - 1);
  }
  OnPlaylistCallback();
}

void Player::TakeSnapshot(std::string file_path, int32_t width,
                          int32_t height) {
  vlc_media_player_.takeSnapshot(0, file_path, width, height);
}

void Player::SetVideoWidth(int32_t width) { preferred_video_width_ = width; }

void Player::SetVideoHeight(int32_t height) {
  preferred_video_height_ = height;
}

void Player::SetAudioTrack(int32_t track) {
  vlc_media_player_.setAudioTrack(track);
}

int32_t Player::GetAudioTrackCount() {
  return vlc_media_player_.audioTrackCount();
}

void Player::SetHWND(int64_t hwnd) {
  vlc_media_player_.setHwnd(reinterpret_cast<void*>(hwnd));
}

void Player::SetOpenCallback(std::function<void(VLC::Media)> callback) {
  open_callback_ = callback;
  vlc_media_player_.eventManager().onMediaChanged(
      std::bind(&Player::OnOpenCallback, this, std::placeholders::_1));
}

void Player::SetPlayCallback(std::function<void()> callback) {
  play_callback_ = callback;
  vlc_media_player_.eventManager().onPlaying(
      std::bind(&Player::OnPlayCallback, this));
}

void Player::SetVideoDimensionsCallback(
    std::function<void(int32_t, int32_t)> callback) {
  video_dimension_callback_ = callback;
  vlc_media_player_.setVideoCallbacks(
      std::bind(&Player::OnVideoLockCallback, this, std::placeholders::_1),
      nullptr,
      std::bind(&Player::OnVideoPictureCallback, this, std::placeholders::_1));
  vlc_media_player_.setVideoFormatCallbacks(
      [=](char* chroma, uint32_t* w, uint32_t* h, uint32_t* p,
          uint32_t* l) -> int32_t {
        int32_t video_width = video_width_, video_height = video_height_,
                pitch = video_width_ * 4;
        if (video_height_ != static_cast<int32_t>(*h) ||
            video_width_ != static_cast<int32_t>(*w)) {
          video_height_ = static_cast<int32_t>(*h);
          video_width_ = static_cast<int32_t>(*w);
          video_dimension_callback_(video_width_, video_height_);
          if (preferred_video_width_.has_value() &&
              preferred_video_height_.has_value()) {
            video_width = preferred_video_width_.value_or(0);
            video_height = preferred_video_height_.value_or(0);
            pitch = video_width_ * 4;
#ifndef __APPLE__
            vlc_media_player_.setVideoFormat("RGBA", video_width, video_height,
                                             pitch);
#else
            vlc_media_player_.setVideoFormat("RV32", video_width, video_height,
                                             pitch);
#endif
          } else {
            video_height = video_height_;
            video_width = video_width_;
            pitch = video_width_ * 4;
#ifndef __APPLE__
            vlc_media_player_.setVideoFormat("RGBA", video_width, video_height,
                                             pitch);
#else
            vlc_media_player_.setVideoFormat("RV32", video_width, video_height,
                                             pitch);
#endif
          }
        }
#ifndef __APPLE__
        strncpy(chroma, "RGBA", 4);
#else
        strncpy(chroma, "RV32", 4);
#endif
        *w = preferred_video_width_.value_or(video_width_);
        *h = preferred_video_height_.value_or(video_height_);
        *p = preferred_video_width_.value_or(video_width_) * 4;
        *l = preferred_video_height_.value_or(video_height_);
        return 1;
      },
      nullptr);
}

void Player::SetPauseCallback(std::function<void()> callback) {
  pause_callback_ = callback;
  vlc_media_player_.eventManager().onPaused(
      std::bind(&Player::OnPauseCallback, this));
}

void Player::SetStopCallback(std::function<void()> callback) {
  stop_callback_ = callback;
  vlc_media_player_.eventManager().onStopped(
      std::bind(&Player::OnStopCallback, this));
}

void Player::SetPositionCallback(std::function<void(int32_t)> callback) {
  position_callback_ = callback;
  vlc_media_player_.eventManager().onPositionChanged(
      std::bind(&Player::OnPositionCallback, this, std::placeholders::_1));
}

void Player::SetSeekableCallback(std::function<void(bool)> callback) {
  seekable_callback_ = callback;
  vlc_media_player_.eventManager().onSeekableChanged(
      std::bind(&Player::OnSeekableCallback, this, std::placeholders::_1));
}

void Player::SetCompleteCallback(std::function<void()> callback) {
  complete_callback_ = callback;
  vlc_media_player_.eventManager().onEndReached(
      std::bind(&Player::OnCompleteCallback, this));
}

void Player::SetVolumeCallback(std::function<void(float)> callback) {
  volume_callback_ = callback;
}

void Player::SetRateCallback(std::function<void(float)> callback) {
  rate_callback_ = callback;
}

void Player::SetPlaylistCallback(std::function<void()> callback) {
  playlist_callback_ = callback;
}

void Player::SetBufferingCallback(std::function<void(float)> callback) {
  vlc_media_player_.eventManager().onBuffering(callback);
}

void Player::SetVideoFrameCallback(
    std::function<void(uint8_t*, int32_t, int32_t)> callback) {
  video_callback_ = callback;
}

void Player::SetErrorCallback(std::function<void(std::string)> callback) {
  vlc_media_player_.eventManager().onEncounteredError([=]() -> void {
    auto error = libvlc_errmsg();
    callback(error != 0 ? error : "");
  });
}

void Player::OnPlaylistCallback() {
  if (is_playlist_modified_) {
    vlc_media_list_player_.setMediaList(vlc_media_list_);
    if (!vlc_media_list_.count()) {
      state_->Reset();
      vlc_media_list_player_.stop();
      return;
    }
    if (state_->index() > vlc_media_list_.count())
      state_->set_index(vlc_media_list_.count() - 1);
    is_playlist_modified_ = false;
    playlist_callback_();
  };
}

void Player::OnOpenCallback(VLC::MediaPtr vlc_media_ptr) {
  state_->set_is_playing(vlc_media_player_.isPlaying());
  state_->set_is_valid(vlc_media_player_.isValid());
  if (duration() > 0) {
    state_->set_is_completed(false);
    state_->set_position(position());
    state_->set_duration(duration());
  } else {
    state_->set_is_completed(false);
    state_->set_position(0);
    state_->set_duration(0);
  }
  state_->set_index(vlc_media_list_.indexOfItem(*vlc_media_ptr.get()));
  open_callback_(*vlc_media_ptr.get());
}

void Player::OnPlayCallback() {
  state_->set_is_playing(vlc_media_player_.isPlaying());
  state_->set_is_valid(vlc_media_player_.isValid());
  if (duration() > 0) {
    state_->set_is_completed(false);
    state_->set_position(position());
    state_->set_duration(duration());
  }
  play_callback_();
}

void Player::OnPauseCallback() {
  state_->set_is_playing(vlc_media_player_.isPlaying());
  state_->set_is_valid(vlc_media_player_.isValid());
  if (duration() > 0) {
    state_->set_position(position());
    state_->set_duration(duration());
  }
  pause_callback_();
}

void Player::OnStopCallback() {
  state_->set_is_playing(vlc_media_player_.isPlaying());
  state_->set_is_valid(vlc_media_player_.isValid());
  state_->set_position(0);
  state_->set_duration(0);
  stop_callback_();
}

void Player::OnPositionCallback(float relative_position) {
  state_->set_is_playing(vlc_media_player_.isPlaying());
  state_->set_is_valid(vlc_media_player_.isValid());
  if (duration() > 0) {
    state_->set_position(position());
    state_->set_duration(duration());
  }
  position_callback_(
      static_cast<int32_t>(relative_position * vlc_media_player_.length()));
}

void Player::OnSeekableCallback(bool is_seekable) {
  if (duration() > 0) {
    state_->set_is_seekable(is_seekable);
    seekable_callback_(is_seekable);
  }
}

void Player::OnCompleteCallback() {
  state_->set_is_playing(vlc_media_player_.isPlaying());
  state_->set_is_valid(vlc_media_player_.isValid());
  if (duration() > 0) {
    state_->set_is_completed(true);
    state_->set_position(position());
    state_->set_duration(duration());
    OnPlaylistCallback();
    complete_callback_();
  } else {
    state_->set_position(0);
    state_->set_duration(0);
  }
}

void* Player::OnVideoLockCallback(void** planes) {
  planes[0] = static_cast<void*>(video_frame_buffer_.get());
  return nullptr;
}

void Player::OnVideoPictureCallback(void* picture) {
  if (video_callback_) {
    video_callback_(video_frame_buffer_.get(),
                    preferred_video_width_.value_or(video_width_),
                    preferred_video_height_.value_or(video_height_));
  }
}

Player::~Player() { vlc_media_player_.stop(); }
