/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "internal.hpp"
#include "state.hpp"


class PlayerGetters: protected PlayerInternal {
public:
	PlayerState* state;
	int videoWidth = 0;
	int videoHeight = 0;

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
