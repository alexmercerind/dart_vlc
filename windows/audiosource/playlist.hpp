#include <string>
#include <map>
#include <vector>

#include "audiosource.hpp"
#include "audio.hpp"

#ifndef Playlist_HEADER
#define Playlist_HEADER


class Playlist : public AudioSource {
public:
	int start;
	std::vector<Audio*> audios;

	Playlist(std::vector<Audio*> audios, int start = 0) {
		this->audios = audios;
		this->start = start;
	};

	std::vector<std::map<std::string, std::string>> get() {
		std::vector<std::map<std::string, std::string>> _audios;
		for (Audio* audio : this->audios) {
			_audios.emplace_back(audio->get());
		}
		return _audios;
	}

	std::string what() {
		return "playlist";
	}
};


#endif
