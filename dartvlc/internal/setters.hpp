/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "events.hpp"
#include "../device.hpp"
#include "../mediasource/mediasource.hpp"
#include "../mediasource/media.hpp"
#include "../mediasource/playlist.hpp"


class PlayerSetters: public PlayerEvents {
public:
	void open(MediaSource* mediaSource, bool autoStart = true) {
		/* Freed the previous `Media` objects when a new `Playlist` or `Media` is opened. */
		this->state->isStarted = false;
		this->stop();
		for (Media* media: this->state->medias->medias) {
			delete media;
		}
		this->state->medias->medias = {};
		this->mediaList = VLC::MediaList(this->instance);
		if (mediaSource->mediaSourceType() == "MediaSourceType.media") {
			Media* media = dynamic_cast<Media*>(mediaSource);
			VLC::Media _ = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
			this->mediaList.addMedia(_);
			this->mediaListPlayer.setMediaList(this->mediaList);
			this->state->medias->medias = { media };
			this->state->isPlaylist = false;
		}
		else if (mediaSource->mediaSourceType() == "MediaSourceType.playlist") {
			Playlist* playlist = dynamic_cast<Playlist*>(mediaSource);
			if (playlist->medias.empty())
				return;
			for (Media* media : playlist->medias) {
				VLC::Media _ = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
				this->mediaList.addMedia(_);
			}
			this->mediaListPlayer.setMediaList(this->mediaList);
			this->state->medias->medias = playlist->medias;
			this->state->isPlaylist = true;
		}
		this->_onOpenCallback(this->mediaList.itemAtIndex(0));
		this->state->index = 0;
		this->state->isPlaying = this->mediaListPlayer.isPlaying();
		this->state->isValid = this->mediaListPlayer.isValid();
		if (autoStart) this->play();
	}

	void play() {
		if (!this->state->isStarted) {
			this->mediaListPlayer.playItemAtIndex(0);
			this->state->isStarted = true;
		}
        else {
			this->mediaListPlayer.play();
		}
	}

    void pause() {
		if(this->mediaListPlayer.isPlaying()){
        	this->mediaListPlayer.pause();
		}
    }

	void playOrPause() {
		if (!this->state->isStarted) {
			this->mediaListPlayer.playItemAtIndex(0);
			this->state->isStarted = true;
		}
		else {
			this->mediaListPlayer.pause();
		}
	}

    void stop() {
        this->mediaListPlayer.stop();
    }

	void next() {
		this->_onPlaylistCallback();
		if (this->state->index < this->mediaList.count())
			this->mediaListPlayer.playItemAtIndex(
				++this->state->index
			);
	}

	void back() {
		this->_onPlaylistCallback();
		if (this->state->index > 0)
			this->mediaListPlayer.playItemAtIndex(
				--this->state->index
			);
	}

	void jump(int index) {
		this->_onPlaylistCallback();
		if (index >= 0 && index < this->mediaList.count())
			this->mediaListPlayer.playItemAtIndex(index);
	}

	void seek(int position) {
		float relativePosition = static_cast<float>(position) / static_cast<float>(this->getDuration());
		this->mediaPlayer.setPosition(relativePosition);
	}

	void setVolume(float volume) {
		this->mediaPlayer.setVolume(
				static_cast<int>(volume * 100)
		);
		this->state->volume = volume;
		this->_volumeCallback(volume);
	}

	void setRate(float rate) {
		this->mediaPlayer.setRate(rate);
		this->state->rate = rate;
		this->_rateCallback(rate);
	}

	void setDevice(Device* device) {
		this->state->device = device->id != "" ? device: nullptr;
		this->mediaPlayer.outputDeviceSet(device->id);
	}

	void setPlaylistMode(PlaylistMode mode) {
		this->mediaListPlayer.setPlaybackMode(
			static_cast<libvlc_playback_mode_t>(mode)
		);
	}

	void setEqualizer(Equalizer* equalizer) {
		this->mediaPlayer.setEqualizer(equalizer->equalizer);
	}

	void setUserAgent(std::string userAgent) {
		this->instance.setUserAgent("Dart VLC", userAgent);
	}

	void add(Media* media) {
		this->isPlaylistModified = true;
		this->state->medias->medias.emplace_back(media);
		VLC::Media _ = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
		this->mediaList.addMedia(_);
		this->_onPlaylistCallback();
		this->state->isPlaylist = true;
	}

	void remove(int index) {
		if (index < 0 || index >= this->state->medias->medias.size()) return;
		this->isPlaylistModified = true;
		this->state->medias->medias.erase(state->medias->medias.begin() + index);
		this->mediaList.removeIndex(index);
		this->_onPlaylistCallback();
		if (!this->state->isCompleted && this->state->index == index) {
			if (this->state->index == this->mediaList.count()) {
				this->mediaListPlayer.stop();
			}
			else
				this->jump(index);
		}
		if (this->state->index > index)
			this->state->index--;
		this->state->isPlaylist = true;
	}

	void insert(int index, Media* media) {
		if (index < 0 || index >= this->state->medias->medias.size()) return;
		this->isPlaylistModified = true;
		this->state->medias->medias.insert(
			state->medias->medias.begin() + index,
			media
		);
		VLC::Media _ = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
		this->mediaList.insertMedia(_, index);
		this->_onPlaylistCallback();
		if (this->state->index <= index)
			this->state->index++;
		this->state->isPlaylist = true;
	}

	void move(int initial, int final) {
		if (
			initial < 0 ||
			initial >= this->state->medias->medias.size() ||
			final < 0 ||
			final >= this->state->medias->medias.size()
		) return;
		if (initial == final) return;
		this->isPlaylistModified = true;
		Media* _ = this->state->medias->medias[initial];
		VLC::Media __ = VLC::Media(this->instance, this->mediaList.itemAtIndex(initial).get()->mrl(), VLC::Media::FromLocation);
		this->state->medias->medias.erase(
			this->state->medias->medias.begin() + initial
		);
		this->mediaList.removeIndex(initial);
		this->state->medias->medias.insert(
			this->state->medias->medias.begin() + final,
			_
		);
		this->mediaList.insertMedia(__, final);
		if (initial == this->state->index) {
			this->state->index = final;
		}
		else if (final == this->state->index) {
			this->state->index++;
		}
		else if (!((initial < this->state->index && final < this->state->index) || (initial > this->state->index && final > this->state->index))) {
			if (initial > final)
				this->state->index++;
			else
				this->state->index--;
		}
		this->_onPlaylistCallback();
	}
};
