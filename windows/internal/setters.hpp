/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include "events.hpp"
#include "../mediasource/mediasource.hpp"
#include "../mediasource/media.hpp"
#include "../mediasource/playlist.hpp"


class PlayerSetters: public PlayerEvents {
public:
	void open(MediaSource* mediaSource, bool autoStart = true) {
		this->stop();
		this->state->medias = new Playlist({});
		this->mediaList = VLC::MediaList(this->instance);
		if (mediaSource->mediaSourceType() == "MediaSourceType.media") {
			Media* media = dynamic_cast<Media*>(mediaSource);
			VLC::Media vlcMedia = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
			this->mediaList.addMedia(vlcMedia);
			this->mediaListPlayer.setMediaList(this->mediaList);
			this->state->medias = new Playlist({ media });
			this->state->isPlaylist = false;

		}
		else if (mediaSource->mediaSourceType() == "MediaSourceType.playlist") {
			Playlist* playlist = dynamic_cast<Playlist*>(mediaSource);
			for (Media* vlcMedia : playlist->medias) {
				VLC::Media media = VLC::Media(this->instance, vlcMedia->location, VLC::Media::FromLocation);
				this->mediaList.addMedia(media);
			}
			this->mediaListPlayer.setMediaList(this->mediaList);
			this->state->medias = playlist;
			this->state->isPlaylist = true;
		}
		this->_onLoadCallback(this->mediaList.itemAtIndex(0));
		if (autoStart) {
			this->mediaListPlayer.playItemAtIndex(0);
			this->state->index = 0;
			this->state->isPlaying = this->mediaListPlayer.isPlaying();
			this->state->isValid = this->mediaListPlayer.isValid();
		}
	}

	void play() {
        this->mediaListPlayer.play();
	}

    void pause() {
        this->mediaListPlayer.pause();
    }

	void playOrPause() {
		if (this->mediaListPlayer.isPlaying())
			this->mediaListPlayer.pause();
		else
			this->mediaListPlayer.play();
	}

    void stop() {
        this->mediaListPlayer.stop();
    }

	void next() {
		this->mediaListPlayer.next();
	}

	void back() {
		this->mediaListPlayer.previous();
	}

	void jump(int index) {
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
	}

	void setRate(float rate) {
		this->mediaPlayer.setRate(rate);
		this->state->rate = rate;
	}
};
