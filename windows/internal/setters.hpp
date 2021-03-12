#include "getters.hpp"
#include "../audiosource/audiosource.hpp"
#include "../audiosource/audio.hpp"
#include "../audiosource/playlist.hpp"


class AudioPlayerSetters: public AudioPlayerGetters {
public:
	void open(AudioSource* audioSource) {
		this->stop();
		this->state->audios = new Playlist({});
		this->mediaList = VLC::MediaList(this->instance);
		if (audioSource->audioSourceType() == "AudioSourceType.audio") {
			Audio* audio = dynamic_cast<Audio*>(audioSource);
			VLC::Media media = VLC::Media(this->instance, audio->location, VLC::Media::FromLocation);
			this->mediaList.addMedia(media);
			this->mediaListPlayer.setMediaList(this->mediaList);
			this->mediaListPlayer.playItemAtIndex(0);
			this->state->audios = new Playlist({ audio });
			this->state->index = 0;
			this->state->isPlaying = this->mediaListPlayer.isPlaying();
			this->state->isValid = this->mediaListPlayer.isValid();
			this->state->isPlaylist = false;

		}
		else if (audioSource->audioSourceType() == "AudioSourceType.playlist") {
			Playlist* playlist = dynamic_cast<Playlist*>(audioSource);
			for (Audio* audio : playlist->audios) {
				VLC::Media media = VLC::Media(this->instance, audio->location, VLC::Media::FromLocation);
				this->mediaList.addMedia(media);
			}
			this->mediaListPlayer.setMediaList(this->mediaList);
			this->mediaListPlayer.playItemAtIndex(playlist->start);
			this->state->audios = playlist;
			this->state->index = playlist->start;
			this->state->isPlaying = this->mediaListPlayer.isPlaying();
			this->state->isValid = this->mediaListPlayer.isValid();
			this->state->isPlaylist = true;
		}
	}

	void play() {
        this->mediaListPlayer.play();
	}

    void pause() {
        this->mediaListPlayer.pause();
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
	}

	void setRate(float rate) {
		this->mediaPlayer.setRate(rate);
		this->state->rate = rate;
	}
};
