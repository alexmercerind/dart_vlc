/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

#ifndef Media_HEADER
#define Media_HEADER

#include <filesystem>
#include <future>
#include <map>
#include <string>
#include <string_view>
#include <vlcpp/vlc.hpp>

#include "mediasource.hpp"

VLC::Instance g_vlc_instance = VLC::Instance(0, nullptr);

class Media : public MediaSource {
 public:
  static constexpr auto kMediaTypeFile = "MediaType.file";
  static constexpr auto kMediaTypeNetwork = "MediaType.network";
  static constexpr auto kMediaTypeDirectShow = "MediaType.directShow";

  std::string& media_type() { return media_type_; };
  std::string& resource() { return resource_; };
  std::string& location() { return location_; };
  std::map<std::string, std::string>& metas() { return metas_; };

  static std::shared_ptr<Media> create(std::string_view type,
                                       const std::string& url,
                                       bool parse = false) {
    if (type.compare(kMediaTypeFile) == 0)
      return std::shared_ptr<Media>(Media::file(url, parse));
    else if (type.compare(kMediaTypeNetwork) == 0)
      return std::shared_ptr<Media>(Media::network(url, parse));
    else
      return std::shared_ptr<Media>(Media::directShow(url));
  }

  static std::shared_ptr<Media> file(std::string path, bool parse = false,
                                     int timeout = 10000) {
    std::shared_ptr<Media> media = std::make_shared<Media>();
    media->resource_ = path;
    media->location_ = "file:///" + path;
    media->media_type_ = kMediaTypeFile;
    if (parse) media->parse(timeout);
    return media;
  }

  static std::shared_ptr<Media> network(std::string url, bool parse = false,
                                        int timeout = 10000) {
    std::shared_ptr<Media> media = std::make_shared<Media>();
    media->resource_ = url;
    media->location_ = url;
    media->media_type_ = kMediaTypeNetwork;
    if (parse) media->parse(timeout);
    return media;
  }

  static std::shared_ptr<Media> directShow(std::string resource) {
    std::shared_ptr<Media> media = std::make_shared<Media>();
    media->resource_ = resource;
    media->location_ = resource;
    media->media_type_ = kMediaTypeDirectShow;
    return media;
  }

  void parse(int timeout) {
    VLC::Media media =
        VLC::Media(g_vlc_instance, location_, VLC::Media::FromLocation);
    std::promise<bool> is_parsed = std::promise<bool>();
    auto is_parsed_ptr = &is_parsed;
    media.eventManager().onParsedChanged(
        [is_parsed_ptr](VLC::Media::ParsedStatus status) -> void {
          is_parsed_ptr->set_value(true);
        });
    media.parseWithOptions(VLC::Media::ParseFlags::Network, timeout);
    is_parsed_ptr->get_future().wait();
    metas_["title"] = media.meta(libvlc_meta_Title);
    metas_["artist"] = media.meta(libvlc_meta_Artist);
    metas_["genre"] = media.meta(libvlc_meta_Genre);
    metas_["copyright"] = media.meta(libvlc_meta_Copyright);
    metas_["album"] = media.meta(libvlc_meta_Album);
    metas_["trackNumber"] = media.meta(libvlc_meta_TrackNumber);
    metas_["description"] = media.meta(libvlc_meta_Description);
    metas_["rating"] = media.meta(libvlc_meta_Rating);
    metas_["date"] = media.meta(libvlc_meta_Date);
    metas_["settings"] = media.meta(libvlc_meta_Setting);
    metas_["url"] = media.meta(libvlc_meta_URL);
    metas_["language"] = media.meta(libvlc_meta_Language);
    metas_["nowPlaying"] = media.meta(libvlc_meta_NowPlaying);
    metas_["encodedBy"] = media.meta(libvlc_meta_EncodedBy);
    metas_["artworkUrl"] = media.meta(libvlc_meta_ArtworkURL);
    metas_["trackTotal"] = media.meta(libvlc_meta_TrackTotal);
    metas_["director"] = media.meta(libvlc_meta_Director);
    metas_["season"] = media.meta(libvlc_meta_Season);
    metas_["episode"] = media.meta(libvlc_meta_Episode);
    metas_["actors"] = media.meta(libvlc_meta_Actors);
    metas_["albumArtist"] = media.meta(libvlc_meta_AlbumArtist);
    metas_["discNumber"] = media.meta(libvlc_meta_DiscNumber);
    metas_["discTotal"] = media.meta(libvlc_meta_DiscTotal);
    metas_["duration"] = std::to_string(media.duration());
  }

  std::string Type() { return "MediaSourceType.media"; }

 private:
  std::string media_type_;
  std::string resource_;
  std::string location_;
  std::map<std::string, std::string> metas_;
};

#endif
