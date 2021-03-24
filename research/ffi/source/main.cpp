/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#ifdef _WIN32
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT
#endif

#include <iostream>
#include <vector>

#include "dart_vlc/player.hpp"


class Players {
public:
	Player* get(int id) {
		if (this->players.find(id) == this->players.end()) {
			this->players[id] = new Player(id);
		}
		return this->players[id];
	}

private:
	std::map<int, Player*> players;
};

Players* players = new Players();


extern "C" {
	EXPORT void create() {}

	EXPORT void open() {}

	EXPORT void play() {}

	EXPORT void pause() {}

	EXPORT void playOrPause() {}

	EXPORT void stop() {}

	EXPORT void next() {}

	EXPORT void back() {}

	EXPORT void jump() {}

	EXPORT void seek() {}

	EXPORT void setVolume() {}

	EXPORT void setRate() {}
}
