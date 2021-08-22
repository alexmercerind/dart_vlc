/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

#ifndef API_EVENTMANAGER_H_
#define API_EVENTMANAGER_H_

#include "base.h"
#include "player.h"
#include "api/dartmanager.h"
#include "dart_api_dl.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*Dart_PostCObjectType)(Dart_Port port_id, Dart_CObject* message);

Dart_PostCObjectType g_dart_post_C_object;
Dart_Port g_callback_port;

DLLEXPORT void InitializeDartApi(Dart_PostCObjectType dart_post_C_object,
                                 Dart_Port callback_port, void* data) {
  g_dart_post_C_object = dart_post_C_object;
  g_callback_port = callback_port;
  Dart_InitializeApiDL(data);
}

inline void OnPlayPauseStop(int32_t id, PlayerState* state) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "playbackEvent";

  Dart_CObject is_playing_object;
  is_playing_object.type = Dart_CObject_kBool;
  is_playing_object.value.as_bool = state->is_playing();

  Dart_CObject is_seekable_object;
  is_seekable_object.type = Dart_CObject_kBool;
  is_seekable_object.value.as_bool = state->is_seekable();

  Dart_CObject* value_objects[] = {&id_object, &type_object, &is_playing_object,
                                   &is_seekable_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 4;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnPosition(int32_t id, PlayerState* state) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "positionEvent";

  Dart_CObject index_object;
  index_object.type = Dart_CObject_kInt32;
  index_object.value.as_int32 = state->index();

  Dart_CObject position_object;
  position_object.type = Dart_CObject_kInt32;
  position_object.value.as_int32 = state->position();

  Dart_CObject duration_object;
  duration_object.type = Dart_CObject_kInt32;
  duration_object.value.as_int32 = state->duration();

  Dart_CObject* value_objects[] = {&id_object, &type_object, &index_object,
                                   &position_object, &duration_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 5;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnComplete(int32_t id, PlayerState* state) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "completeEvent";

  Dart_CObject is_completed_object;
  is_completed_object.type = Dart_CObject_kBool;
  is_completed_object.value.as_int32 = state->is_completed();

  Dart_CObject* value_objects[] = {&id_object, &type_object,
                                   &is_completed_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 3;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnVolume(int32_t id, PlayerState* state) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "volumeEvent";

  Dart_CObject volume_object;
  volume_object.type = Dart_CObject_kDouble;
  volume_object.value.as_double = state->volume();

  Dart_CObject* value_objects[] = {&id_object, &type_object, &volume_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 3;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnRate(int32_t id, PlayerState* state) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "rateEvent";

  Dart_CObject rate_object;
  rate_object.type = Dart_CObject_kDouble;
  rate_object.value.as_double = state->rate();

  Dart_CObject* value_objects[] = {&id_object, &type_object, &rate_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 3;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnOpen(int32_t id, PlayerState* state) {
  const auto& media_items = state->medias()->medias();

  auto value_objects = std::unique_ptr<Dart_CObject* []>(
      new Dart_CObject*[4 + media_items.size() * 2]);

  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;
  value_objects[0] = &id_object;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "openEvent";
  value_objects[1] = &type_object;

  Dart_CObject index_object;
  index_object.type = Dart_CObject_kInt32;
  index_object.value.as_int32 = state->index();
  value_objects[2] = &index_object;

  Dart_CObject is_playlist_object;
  is_playlist_object.type = Dart_CObject_kBool;
  is_playlist_object.value.as_int32 = state->is_playlist();
  value_objects[3] = &is_playlist_object;
  int index = 0;
  for (const auto& media : media_items) {
    Dart_CObject media_type_object;
    media_type_object.type = Dart_CObject_kString;
    media_type_object.value.as_string =
        const_cast<char*>(media->media_type().c_str());

    Dart_CObject resource_object;
    resource_object.type = Dart_CObject_kString;
    resource_object.value.as_string =
        const_cast<char*>(media->resource().c_str());
    value_objects[index + 4] = &media_type_object;
    value_objects[index + 5] = &resource_object;
    index += 2;
  }

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 4 + media_items.size() * 2;
  return_object.value.as_array.values = value_objects.get();
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnVideoDimensions(int32_t id, int32_t video_width,
                              int32_t video_height) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "videoDimensionsEvent";

  Dart_CObject video_width_object;
  video_width_object.type = Dart_CObject_kInt32;
  video_width_object.value.as_int32 = video_width;

  Dart_CObject video_height_object;
  video_height_object.type = Dart_CObject_kInt32;
  video_height_object.value.as_int32 = video_height;

  Dart_CObject* value_objects[] = {&id_object, &type_object,
                                   &video_width_object, &video_height_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 4;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnVideo(int32_t id, int size, uint8_t* frame) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "videoEvent";

  Dart_CObject frame_object;
  frame_object.type = Dart_CObject_kTypedData;
  frame_object.value.as_typed_data.type = Dart_TypedData_kUint8;
  frame_object.value.as_typed_data.values = frame;
  frame_object.value.as_typed_data.length = size;

  Dart_CObject* value_objects[] = {&id_object, &type_object, &frame_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 3;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

#ifdef __cplusplus
}
#endif

#endif
