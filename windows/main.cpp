#include <iostream>
#include <vector>

#include "AudioPlayer.hpp"

AudioPlayer* audioPlayer;

void callback() {
	std::cout << std::boolalpha;
	std::cout << "Index       : " << audioPlayer->state->index << std::endl;
	std::cout << "Position    : " << audioPlayer->state->position << std::endl;
	std::cout << "Duration    : " << audioPlayer->state->duration << std::endl;
	std::cout << "IsPlaying   : " << audioPlayer->state->isPlaying << std::endl;
	std::cout << "IsCompleted : " << audioPlayer->state->isCompleted << std::endl;
	std::cout << "IsValid     : " << audioPlayer->state->isValid << std::endl;
	std::cout << "Volume      : " << audioPlayer->state->volume << std::endl;
	std::cout << "Playlist    : " << "[ " << std::endl;
	int index = 0;
	for (Audio* audio : audioPlayer->state->audios->audios) {
		std::cout << "    " << index << ". " << audio->type << ", " << audio->resource << ", " << std::endl;
		index++;
	}
	std::cout << "]" << std::endl << std::endl;
}

int main(int argc, char** argv) {
	audioPlayer = new AudioPlayer();
	audioPlayer->on(callback);
	std::vector<Audio*> audios;
	for (int index = 0; index < argc; index++) {
		audios.emplace_back(
			Audio::file(argv[index])
		);
	}
	audioPlayer->open(
		new Playlist(audios)
	);
	std::cin.get();
	audioPlayer->seek(175000);
	std::cin.get();
	return 0;
}