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
#include "../devices.hpp"
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
			if (playlist->medias.empty())
				return;
			for (Media* vlcMedia : playlist->medias) {
				VLC::Media media = VLC::Media(this->instance, vlcMedia->location, VLC::Media::FromLocation);
				this->mediaList.addMedia(media);
			}
			this->mediaListPlayer.setMediaList(this->mediaList);
			this->state->medias = playlist;
			this->state->isPlaylist = true;
		}
		this->_onOpenCallback(this->mediaList.itemAtIndex(0));
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
		/*
		 * Intentionally not using `this->mediaListPlayer.back` to get `this->state->index` changed.
		 */
		this->mediaListPlayer.playItemAtIndex(
			++this->state->index
		);
	}

	void back() {
		/*
		 * Intentionally not using `this->mediaListPlayer.next` to get `this->state->index` changed.
		 */
		this->mediaListPlayer.playItemAtIndex(
			--this->state->index
		);
	}

	void jump(int index) {
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
		this->mediaPlayer.outputDeviceSet(device->name);
	}

	void setPlaylistMode(int mode) {
		this->mediaListPlayer.setPlaybackMode(
			static_cast<libvlc_playback_mode_t>(mode)
		);
	}

	void add(Media* media) {
		isPlaylistModified = true;
		/* Append the `Media` to `Playlist`. */
		this->state->medias->medias.emplace_back(media);
		VLC::Media vlcMedia = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
		this->mediaList.addMedia(vlcMedia);
		/* Notify changes & play if completion was done. */
		this->_onPlaylistCallback(this->state->isCompleted);
		this->state->isPlaylist = true;
	}

	void remove(int index) {
		isPlaylistModified = true;
		/* Update the `Media` `Playlist`. */
		this->state->medias->medias.erase(state->medias->medias.begin() + index);
		this->mediaList.removeIndex(index);
		/* Notify changes & play if completion was done. */
		this->_onPlaylistCallback(this->state->isCompleted);
		/* Handling if the index is same as current `Media` index. */
		if (!this->state->isCompleted && this->state->index == index) {
			/* Stop the `Player` if the remove index was the last. */
			if (this->state->index == this->mediaList.count()) {
				this->mediaListPlayer.stop();
				this->state->isPlaying = false;
				this->state->position = 0;
				this->state->duration = 0;
			}
			/* If the remove index is same as the current index, then start playing the `Media` at the same index (since, it is now the next `Media` in the playlist after removal.) */
			else
				this->jump(index);
		}
		/* Decrement the current index if it falls behind current `Media` index. */
		if (this->state->index > index)
			this->state->index--;
	}

	void insert(int index, Media* media) {
		isPlaylistModified = true;
		/* Update the `Media` `Playlist`. */
		this->state->medias->medias.insert(
			state->medias->medias.begin() + index,
			media
		);
		VLC::Media vlcMedia = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
		this->mediaList.insertMedia(vlcMedia, index);
		/* Notify changes & play if completion was done. */
		this->_onPlaylistCallback(this->state->isCompleted);
		this->state->isPlaylist = true;
		/* If the insertion index falls behind the current `Media` index, increment current index by 1. */
		if (this->state->index <= index)
			this->state->index++;
	}
};
