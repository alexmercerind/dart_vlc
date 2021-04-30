/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
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

	void onPlaylist(std::function<void(void)> callback) {
		this->_playlistCallback = callback;
	}

	void onVideo(std::function<void(uint8_t* frame)> callback) {
		this->_videoCallback = callback;
		int pitch = this->videoWidth * 4;
		int size = this->videoHeight * pitch;
		this->_videoFrameBuffer = new uint8_t[size];
		this->mediaPlayer.setVideoCallbacks(
			std::bind(&PlayerEvents::_videoLockCallback, this, std::placeholders::_1),
			nullptr,
			std::bind(&PlayerEvents::_videoPictureCallback, this, std::placeholders::_1)
		);
		this->mediaPlayer.setVideoFormatCallbacks(
			[=](char* chroma, uint32_t* w, uint32_t* h, uint32_t* p, uint32_t* l) -> int {
				strcpy(chroma, "RV32");
				*w = this->videoWidth;
				*h = this->videoHeight;
				*p = pitch;
				*l = this->videoHeight;
				return 1;
			},
			nullptr
		);
		this->mediaPlayer.setVideoFormat("RV32", this->videoWidth, this->videoHeight, pitch);
	}

protected:

	std::function<void(void)> _playlistCallback = [=]() -> void {};

	void _onPlaylistCallback() {
		if (this->isPlaylistModified) {
			this->mediaListPlayer.setMediaList(this->mediaList);
			if (!this->mediaList.count()) {
				this->state = new PlayerState();
				this->mediaListPlayer.stop();
				return;
			}
			if (this->state->index > this->mediaList.count())
				this->state->index = this->mediaList.count() - 1;
			this->isPlaylistModified = false;
			this->_playlistCallback();
		};
	}

	std::function<void(VLC::Media)> _openCallback = [=](VLC::Media media) -> void {};

	void _onOpenCallback(VLC::MediaPtr media) {
		this->state->isPlaying = this->mediaPlayer.isPlaying();
		this->state->isValid = this->mediaPlayer.isValid();
		if (this->getDuration() > 0) {
			this->state->isCompleted = false;
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
		}
		else {
			this->state->isCompleted = false;
			this->state->position = 0;
			this->state->duration = 0;
		}
		this->state->index = this->mediaList.indexOfItem(*media.get());
		this->_openCallback(*media.get());
	}

	std::function<void(void)> _playCallback = [=]() -> void {};

	void _onPlayCallback() {
		this->state->isPlaying = this->mediaPlayer.isPlaying();
		if (this->getDuration() > 0) {
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->isCompleted = false;
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
		}
		this->_playCallback();
	}

	std::function<void(void)> _pauseCallback = [=]() -> void {};

	void _onPauseCallback() {
		this->state->isPlaying = this->mediaPlayer.isPlaying();
		if (this->getDuration() > 0) {
			this->state->position = this->getPosition();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->duration = this->getDuration();
		}
		this->_pauseCallback();
	}

	std::function<void(void)> _stopCallback = [=]() -> void {};

	void _onStopCallback() {
		this->state->isPlaying = this->mediaPlayer.isPlaying();
		this->state->isValid = this->mediaPlayer.isValid();
		this->state->position = 0;
		this->state->duration = 0;
		this->_stopCallback();
	}

	std::function<void(int)> _positionCallback = [=](int position) -> void {};

	void _onPositionCallback(float relativePosition) {
		this->state->isPlaying = this->mediaPlayer.isPlaying();
		if (this->getDuration() > 0) {
			this->state->position = this->getPosition();
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->duration = this->getDuration();
		}
		this->_positionCallback(
			static_cast<int>(relativePosition * this->mediaPlayer.length())
		);
	}

	std::function<void(bool)> _seekableCallback = [=](bool isSeekable) -> void {};

	void _onSeekableCallback(bool isSeekable) {
		if (this->getDuration() > 0) {
			this->state->isSeekable = isSeekable;
			this->_seekableCallback(isSeekable);
		}
	}

	std::function<void(void)> _completeCallback = [=]() -> void {};

	void _onCompleteCallback() {
		this->state->isPlaying = this->mediaPlayer.isPlaying();
		if (this->getDuration() > 0) {
			this->state->isValid = this->mediaPlayer.isValid();
			this->state->isCompleted = true;
			this->state->position = this->getPosition();
			this->state->duration = this->getDuration();
			this->_onPlaylistCallback();
			this->_completeCallback();
		} else {
			this->state->position = 0;
			this->state->duration = 0;
		}
	}

	std::function<void(float)> _volumeCallback = [=](float volume) -> void {};

	std::function<void(float)> _rateCallback = [=](float rate) -> void {};

	std::function<void(uint8_t* frame)> _videoCallback = [=](uint8_t* frame) -> void {};

	uint8_t* _videoFrameBuffer;

	void* _videoLockCallback(void** planes) {
		planes[0] = static_cast<void*>(this->_videoFrameBuffer);
		return nullptr;
	}

	void _videoPictureCallback(void* picture) {
		this->_videoCallback(static_cast<uint8_t*>(this->_videoFrameBuffer));
	}
};
