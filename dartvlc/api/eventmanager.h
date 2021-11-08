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

#include "api/dartmanager.h"
#include "base.h"
#include "dart_api_dl.h"
#include "player.h"
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
  type_object.value.as_string = const_cast<char*>("playbackEvent");

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
  type_object.value.as_string = const_cast<char*>("positionEvent");

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
  type_object.value.as_string = const_cast<char*>("completeEvent");

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
  type_object.value.as_string = const_cast<char*>("volumeEvent");

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
  type_object.value.as_string = const_cast<char*>("rateEvent");

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

  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = const_cast<char*>("openEvent");

  Dart_CObject index_object;
  index_object.type = Dart_CObject_kInt32;
  index_object.value.as_int32 = state->index();

  Dart_CObject is_playlist_object;
  is_playlist_object.type = Dart_CObject_kBool;
  is_playlist_object.value.as_int32 = state->is_playlist();

  auto types_objects =
      std::unique_ptr<Dart_CObject[]>(new Dart_CObject[media_items.size()]);
  auto types_object_refs =
      std::unique_ptr<Dart_CObject* []>(new Dart_CObject*[media_items.size()]);
  std::vector<std::string> types_str(media_items.size());
  std::vector<const char*> types_ptr(media_items.size());
  for (int32_t i = 0; i < media_items.size(); i++) {
    types_str[i] = media_items[i]->media_type();
    types_ptr[i] = types_str[i].c_str();
    Dart_CObject* value_object = &types_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = const_cast<char*>(types_ptr[i]);
    types_object_refs[i] = value_object;
  }
  Dart_CObject types_object;
  types_object.type = Dart_CObject_kArray;
  types_object.value.as_array.length = media_items.size();
  types_object.value.as_array.values = types_object_refs.get();

  auto resources_objects =
      std::unique_ptr<Dart_CObject[]>(new Dart_CObject[media_items.size()]);
  auto resources_object_refs =
      std::unique_ptr<Dart_CObject* []>(new Dart_CObject*[media_items.size()]);
  std::vector<std::string> resources_str(media_items.size());
  std::vector<const char*> resources_ptr(media_items.size());
  for (int32_t i = 0; i < media_items.size(); i++) {
    resources_str[i] = media_items[i]->resource();
    resources_ptr[i] = resources_str[i].c_str();
    Dart_CObject* value_object = &resources_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = const_cast<char*>(resources_ptr[i]);
    resources_object_refs[i] = value_object;
  }
  Dart_CObject resources_object;
  resources_object.type = Dart_CObject_kArray;
  resources_object.value.as_array.length = media_items.size();
  resources_object.value.as_array.values = resources_object_refs.get();

  Dart_CObject* value_objects[] = {&id_object,    &type_object,
                                   &index_object, &is_playlist_object,
                                   &types_object, &resources_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 6;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnVideoDimensions(int32_t id, int32_t video_width,
                              int32_t video_height) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = const_cast<char*>("videoDimensionsEvent");

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
  type_object.value.as_string = const_cast<char*>("videoEvent");

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

inline void OnBuffering(int32_t id, float buffering) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = const_cast<char*>("bufferingEvent");

  Dart_CObject buffering_object;
  buffering_object.type = Dart_CObject_kDouble;
  buffering_object.value.as_double = static_cast<double>(buffering);

  Dart_CObject* value_objects[] = {&id_object, &type_object, &buffering_object};

  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 3;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
}

inline void OnError(int32_t id, const char* error) {
  Dart_CObject id_object;
  id_object.type = Dart_CObject_kInt32;
  id_object.value.as_int32 = id;

  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = const_cast<char*>("errorEvent");

  Dart_CObject error_object;
  error_object.type = Dart_CObject_kString;
  error_object.value.as_string = const_cast<char*>(error);

  Dart_CObject* value_objects[] = {&id_object, &type_object, &error_object};

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
