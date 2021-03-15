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

#include "mediasource.hpp"

#ifndef Media_HEADER
#define Media_HEADER


class Media: public MediaSource {
public:
	std::string mediaType;
	std::string resource;
	std::string location;
	std::map<std::string, std::string> metas;

	static Media* file(std::string path) {
		Media* media = new Media();
		media->resource = path;
		media->location = "file:///" + path;
		media->mediaType = "MediaType.file";
		return media;
	}

	static Media* network(std::string url) {
		Media* media = new Media();
		media->resource = url;
		media->location = url;
		media->mediaType = "MediaType.network";
		return media;
	}

	static Media* asset(std::string path) {
		Media* media = new Media();
		media->resource = path;
		media->location = "file:///" + std::filesystem::temp_directory_path().u8string() + path;
		media->mediaType = "MediaType.asset";
		return media;
	}

	std::map<std::string, std::string> get() {
		std::map<std::string, std::string> media;
		media["mediaType"] = this->mediaType;
		media["resource"] = this->resource;
		return media;
	}
	
	std::string mediaSourceType() {
		return "MediaSourceType.media";
	}
};


#endif
