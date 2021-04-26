/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "../mediasource/playlist.hpp"
#include "../device.hpp"


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
	float volume = 0.5;
	float rate = 1.0;
	bool isPlaylist = false;
	Device* device = nullptr;
};
