/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "../include/vlcpp/vlc.hpp"
#include "../device.hpp"


class PlayerInternal {
protected:
	VLC::Instance instance;
	VLC::MediaPlayer mediaPlayer;
	VLC::MediaListPlayer mediaListPlayer;
	VLC::MediaList mediaList;
	bool isPlaylistModified = false;
	Device* device = nullptr;
};
