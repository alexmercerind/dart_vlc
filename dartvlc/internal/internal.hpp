/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#ifdef _WIN32
#include <vlcpp/vlc.hpp>
#else
#include "vlcpp/vlc.hpp"
#endif

class PlayerInternal {
protected:
	VLC::Instance instance;
	VLC::MediaPlayer mediaPlayer;
	VLC::MediaListPlayer mediaListPlayer;
	VLC::MediaList mediaList;
	bool isPlaylistModified = false;
};
