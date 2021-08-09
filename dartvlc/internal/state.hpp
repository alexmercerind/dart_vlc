/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "../mediasource/playlist.hpp"
#include "../device.hpp"
#include "../equalizer.hpp"


class PlayerState {
public:
	int id;
	int index = 0;
	Playlist* medias = new Playlist({}, PlaylistMode::single);
	bool isPlaying = false;
	bool isValid = true;
	bool isSeekable = true;
	bool isCompleted = false;
	int position = 0;
	int duration = 0;
	float volume = 1.0;
	float rate = 1.0;
	bool isPlaylist = false;
	Device* device = nullptr;
	/* TODO: Not used yet.
	Equalizer* equalizer = nullptr;
	*/

	void reset() {
		this->index = 0;
		this->medias->medias = {};
		this->isPlaying = false;
		this->isValid = true;
		this->isSeekable = true;
		this->isCompleted = false;
		this->position = 0;
		this->duration = 0;
		this->volume = 1.0;
		this->rate = 1.0;
		this->isPlaylist = false;
		this->device = nullptr;
		/*
		this->equalizer = nullptr;
		*/
	}

	~PlayerState() {
		delete this->medias;
	}
};
