#include "internal.hpp"
#include "state.hpp"


class AudioPlayerGetters: protected AudioPlayerInternal {
public:
	AudioPlayerState* state;

	int getDuration() {
		return static_cast<int>(
			this->mediaPlayer.length()
		);
	}

	int getPosition() {
		return static_cast<int>(
			this->mediaPlayer.length() * this->mediaPlayer.position()
		);
	}

	float getVolume() {
		float volume = this->mediaPlayer.volume() / 100.0f;
		return volume;
	}

	float getRate() {
		return this->mediaPlayer.rate();
	}

	bool isPlaying() {
		return this->mediaPlayer.isPlaying();
	}

	bool isPaused() {
		return !this->mediaPlayer.isPlaying();
	}
};
