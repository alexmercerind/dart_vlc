/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#include <string>
#include <map>
#include <filesystem>

#include <vlcpp/vlc.hpp>

#include "mediasource.hpp"

#ifndef Media_HEADER
#define Media_HEADER


class Media: public MediaSource {
public:
	int id;
	std::string mediaType;
	std::string resource;
	std::string location;
	std::map<std::string, std::string> metas;

	static Media* file(int id, std::string path, bool parse = false, int timeout = 10000) {
		Media* media = new Media();
		media->id = id;
		media->resource = path;
		media->location = "file:///" + path;
		media->mediaType = "MediaType.file";
		if (parse) media->parse(timeout);
		return media;
	}

	static Media* network(int id, std::string url, bool parse = false, int timeout = 10000) {
		Media* media = new Media();
		media->id = id;
		media->resource = url;
		media->location = url;
		media->mediaType = "MediaType.network";
		if (parse) media->parse(timeout);
		return media;
	}

	static Media* asset(int id, std::string path, bool parse = false, int timeout = 10000) {
		Media* media = new Media();
		media->id = id;
		media->resource = path;
		media->location = "file:///" + std::filesystem::temp_directory_path().u8string() + path;
		media->mediaType = "MediaType.asset";
		if (parse) media->parse(timeout);
		return media;
	}

	void parse(int timeout) {
		bool isParsed = false;
		bool* isParsedPointer = &isParsed;
		VLC::Instance __ = VLC::Instance(0, nullptr);
		VLC::Media ___ = VLC::Media(__, this->location, VLC::Media::FromLocation);
		___.eventManager().onParsedChanged(
			[=](VLC::Media::ParsedStatus status) mutable {
				*isParsedPointer = true;
			}
		);
		___.parseWithOptions(
			VLC::Media::ParseFlags::Network,
			timeout
		);
		while (!*isParsedPointer)
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		this->metas["title"]       = ___.meta(libvlc_meta_Title);
		this->metas["artist"]      = ___.meta(libvlc_meta_Artist);
		this->metas["genre"]       = ___.meta(libvlc_meta_Genre);
		this->metas["copyright"]   = ___.meta(libvlc_meta_Copyright);
		this->metas["trackNumber"] = ___.meta(libvlc_meta_TrackNumber);
		this->metas["description"] = ___.meta(libvlc_meta_Description);
		this->metas["rating"]      = ___.meta(libvlc_meta_Rating);
		this->metas["date"]        = ___.meta(libvlc_meta_Date);
		this->metas["settings"]    = ___.meta(libvlc_meta_Setting);
		this->metas["url"]         = ___.meta(libvlc_meta_URL);
		this->metas["language"]    = ___.meta(libvlc_meta_Language);
		this->metas["nowPlaying"]  = ___.meta(libvlc_meta_NowPlaying);
		this->metas["encodedBy"]   = ___.meta(libvlc_meta_EncodedBy);
		this->metas["artworkUrl"]  = ___.meta(libvlc_meta_ArtworkURL);
		this->metas["trackTotal"]  = ___.meta(libvlc_meta_TrackTotal);
		this->metas["director"]    = ___.meta(libvlc_meta_Director);
		this->metas["season"]      = ___.meta(libvlc_meta_Season);
		this->metas["episode"]     = ___.meta(libvlc_meta_Episode);
		this->metas["actors"]      = ___.meta(libvlc_meta_Actors);
		this->metas["albumArtist"] = ___.meta(libvlc_meta_AlbumArtist);
		this->metas["discNumber"]  = ___.meta(libvlc_meta_DiscNumber);
		this->metas["discTotal"]   = ___.meta(libvlc_meta_DiscTotal);
		this->metas["duration"]    = std::to_string(___.duration());
	}

	std::string mediaSourceType() {
		return "MediaSourceType.media";
	}

	std::map<std::string, std::string> get() {
		std::map<std::string, std::string> media;
		media["id"] = this->id;
		media["mediaType"] = this->mediaType;
		media["resource"] = this->resource;
		return media;
	}
};


#endif
