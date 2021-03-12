#include <string>
#include <map>
#include <filesystem>

#include "audiosource.hpp"

#ifndef Audio_HEADER
#define Audio_HEADER


class Audio: public AudioSource {
public:
	std::string audioType;
	std::string resource;
	std::string location;
	std::map<std::string, std::string> metas;

	static Audio* file(std::string path) {
		Audio* audio = new Audio();
		audio->resource = path;
		audio->location = "file:///" + path;
		audio->audioType = "AudioType.file";
		return audio;
	}

	static Audio* network(std::string url) {
		Audio* audio = new Audio();
		audio->resource = url;
		audio->location = url;
		audio->audioType = "AudioType.network";
		return audio;
	}

	static Audio* asset(std::string path) {
		Audio* audio = new Audio();
		audio->resource = path;
		audio->location = "file:///" + std::filesystem::temp_directory_path().u8string() + path;
		audio->audioType = "AudioType.asset";
		return audio;
	}

	std::map<std::string, std::string> get() {
		std::map<std::string, std::string> audio;
		audio["audioType"] = this->audioType;
		audio["resource"] = this->resource;
		return audio;
	}
	
	std::string audioSourceType() {
		return "AudioSourceType.audio";
	}
};


#endif
