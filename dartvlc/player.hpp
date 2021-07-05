/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
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


class Player: public PlayerSetters {
public:
	Player(int id, std::vector<std::string> commandlineArguments = {}) {
		if (commandlineArguments.empty()) {
			this->instance = VLC::Instance(0, nullptr);
		}
		else {
			char** args = new char*[commandlineArguments.size()];
			for (int index = 0; index < commandlineArguments.size(); index++)  args[index] = commandlineArguments[index].data();
			this->instance = VLC::Instance(static_cast<int>(commandlineArguments.size()), args);
		}
		this->mediaPlayer = VLC::MediaPlayer(this->instance);
		this->mediaListPlayer = VLC::MediaListPlayer(this->instance);
		this->mediaList = VLC::MediaList(this->instance);
		this->mediaListPlayer.setMediaPlayer(this->mediaPlayer);
		this->state = new PlayerState();
		this->state->id = id;
		this->mediaPlayer.setVolume(100);
	}

	void onEvent(std::function<void(void)> callback) {
		this->onPlay(callback);
		this->onPause(callback);
		this->onStop(callback);
		this->onComplete(callback);
		this->onSeekable(
			[callback](bool _) -> void { callback(); }
		);
		this->onOpen(
			[callback](VLC::Media _) -> void { callback(); }
		);
		this->onPosition(
			[callback](int _) -> void { callback(); }
		);
		this->onPlaylist(callback);
	}

	void onException(std::function<void(void)> callback) {
		this->mediaPlayer.eventManager().onEncounteredError(callback);
	}
};


class Players {
public:
	Player* get(int id, std::vector<std::string> commandlineArguments = {}) {
		if (this->players.find(id) == this->players.end()) {
			this->players[id] = new Player(id, commandlineArguments);
		}
		return this->players[id];
	}

private:
	std::map<int, Player*> players;
};


Players* players = new Players();


#endif
