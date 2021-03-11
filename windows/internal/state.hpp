#include "audiosource/playlist.hpp"


class AudioPlayerState {
public:
	int index = 0;
	Playlist* audios = new Playlist({});
	bool isPlaying = false;
	bool isValid = true;
	bool isCompleted = false;
	int position = 0;
	int duration = 0;
	double volume = 1.0;
};
