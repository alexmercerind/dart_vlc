/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "internal/setters.hpp"


class Player: public PlayerSetters {
public:
	Player(int id) {
		this->instance = VLC::Instance(0, nullptr);
		this->mediaPlayer = VLC::MediaPlayer(this->instance);
		this->mediaListPlayer = VLC::MediaListPlayer(this->instance);
		this->mediaList = VLC::MediaList(this->instance);
		this->mediaListPlayer.setMediaPlayer(this->mediaPlayer);
		this->state = new PlayerState();
		this->state->id = id;
	}

	void onEvent(std::function<void(void)> callback) {
		this->onPlay(callback);
		this->onPause(callback);
		this->onStop(callback);
		this->onComplete(callback);
		this->onSeekable(
			[callback](bool _) -> void { callback(); }
		);
		this->onOpen(
			[callback](VLC::Media _) -> void { callback(); }
		);
		this->onPosition(
			[callback](int _) -> void { callback(); }
		);
		this->onPlaylist(callback);
	}

	void onException(std::function<void(void)> callback) {
		this->mediaPlayer.eventManager().onEncounteredError(callback);
	}
};
