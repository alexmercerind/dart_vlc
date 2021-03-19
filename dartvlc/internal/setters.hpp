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
#include "../device.hpp"
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
		this->state->isPlaying = false;
		this->state->position = 0;
		this->state->duration = 0;
    }

	void next() {
		/*
		 * Intentionally not using `this->mediaListPlayer.back` to get `this->state->index` changed.
		 */
		this->_onPlaylistCallback(true);
		if (this->state->index < this->mediaList.count())
			this->mediaListPlayer.playItemAtIndex(
				++this->state->index
			);
	}

	void back() {
		/*
		 * Intentionally not using `this->mediaListPlayer.next` to get `this->state->index` changed.
		 */
		this->_onPlaylistCallback(true);
		if (this->state->index > 0)
			this->mediaListPlayer.playItemAtIndex(
				--this->state->index
			);
	}

	void jump(int index) {
		this->_onPlaylistCallback(true);
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
		this->mediaPlayer.outputDeviceSet(device->id);
	}

	void setPlaylistMode(int mode) {
		this->mediaListPlayer.setPlaybackMode(
			static_cast<libvlc_playback_mode_t>(mode)
		);
	}

	void add(Media* media) {
		this->isPlaylistModified = true;
		/* Append the `Media` to `Playlist`. */
		this->state->medias->medias.emplace_back(media);
		VLC::Media _ = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
		this->mediaList.addMedia(_);
		this->_onPlaylistCallback(false);
		this->state->isPlaylist = true;
	}

	void remove(int index) {
		if (index < 0 || index >= this->state->medias->medias.size()) return;
		this->isPlaylistModified = true;
		/* Update the `Media` `Playlist`. */
		this->state->medias->medias.erase(state->medias->medias.begin() + index);
		this->mediaList.removeIndex(index);
		this->_onPlaylistCallback(false);
		/* Handling if the index is same as current `Media` index. */
		if (!this->state->isCompleted && this->state->index == index) {
			/* Stop the `Player` if the remove index was the last. */
			if (this->state->index == this->mediaList.count()) {
				this->mediaListPlayer.stop();
			}
			/* If the remove index is same as the current index, then start playing the `Media` at the same index (since it is now the next `Media` in the playlist after removal). */
			else
				this->jump(index);
		}
		/* Decrement the current index if it falls behind current `Media` index. */
		if (this->state->index > index)
			this->state->index--;
		this->state->isPlaylist = true;
	}

	void insert(int index, Media* media) {
		if (index < 0 || index >= this->state->medias->medias.size()) return;
		this->isPlaylistModified = true;
		/* Update the `Media` `Playlist`. */
		this->state->medias->medias.insert(
			state->medias->medias.begin() + index,
			media
		);
		VLC::Media _ = VLC::Media(this->instance, media->location, VLC::Media::FromLocation);
		this->mediaList.insertMedia(_, index);
		this->_onPlaylistCallback(false);
		/* If the insertion index falls behind the current `Media` index, increment current index by 1. */
		if (this->state->index <= index)
			this->state->index++;
		this->state->isPlaylist = true;
	}

	void move(int initial, int final) {
		if (initial < 0 || initial >= this->state->medias->medias.size() || final < 0 || final >= this->state->medias->medias.size()) return;
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
		/* If `initial` & `final` lie on the either side of the `this->state->index` */
		if (initial != this->state->index && final != this->state->index) {
			if (initial > final)
				this->state->index++;
			else
				this->state->index--;
			this->_onPlaylistCallback(false);
		}
		else if (initial == this->state->index) {
			/* If the moving `Media` is the one that is playing currently, then just change the current index. */
			this->state->index = final;
			this->_onPlaylistCallback(false);
		}
		else if (final == this->state->index) {
			/* If the final index is same as the currently playing index, then stop current `Media` and play the moved one. */
			this->state->index++;
			this->_onPlaylistCallback(false);
		}
	}
};
