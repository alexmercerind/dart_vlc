#include "setters.hpp"


class AudioPlayerEvents : public AudioPlayerSetters {
public:
	void onLoad(std::function<void(VLC::Media)> callback) {
		this->_loadCallback = callback;
		this->mediaPlayer.eventManager().onMediaChanged(
			std::bind(&AudioPlayerEvents::_onLoadCallback, this, std::placeholders::_1)
		);
	}

	void onPlay(std::function<void(void)> callback) {
		this->_playCallback = callback;
		this->mediaPlayer.eventManager().onPlaying(
			std::bind(&AudioPlayerEvents::_onPlayCallback, this)
		);
	}

	void onPause(std::function<void(void)> callback) {
		this->_pauseCallback = callback;
		this->mediaPlayer.eventManager().onPaused(
			std::bind(&AudioPlayerEvents::_onPauseCallback, this)
		);
	}

	void onStop(std::function<void(void)> callback) {
		this->_stopCallback = callback;
		this->mediaPlayer.eventManager().onStopped(
			std::bind(&AudioPlayerEvents::_onStopCallback, this)
		);
	}

	void onPosition(std::function<void(int)> callback) {
		this->_positionCallback = callback;
		this->mediaPlayer.eventManager().onPositionChanged(
			std::bind(&AudioPlayerEvents::_onPositionCallback, this, std::placeholders::_1)
		);
	}

	void onVolume(std::function<void(float)> callback) {
		this->_volumeCallback = callback;
		this->mediaPlayer.eventManager().onAudioVolume(
			std::bind(&AudioPlayerEvents::_onVolumeCallback, this, std::placeholders::_1)
		);
	}

	void onSeekable(std::function<void(bool)> callback) {
		this->_seekableCallback = callback;
		this->mediaPlayer.eventManager().onSeekableChanged(
			std::bind(&AudioPlayerEvents::_onSeekableCallback, this, std::placeholders::_1)
		);
	}

	void onComplete(std::function<void(void)> callback) {
		this->_completeCallback = callback;
		this->mediaPlayer.eventManager().onEndReached(
			std::bind(&AudioPlayerEvents::_onCompleteCallback, this)
		);
	}

private:
	std::function<void(VLC::Media)> _loadCallback;

	void _onLoadCallback(VLC::MediaPtr media) {
		if (this->getDuration() > 0) {
			this->state->isPlaying = this->mediaPlayer.isPlaying();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->isCompleted = false;
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->_loadCallback(*media.get());
		}
		else {
			this->state->index = this->mediaList.indexOfItem(*media.get());
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
			this->state->isCompleted = false;
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
			this->state->isCompleted = false;
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
			this->state->isCompleted = false;
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->_positionCallback(
				static_cast<int>(relativePosition * this->mediaPlayer.length())
			);
		}
	}

	std::function<void(float)> _volumeCallback;

	void _onVolumeCallback(int volume) {
		if (this->getDuration() > 0) {
			this->state->volume = static_cast<float>(volume) / 100.0f;
			this->_volumeCallback(this->state->volume);
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
};
