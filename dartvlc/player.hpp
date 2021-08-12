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

#include "internal/setters.hpp"

#ifndef Player_HEADER
#define Player_HEADER

auto TO_CHARARRAY = [](std::vector<std::string>* vector) -> char** {
  size_t size = vector->size();
  char** array = new char*[size];
  for (int32_t i = 0; i < size; i++) array[i] = (*vector)[i].data();
  return array;
};

class Player : public PlayerSetters {
 public:
  Player(int32_t id, std::vector<std::string> cmd_arguments = {}) {
    if (cmd_arguments.empty()) {
      vlc_instance_ = VLC::Instance(0, nullptr);
    } else {
      char** args = TO_CHARARRAY(&cmd_arguments);
      vlc_instance_ =
          VLC::Instance(static_cast<int32_t>(cmd_arguments.size()), args);
      delete[] args;
    }
    vlc_media_player_ = VLC::MediaPlayer(vlc_instance_);
    vlc_media_list_player_ = VLC::MediaListPlayer(vlc_instance_);
    vlc_media_list_ = VLC::MediaList(vlc_instance_);
    vlc_media_list_player_.setMediaPlayer(vlc_media_player_);
    state_ = std::make_unique<PlayerState>();
    state_->id_ = id;
    vlc_media_player_.setVolume(100);
  }

  ~Player() { vlc_media_player_.stop(); }
};

class Players {
 public:
  Player* Get(int32_t id, std::vector<std::string> cmd_arguments = {}) {
    auto it = players_.find(id);
    if (it == players_.end()) {
      players_[id] = std::make_unique<Player>(id, cmd_arguments);
    }
    return players_[id].get();
  }

  void Dispose(int32_t id, std::function<void()> callback = []() -> void {}) {
    players_.erase(id);
    callback();
  }

 private:
  std::map<int32_t, std::unique_ptr<Player>> players_;
};

std::unique_ptr<Players> g_players = std::make_unique<Players>();

#endif
