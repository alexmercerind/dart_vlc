/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include <string>
#include <map>
#include <vector>

#include "mediasource.hpp"
#include "media.hpp"

#ifndef Playlist_HEADER
#define Playlist_HEADER


enum PlaylistMode {
	single,
	loop,
	repeat
};


class Playlist : public MediaSource {
public:
	std::vector<Media*> medias;
	PlaylistMode playlistMode;

	Playlist(std::vector<Media*> medias, PlaylistMode playlistMode = PlaylistMode::single) {
		this->medias = medias;
		this->playlistMode = playlistMode;
	};

	std::vector<std::map<std::string, std::string>> get() {
		std::vector<std::map<std::string, std::string>> _medias;
		for (Media* audio : this->medias) {
			_medias.emplace_back(audio->get());
		}
		return _medias;
	}

	std::string mediaSourceType() {
		return "MediaSourceType.playlist";
	}
};


#endif
