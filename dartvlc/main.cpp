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
Devices* devices = new Devices();


int main() {
	Player* player = new Player(0);
	player->onEvent([]() -> void {});
	std::string input;
	while (true) {
		std::cout << "Listening..." << std::endl;
		std::cin >> input;
		if (input == "show") {
			std::cout << std::boolalpha;
			std::cout << "index       : " << player->state->index << std::endl;
			std::cout << "position    : " << player->state->position << std::endl;
			std::cout << "duration    : " << player->state->duration << std::endl;
			std::cout << "isPlaylist  : " << player->state->isPlaylist << std::endl;
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
		}
		if (input == "add") {
			std::string media;
			std::cout << "Media Location:";
			std::cin >> media;
			player->add(
				Media::file(media)
			);
		}
		if (input == "remove") {
			int index;
			std::cout << "Index:";
			std::cin >> index;
			player->remove(index);
		}
		if (input == "move") {
			int initial;
			std::cout << "Initial:";
			std::cin >> initial;
			int final;
			std::cout << "Final:";
			std::cin >> final;
			player->move(initial, final);
		}
		if (input == "insert") {
			int index;
			std::string media;
			std::cout << "Index:";
			std::cin >> index;
			std::cout << "Media Location:";
			std::cin >> media;
			player->insert(
				index,
				Media::file(media)
			);
		}
		if (input == "play") {
			player->play();
		}
		if (input == "pause") {
			player->pause();
		}
		if (input == "next") {
			player->next();
		}
		if (input == "back") {
			player->back();
		}
		if (input == "jump") {
			int index;
			std::cout << "Index:";
			std::cin >> index;
			player->jump(index);
		}
		if (input == "seek") {
			int duration;
			std::cout << "Duration:";
			std::cin >> duration;
			player->seek(duration);
		}
	}
	std::cin.get();
	return 0;
}
