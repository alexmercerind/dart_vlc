/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include <iostream>
#include <vector>

#include "player.hpp"


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

int main(int argc, char** argv) {
	Player* player = new Player(0);
	player->onEvent([player] () -> void {
		std::cout << std::boolalpha;
		std::cout << "index       : " << player->state->index << std::endl;
		std::cout << "position    : " << player->state->position << std::endl;
		std::cout << "duration    : " << player->state->duration << std::endl;
		std::cout << "isPlaying   : " << player->state->isPlaying << std::endl;
		std::cout << "isSeekable  : " << player->state->isSeekable << std::endl;
		std::cout << "isCompleted : " << player->state->isCompleted << std::endl;
		std::cout << "isValid     : " << player->state->isValid << std::endl;
		std::cout << "volume      : " << player->state->volume << std::endl;
		std::cout << "playlist    : " << "[ " << std::endl;
		int index = 0;
		for (Media* media : player->state->medias->medias) {
			std::cout << "    " << index << ". " << media->mediaType << ", " << media->resource << ", " << std::endl;
			index++;
		}
		std::cout << "]" << std::endl << std::endl;
	});
	std::vector<Media*> medias;
	for (int index = 0; index < argc; index++) {
		medias.emplace_back(
			Media::file(argv[index])
		);
	}
	player->open(
		new Playlist(medias)
	);
	std::cin.get();
	return 0;
}
