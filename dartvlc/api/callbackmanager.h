#ifndef API_CALLBACKMANAGER_H_
#define API_CALLBACKMANAGER_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "base.h"
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

#ifdef __cplusplus
}
#endif

void CallbackInt32(int32_t value) {
  Dart_CObject dart_object;
  dart_object.type = Dart_CObject_kInt32;
  dart_object.value.as_int32 = value;
  g_dart_post_C_object(g_callback_port, &dart_object);
}

void CallbackStringArray(int32_t length, char** values) {
  auto value_objects =
      std::unique_ptr<Dart_CObject[]>(new Dart_CObject[length]);
  auto value_object_refs =
      std::unique_ptr<Dart_CObject* []>(new Dart_CObject*[length]);

  for (int32_t i = 0; i < length; i++) {
    Dart_CObject* value_object = &value_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = values[i];
    value_object_refs[i] = value_object;
  }
  Dart_CObject dart_object;
  dart_object.type = Dart_CObject_kArray;
  dart_object.value.as_array.length = length;
  dart_object.value.as_array.values = value_object_refs.get();
  g_dart_post_C_object(g_callback_port, &dart_object);
}

void CallbackStringArray(const std::vector<std::string>& values) {
  auto length = values.size();
  auto value_objects =
      std::unique_ptr<Dart_CObject[]>(new Dart_CObject[length]);
  auto value_object_refs =
      std::unique_ptr<Dart_CObject* []>(new Dart_CObject*[length]);

  for (int32_t i = 0; i < length; i++) {
    Dart_CObject* value_object = &value_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = const_cast<char*>(values[i].c_str());
    value_object_refs[i] = value_object;
  }
  Dart_CObject dart_object;
  dart_object.type = Dart_CObject_kArray;
  dart_object.value.as_array.length = length;
  dart_object.value.as_array.values = value_object_refs.get();
  g_dart_post_C_object(g_callback_port, &dart_object);
}

void CallbackFrame(int32_t id, int32_t length, uint8_t* frame) {
  Dart_CObject idObject;
  idObject.type = Dart_CObject_kInt32;
  idObject.value.as_int32 = id;
  Dart_CObject frame_object;
  frame_object.type = Dart_CObject_kTypedData;
  frame_object.value.as_typed_data.type = Dart_TypedData_kUint8;
  frame_object.value.as_typed_data.values = frame;
  frame_object.value.as_typed_data.length = length;
  Dart_CObject* values[2] = {&idObject, &frame_object};
  Dart_CObject value_object;
  value_object.type = Dart_CObject_kArray;
  value_object.value.as_array.length = 2;
  value_object.value.as_array.values = values;
  g_dart_post_C_object(g_callback_port, &value_object);
}

#endif
