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
#include <map>
#include <filesystem>
#include <future>

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

	static Media* directShow(int id, std::string resource) {
		Media* media = new Media();
		media->id = id;
		media->resource = resource;
		media->location = resource;
		media->mediaType = "MediaType.directShow";
		return media;
	}

	void parse(int timeout) {
		VLC::Instance instance = VLC::Instance(0, nullptr);
		VLC::Media media = VLC::Media(instance, this->location, VLC::Media::FromLocation);
		std::promise<bool>* isParsed = new std::promise<bool>();
		media.eventManager().onParsedChanged(
			[isParsed](VLC::Media::ParsedStatus status) -> void {
				isParsed->set_value(true);
			}
		);
		media.parseWithOptions(
			VLC::Media::ParseFlags::Network,
			timeout
		);
		isParsed->get_future().get();
		this->metas["title"]       = media.meta(libvlc_meta_Title);
		this->metas["artist"]      = media.meta(libvlc_meta_Artist);
		this->metas["genre"]       = media.meta(libvlc_meta_Genre);
		this->metas["copyright"]   = media.meta(libvlc_meta_Copyright);
		this->metas["trackNumber"] = media.meta(libvlc_meta_TrackNumber);
		this->metas["description"] = media.meta(libvlc_meta_Description);
		this->metas["rating"]      = media.meta(libvlc_meta_Rating);
		this->metas["date"]        = media.meta(libvlc_meta_Date);
		this->metas["settings"]    = media.meta(libvlc_meta_Setting);
		this->metas["url"]         = media.meta(libvlc_meta_URL);
		this->metas["language"]    = media.meta(libvlc_meta_Language);
		this->metas["nowPlaying"]  = media.meta(libvlc_meta_NowPlaying);
		this->metas["encodedBy"]   = media.meta(libvlc_meta_EncodedBy);
		this->metas["artworkUrl"]  = media.meta(libvlc_meta_ArtworkURL);
		this->metas["trackTotal"]  = media.meta(libvlc_meta_TrackTotal);
		this->metas["director"]    = media.meta(libvlc_meta_Director);
		this->metas["season"]      = media.meta(libvlc_meta_Season);
		this->metas["episode"]     = media.meta(libvlc_meta_Episode);
		this->metas["actors"]      = media.meta(libvlc_meta_Actors);
		this->metas["albumArtist"] = media.meta(libvlc_meta_AlbumArtist);
		this->metas["discNumber"]  = media.meta(libvlc_meta_DiscNumber);
		this->metas["discTotal"]   = media.meta(libvlc_meta_DiscTotal);
		this->metas["duration"]    = std::to_string(media.duration());
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
