/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "../include/vlcpp/vlc.hpp"


class PlayerInternal {
protected:
	VLC::Instance instance;
	VLC::MediaPlayer mediaPlayer;
	VLC::MediaListPlayer mediaListPlayer;
	VLC::MediaList mediaList;
	bool isPlaylistModified = false;
};
