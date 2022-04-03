// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "media_source/media.h"

#include <filesystem>
#include <future>
#include <map>
#include <string>
#include <string_view>
#include <vlcpp/vlc.hpp>

std::shared_ptr<Media> Media::Create(std::string_view type,
                                     const std::string& url, bool parse,
                                     int32_t timeout) {
  if (type.compare(kMediaTypeFile) == 0) {
    return std::shared_ptr<Media>(Media::File(url, parse, timeout));
  } else if (type.compare(kMediaTypeNetwork) == 0) {
    return std::shared_ptr<Media>(Media::Network(url, parse, timeout));
  } else {
    return std::shared_ptr<Media>(Media::DirectShow(url));
  }
}

std::shared_ptr<Media> Media::File(std::string path, bool parse,
                                   int32_t timeout, std::string start_time,
                                   std::string stop_time) {
  std::shared_ptr<Media> media = std::make_shared<Media>();
  media->resource_ = path;
  media->location_ = "file:///" + path;
  media->media_type_ = kMediaTypeFile;
  media->start_time_ = start_time;
  media->stop_time_ = stop_time;
  if (parse) {
    media->Parse(timeout);
  }
  return media;
}

std::shared_ptr<Media> Media::Network(std::string url, bool parse,
                                      int32_t timeout, std::string start_time,
                                      std::string stop_time) {
  std::shared_ptr<Media> media = std::make_shared<Media>();
  media->resource_ = url;
  media->location_ = url;
  media->media_type_ = kMediaTypeNetwork;
  media->start_time_ = start_time;
  media->stop_time_ = stop_time;
  if (parse) {
    media->Parse(timeout);
  }
  return media;
}

std::shared_ptr<Media> Media::DirectShow(std::string resource) {
  std::shared_ptr<Media> media = std::make_shared<Media>();
  media->resource_ = resource;
  media->location_ = resource;
  media->media_type_ = kMediaTypeDirectShow;
  return media;
}

void Media::Parse(int32_t timeout) {
  VLC::Instance vlc_instance = VLC::Instance(0, nullptr);
  VLC::Media media =
      VLC::Media(vlc_instance, location_, VLC::Media::FromLocation);
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

std::string Media::Type() { return "MediaSourceType.media"; }
