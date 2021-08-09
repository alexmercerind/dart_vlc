/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

#ifndef MediaSource_HEADER
#define MediaSource_HEADER

#include <string>


class MediaSource {
public:
	int _;
        virtual ~MediaSource() = default;
	virtual std::string mediaSourceType() = 0;
};


#endif
