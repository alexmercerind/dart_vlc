/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "getters.hpp"


class PlayerEvents : public PlayerGetters {
public:
	void onOpen(std::function<void(VLC::Media)> callback) {
		this->_openCallback = callback;
		this->mediaPlayer.eventManager().onMediaChanged(
			std::bind(&PlayerEvents::_onOpenCallback, this, std::placeholders::_1)
		);
	}

	void onPlay(std::function<void(void)> callback) {
		this->_playCallback = callback;
		this->mediaPlayer.eventManager().onPlaying(
			std::bind(&PlayerEvents::_onPlayCallback, this)
		);
	}

	void onPause(std::function<void(void)> callback) {
		this->_pauseCallback = callback;
		this->mediaPlayer.eventManager().onPaused(
			std::bind(&PlayerEvents::_onPauseCallback, this)
		);
	}

	void onStop(std::function<void(void)> callback) {
		this->_stopCallback = callback;
		this->mediaPlayer.eventManager().onStopped(
			std::bind(&PlayerEvents::_onStopCallback, this)
		);
	}

	void onPosition(std::function<void(int)> callback) {
		this->_positionCallback = callback;
		this->mediaPlayer.eventManager().onPositionChanged(
			std::bind(&PlayerEvents::_onPositionCallback, this, std::placeholders::_1)
		);
	}

	void onSeekable(std::function<void(bool)> callback) {
		this->_seekableCallback = callback;
		this->mediaPlayer.eventManager().onSeekableChanged(
			std::bind(&PlayerEvents::_onSeekableCallback, this, std::placeholders::_1)
		);
	}

	void onComplete(std::function<void(void)> callback) {
		this->_completeCallback = callback;
		this->mediaPlayer.eventManager().onEndReached(
			std::bind(&PlayerEvents::_onCompleteCallback, this)
		);
	}

	void onVolume(std::function<void(float)> callback) {
		this->_volumeCallback = callback;
	}

	void onRate(std::function<void(float)> callback) {
		this->_rateCallback = callback;
	}

protected:
	std::function<void(VLC::Media)> _openCallback;

	void _onOpenCallback(VLC::MediaPtr media) {
		if (this->getDuration() > 0) {
			this->state->isPlaying = this->mediaPlayer.isPlaying();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->isCompleted = false;
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->state->index = this->mediaList.indexOfItem(*media.get());
			this->_openCallback(*media.get());
		}
		else {
			this->state->isPlaying = this->mediaPlayer.isPlaying();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->isCompleted = false;
			this->state->position = 0;
			this->state->duration = 0;
			this->state->index = this->mediaList.indexOfItem(*media.get());
			this->_openCallback(*media.get());
		}
	}

	std::function<void(void)> _playCallback;

	void _onPlayCallback() {
		if (this->getDuration() > 0) {
			this->state->isPlaying = this->mediaPlayer.isPlaying();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->isCompleted = false;
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->_playCallback();
		}
	}

	std::function<void(void)> _pauseCallback;

	void _onPauseCallback() {
		if (this->getDuration() > 0) {
			this->state->isPlaying = this->mediaPlayer.isPlaying();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->_pauseCallback();
		}
	}

	std::function<void(void)> _stopCallback;

	void _onStopCallback() {
		if (this->getDuration() > 0) {
			this->state->isPlaying = this->mediaPlayer.isPlaying();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->_stopCallback();
		}
	}

	std::function<void(int)> _positionCallback;

	void _onPositionCallback(float relativePosition) {
		if (this->getDuration() > 0) {
			this->state->isPlaying = this->mediaPlayer.isPlaying();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->_positionCallback(
				static_cast<int>(relativePosition * this->mediaPlayer.length())
			);
		}
	}

	std::function<void(bool)> _seekableCallback;

	void _onSeekableCallback(bool isSeekable) {
		if (this->getDuration() > 0) {
			this->state->isSeekable = isSeekable;
			this->_seekableCallback(isSeekable);
		}
	}

	std::function<void(void)> _completeCallback;

	void _onCompleteCallback() {
		if (this->getDuration() > 0) {
			this->state->isPlaying = this->mediaPlayer.isPlaying();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->isCompleted = true;
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->_completeCallback();
		}
	}

	std::function<void(float)> _volumeCallback;

	std::function<void(float)> _rateCallback;
};
