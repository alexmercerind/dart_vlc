/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

#ifndef API_DART_VLC_H_
#define API_DART_VLC_H_

#include <cstdint>

#include "base.h"
#include "api/eventmanager.h"

#ifdef __cplusplus
extern "C" {
#endif

struct EqualizerStruct {
  int32_t id;
  float pre_amp;
  float* bands;
  float* amps;
  int32_t size;
};

DLLEXPORT void PlayerCreate(int32_t id, int32_t video_width,
                            int32_t video_height,
                            int32_t commandLineArgumentsCount,
                            const char** commandLineArguments);
DLLEXPORT void PlayerDispose(int32_t id);

DLLEXPORT void PlayerOpen(int32_t id, bool auto_start, const char** source,
                          int32_t source_size);

DLLEXPORT void PlayerPlay(int32_t id);

DLLEXPORT void PlayerPause(int32_t id);

DLLEXPORT void PlayerPlayOrPause(int32_t id);

DLLEXPORT void PlayerStop(int32_t id);
DLLEXPORT void PlayerNext(int32_t id);

DLLEXPORT void PlayerBack(int32_t id);

DLLEXPORT void PlayerJump(int32_t id, int32_t index);

DLLEXPORT void PlayerSeek(int32_t id, int32_t position);

DLLEXPORT void PlayerSetVolume(int32_t id, float volume);

DLLEXPORT void PlayerSetRate(int32_t id, float rate);

DLLEXPORT void PlayerSetUserAgent(int32_t id, const char* userAgent);

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

DLLEXPORT char** MediaParse(const char* type, const char* resource,
                            int32_t timeout);

DLLEXPORT void MediaClear();

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
DLLEXPORT char** DevicesAll();
DLLEXPORT void DevicesClear();
DLLEXPORT struct EqualizerStruct EqualizerCreateEmpty(Dart_Handle object);
DLLEXPORT struct EqualizerStruct EqualizerCreateMode(Dart_Handle object,
                                                     int32_t mode);
DLLEXPORT void EqualizerSetBandAmp(int32_t id, float band, float amp);
DLLEXPORT void EqualizerSetPreAmp(int32_t id, float amp);

#endif
#ifdef __cplusplus
}
#endif
