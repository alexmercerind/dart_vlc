#include "internal/events.hpp"


class AudioPlayer: public AudioPlayerEvents {
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
