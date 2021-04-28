/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include <iostream>

#include "player.hpp"
#include "device.hpp"
#include "broadcast.hpp"
#include "record.hpp"
#include "chromecast.hpp"

/* libVLC++ wrapper for dart_vlc */

int main() {
	Player* player = new Player(0);
	player->onEvent([]() -> void {});
    player->add(Media::file(0, "C:/audio1.mp3"));
    player->add(Media::file(0, "C:/audio2.mp3"));
    player->add(Media::file(0, "C:/audio3.mp3"));
    player->add(Media::file(0, "C:/audio4.mp3"));
    player->add(Media::file(0, "C:/audio5.mp3"));
	std::string input;
	while (true) {
		std::cout << "listening..." << std::endl;
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
			std::cout << "medias      : " << "[ " << std::endl;
			int index = 0;
			for (Media* media : player->state->medias->medias) {
				std::cout << "    " << index << ". " << media->mediaType << ", " << media->resource << ", " << std::endl;
				index++;
			}
			std::cout << "]" << std::endl << std::endl;
		}
        if (input == "open") {
			std::string media;
			std::cout << "media location:";
			std::cin >> media;
			player->open(
				Media::file(0, media)
			);
		}
		if (input == "add") {
			std::string media;
			std::cout << "media location:";
			std::cin >> media;
			player->add(
				Media::file(0, media)
			);
		}
		if (input == "remove") {
			int index;
			std::cout << "index:";
			std::cin >> index;
			player->remove(index);
		}
		if (input == "insert") {
			int index;
			std::string media;
			std::cout << "Index:";
			std::cin >> index;
			std::cout << "media location:";
			std::cin >> media;
			player->insert(
				index,
				Media::file(0, media)
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
			std::cout << "index:";
			std::cin >> index;
			player->jump(index);
		}
		if (input == "seek") {
			int duration;
			std::cout << "duration:";
			std::cin >> duration;
			player->seek(duration);
		}
		if (input == "exit") {
			break;
		}
	}
	std::cin.get();
	return 0;
}
