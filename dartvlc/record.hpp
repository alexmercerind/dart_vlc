/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
*/

#include <string>
#include <sstream>

#include "mediasource/media.hpp"

#ifndef Record_HEADER
#define Record_HEADER

class Record {
public:
    int id;
    Media* media;
    std::string savingFile;

    Record(int id, Media* media, std::string savingFile) {
        this->id = id;
        this->media = media;
        this->savingFile = savingFile;
        this->instance = VLC::Instance(0, nullptr);
    }

    void start() {
        std::stringstream sout;
        sout << "#std{access=file,mux=raw,dst=" << this->savingFile << "}";
        libvlc_vlm_add_broadcast(
            this->instance.get(),
            this->media->location.c_str(),
            this->media->location.c_str(),
            sout.str().c_str(),
            0,
            nullptr,
            true,
            false
        );
        libvlc_vlm_play_media(
            this->instance.get(),
            this->media->location.c_str()
        );
    }

    void dispose() {
        libvlc_vlm_release(this->instance.get());
    }

private:
    VLC::Instance instance;
};


class Records {
public:
	Record* get(int id, Media* media, std::string savingFile) {
		if (this->records.find(id) == this->records.end()) {
			this->records[id] = new Record(id, media, savingFile);
		}
		return this->records[id];
	}

private:
	std::map<int, Record*> records;
};


Records* records = new Records();

#endif