#include "internal/setters.hpp"


const std::string __title__   = "flutter_vlc";
const std::string __version__ = "v0.0.1";
const std::string __author__  = "Hitesh Kumar Saini";
const std::string __github__  = "https://github.com/alexmercerind";
const std::string __email__   = "saini123hitesh@gmail.com";
const std::string __license__ = "LGPL v2.1";


class AudioPlayer: public AudioPlayerSetters {
public:
	AudioPlayer() {
		this->instance = VLC::Instance(0, nullptr);
		this->mediaPlayer = VLC::MediaPlayer(this->instance);
		this->mediaListPlayer = VLC::MediaListPlayer(this->instance);
		this->mediaList = VLC::MediaList(this->instance);
		this->mediaListPlayer.setMediaPlayer(this->mediaPlayer);
		this->state = new AudioPlayerState();
	}

	void onEvent(std::function<void(void)> callback) {
		this->onPlay(callback);
		this->onPause(callback);
		this->onStop(callback);
		this->onComplete(callback);
		this->onSeekable(
			[callback](bool _) -> void { callback(); }
		);
		this->onLoad(
			[callback](VLC::Media _) -> void { callback(); }
		);
		this->onPosition(
			[callback](int _) -> void { callback(); }
		);
	}

	void onException(std::function<void(void)> callback) {
		this->mediaPlayer.eventManager().onEncounteredError(callback);
	}
};
