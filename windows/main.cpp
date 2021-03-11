#include <iostream>
#include <vector>

#include "AudioPlayer.hpp"


class AudioPlayers {
public:
	AudioPlayer* get(int id) {
		if (this->audioPlayers.find(id) == this->audioPlayers.end()) {
			this->audioPlayers[id] = new AudioPlayer();
		}
		return this->audioPlayers[id];
	}

private:
	std::map<int, AudioPlayer*> audioPlayers;
};

AudioPlayers* audioPlayers = new AudioPlayers();

int main(int argc, char** argv) {
	AudioPlayer* audioPlayer = new AudioPlayer();
	audioPlayer->onEvent([audioPlayer] () -> void {
		std::cout << std::boolalpha;
		std::cout << "index       : " << audioPlayer->state->index << std::endl;
		std::cout << "position    : " << audioPlayer->state->position << std::endl;
		std::cout << "duration    : " << audioPlayer->state->duration << std::endl;
		std::cout << "isPlaying   : " << audioPlayer->state->isPlaying << std::endl;
		std::cout << "isSeekable  : " << audioPlayer->state->isSeekable << std::endl;
		std::cout << "isCompleted : " << audioPlayer->state->isCompleted << std::endl;
		std::cout << "isValid     : " << audioPlayer->state->isValid << std::endl;
		std::cout << "volume      : " << audioPlayer->state->volume << std::endl;
		std::cout << "playlist    : " << "[ " << std::endl;
		int index = 0;
		for (Audio* audio : audioPlayer->state->audios->audios) {
			std::cout << "    " << index << ". " << audio->type << ", " << audio->resource << ", " << std::endl;
			index++;
		}
		std::cout << "]" << std::endl << std::endl;
	});
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
	return 0;
}
