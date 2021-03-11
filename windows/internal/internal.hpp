#include "../include/vlcpp/vlc.hpp"


class AudioPlayerInternal {
protected:
	VLC::Instance instance;
	VLC::MediaPlayer mediaPlayer;
	VLC::MediaListPlayer mediaListPlayer;
	VLC::MediaList mediaList;
};
