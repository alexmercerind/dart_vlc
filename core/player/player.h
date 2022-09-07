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

#ifndef PLAYER_PLAYER_H_
#define PLAYER_PLAYER_H_

#include <memory>
#include <optional>
#include <vector>
#include <vlcpp/vlc.hpp>

#include "player/player_state.h"

class Player {
  static constexpr auto kFrameBufferSize = 67108864;

 public:
  int32_t video_width() const { return video_width_; }
  int32_t video_height() const { return video_height_; }
  PlayerState* state() const { return state_.get(); }
  int32_t duration() {
    return static_cast<int32_t>(vlc_media_player_.length());
  }
  int32_t position() { return static_cast<int32_t>(vlc_media_player_.time()); }
  float volume() { return vlc_media_player_.volume() / 100.0f; }
  float rate() { return vlc_media_player_.rate(); }
  bool is_playing() { return vlc_media_player_.isPlaying(); }
  bool is_paused() { return !vlc_media_player_.isPlaying(); }
  int32_t audio_track_count() { return vlc_media_player_.audioTrackCount(); }

  // Public API.

  Player(const std::vector<std::string>& cmd_arguments);

  void Open(std::shared_ptr<MediaSource> media_source, bool auto_start = true);

  void Play();

  void Pause();

  void PlayOrPause();

  void Stop();

  void Next();

  void Previous();

  void JumpToIndex(int32_t index);

  void Seek(int32_t position);

  void SetVolume(float volume);

  void SetRate(float rate);

  void SetDevice(Device device);

  void SetPlaylistMode(PlaylistMode playlist_mode);

  void SetEqualizer(Equalizer* equalizer);

  void SetUserAgent(std::string user_agent);

  void Add(std::shared_ptr<Media> media);

  void Remove(int32_t index);

  void Insert(int32_t index, std::shared_ptr<Media> media);

  void Move(int32_t from, int32_t to);

  void TakeSnapshot(std::string file_path, int32_t width, int32_t height);

  void SetVideoWidth(int32_t width);

  void SetVideoHeight(int32_t height);

  void SetAudioTrack(int32_t track);

  int32_t GetAudioTrackCount();

  void SetHWND(int64_t hwnd);

  // These methods register callbacks from the client code which get invoked
  // when the corresponding event occurs.

  void SetOpenCallback(std::function<void(VLC::Media)> callback);

  void SetPlayCallback(std::function<void()> callback);

  void SetVideoDimensionsCallback(
      std::function<void(int32_t, int32_t)> callback);

  void SetPauseCallback(std::function<void()> callback);

  void SetStopCallback(std::function<void()> callback);

  void SetPositionCallback(std::function<void(int32_t)> callback);

  void SetSeekableCallback(std::function<void(bool)> callback);

  void SetCompleteCallback(std::function<void()> callback);

  void SetVolumeCallback(std::function<void(float)> callback);

  void SetRateCallback(std::function<void(float)> callback);

  void SetPlaylistCallback(std::function<void()> callback);

  void SetBufferingCallback(std::function<void(float)> callback);

  void SetVideoFrameCallback(
      std::function<void(uint8_t*, int32_t, int32_t)> callback);

  void SetErrorCallback(std::function<void(std::string)> callback);

  ~Player();

 private:
  // Internal methods which get called when the corresponding event occurs &
  // invoke the corresponding lambda callbacks.

  void OnPlaylistCallback();

  void OnOpenCallback(VLC::MediaPtr vlc_media_ptr);

  void OnPlayCallback();

  void OnPauseCallback();

  void OnStopCallback();

  void OnPositionCallback(float relative_position);

  void OnSeekableCallback(bool isSeekable);

  void OnCompleteCallback();

  void* OnVideoLockCallback(void** planes);

  void OnVideoPictureCallback(void* picture);

  VLC::Instance vlc_instance_;
  VLC::MediaPlayer vlc_media_player_;
  VLC::MediaListPlayer vlc_media_list_player_;
  VLC::MediaList vlc_media_list_;
  std::unique_ptr<PlayerState> state_ = std::make_unique<PlayerState>();
  std::unique_ptr<uint8_t[]> video_frame_buffer_ =
      std::make_unique<uint8_t[]>(kFrameBufferSize);
  int32_t video_width_ = -1;
  int32_t video_height_ = -1;
  std::optional<int32_t> preferred_video_width_ = std::nullopt;
  std::optional<int32_t> preferred_video_height_ = std::nullopt;
  bool is_playlist_modified_ = false;

  // lambda callbacks that are registered by client code & get called when
  // |Player| state changes.

  std::function<void()> playlist_callback_ = nullptr;
  std::function<void(VLC::Media)> open_callback_ = nullptr;
  std::function<void(int32_t, int32_t)> video_dimension_callback_ = nullptr;
  std::function<void()> play_callback_ = nullptr;
  std::function<void()> pause_callback_ = nullptr;
  std::function<void()> stop_callback_ = nullptr;
  std::function<void(int32_t)> position_callback_ = nullptr;
  std::function<void(bool)> seekable_callback_ = nullptr;
  std::function<void()> complete_callback_ = nullptr;
  std::function<void(float)> volume_callback_ = nullptr;
  std::function<void(float)> rate_callback_ = nullptr;
  std::function<void(uint8_t*, int32_t, int32_t)> video_callback_ = nullptr;
};

#endif  // PLAYER_PLAYER_H_
