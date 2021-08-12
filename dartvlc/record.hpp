/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
*/

#ifndef Record_HEADER
#define Record_HEADER

#include <memory>
#include <string>
#include <sstream>

#include "mediasource/media.hpp"

class Record {
public:
    int id;
    std::string savingFile;

    Record(int id, std::unique_ptr<Media> media, std::string savingFile) : media_(std::move(media)) {
        id = id;
        savingFile = savingFile;
        instance_ = VLC::Instance(0, nullptr);
    }

    void start() {
        std::stringstream sout;
        sout << "#std{access=file,mux=raw,dst=" << savingFile << "}";
        libvlc_vlm_add_broadcast(
            instance_.get(),
            media_->location.c_str(),
            media_->location.c_str(),
            sout.str().c_str(),
            0,
            nullptr,
            true,
            false
        );
        libvlc_vlm_play_media(
            instance_.get(),
            media_->location.c_str()
        );
    }

    ~Record() {
        libvlc_vlm_release(instance_.get());
    }

private:
    VLC::Instance instance_;
    std::unique_ptr<Media> media_;
};


class Records {
public:
    // TODO: The id should be determined automatically.
    Record* create(int id, std::unique_ptr<Media> media, std::string savingFile) {
		if (records_.find(id) == records_.end()) {
			records_[id] = std::make_unique<Record>(id, std::move(media), savingFile);
		}
		return records_[id].get();
    }

	Record* get(int id) {
        auto it = records_.find(id);
        if(it != records_.end()) {
            return it->second.get();
        }
        return nullptr;
	}

    void dispose(int id, std::function<void()> callback = []() -> void {}) {
        records_.erase(id);
        callback();
    }

private:
	std::map<int, std::unique_ptr<Record>> records_;
};


Records* records = new Records();

#endif