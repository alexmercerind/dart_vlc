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

#ifndef API_API_H_
#define API_API_H_

#include <cstdint>

#include "api/event_manager.h"
#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

struct DartDeviceList {
  struct Device {
    const char* name;
    const char* id;
    explicit Device(const char* name, const char* id) : name(name), id(id) {}
  };

  int32_t size;
  const Device* device_infos;
};

struct DartEqualizer {
  int32_t id;
  float pre_amp;
  const float* bands;
  const float* amps;
  int32_t size;
};

DLLEXPORT void PlayerCreate(int32_t id, int32_t video_width,
                            int32_t video_height,
                            int32_t command_line_argument_count,
                            const char** command_line_arguments);
DLLEXPORT void PlayerDispose(int32_t id);

DLLEXPORT void PlayerOpen(int32_t id, bool auto_start, const char** source,
                          int32_t source_size);

DLLEXPORT void PlayerPlay(int32_t id);

DLLEXPORT void PlayerPause(int32_t id);

DLLEXPORT void PlayerPlayOrPause(int32_t id);

DLLEXPORT void PlayerStop(int32_t id);

DLLEXPORT void PlayerNext(int32_t id);

DLLEXPORT void PlayerPrevious(int32_t id);

DLLEXPORT void PlayerJumpToIndex(int32_t id, int32_t index);

DLLEXPORT void PlayerSeek(int32_t id, int32_t position);

DLLEXPORT void PlayerSetVolume(int32_t id, float volume);

DLLEXPORT void PlayerSetRate(int32_t id, float rate);

DLLEXPORT void PlayerSetUserAgent(int32_t id, const char* user_agent);

DLLEXPORT void PlayerSetDevice(int32_t id, const char* device_id,
                               const char* device_name);

DLLEXPORT void PlayerSetEqualizer(int32_t id, int32_t equalizer_id);

DLLEXPORT void PlayerSetPlaylistMode(int32_t id, const char* mode);

DLLEXPORT void PlayerAdd(int32_t id, const char* type, const char* resource);

DLLEXPORT void PlayerRemove(int32_t id, int32_t index);

DLLEXPORT void PlayerInsert(int32_t id, int32_t index, const char* type,
                            const char* resource);

DLLEXPORT void PlayerMove(int32_t id, int32_t initial_index,
                          int32_t final_index);

DLLEXPORT void PlayerTakeSnapshot(int32_t id, const char* file_path,
                                  int32_t width, int32_t height);

DLLEXPORT void PlayerSetAudioTrack(int32_t id, int32_t track);

DLLEXPORT int32_t PlayerGetAudioTrackCount(int32_t id);

DLLEXPORT void PlayerSetHWND(int32_t id, int64_t hwnd);

DLLEXPORT const char** MediaParse(Dart_Handle object, const char* type,
                                  const char* resource, int32_t timeout);

DLLEXPORT void BroadcastCreate(int32_t id, const char* type,
                               const char* resource, const char* access,
                               const char* mux, const char* dst,
                               const char* vcodec, int32_t vb,
                               const char* acodec, int32_t ab);

DLLEXPORT void BroadcastStart(int32_t id);

DLLEXPORT void BroadcastDispose(int32_t id);

DLLEXPORT void ChromecastCreate(int32_t id, const char* type,
                                const char* resource, const char* ip_address);

DLLEXPORT void ChromecastStart(int32_t id);

DLLEXPORT void ChromecastDispose(int32_t id);

DLLEXPORT void RecordCreate(int32_t id, const char* saving_file,
                            const char* type, const char* resource);

DLLEXPORT void RecordStart(int32_t id);

DLLEXPORT void RecordDispose(int32_t id);

DLLEXPORT DartDeviceList* DevicesAll(Dart_Handle object);

DLLEXPORT struct DartEqualizer* EqualizerCreateEmpty(Dart_Handle object);

DLLEXPORT struct DartEqualizer* EqualizerCreateMode(Dart_Handle object,
                                                    int32_t mode);

DLLEXPORT void EqualizerSetBandAmp(int32_t id, float band, float amp);

DLLEXPORT void EqualizerSetPreAmp(int32_t id, float amp);

#ifdef __cplusplus
}
#endif
#endif
