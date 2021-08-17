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

#ifndef MEDIASOURCE_MEDIASOURCE_H_
#define MEDIASOURCE_MEDIASOURCE_H_

#include <string>

class MediaSource {
 public:
  virtual std::string Type() = 0;
  virtual ~MediaSource() = default;
};

#endif