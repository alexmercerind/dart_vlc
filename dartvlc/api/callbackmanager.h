#ifndef API_CALLBACKMANAGER_H_
#define API_CALLBACKMANAGER_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "base.h"

typedef int64_t Dart_Port;

typedef enum {
  Dart_TypedData_kByteData = 0,
  Dart_TypedData_kInt8,
  Dart_TypedData_kUint8,
  Dart_TypedData_kUint8Clamped,
  Dart_TypedData_kInt16,
  Dart_TypedData_kUint16,
  Dart_TypedData_kInt32,
  Dart_TypedData_kUint32,
  Dart_TypedData_kInt64,
  Dart_TypedData_kUint64,
  Dart_TypedData_kFloat32,
  Dart_TypedData_kFloat64,
  Dart_TypedData_kFloat32x4,
  Dart_TypedData_kInvalid
} Dart_TypedData_Type;

typedef struct _Dart_WeakPersistentHandle* Dart_WeakPersistentHandle;

typedef void (*Dart_WeakPersistentHandleFinalizer)(
    void* isolate_callback_data, Dart_WeakPersistentHandle handle, void* peer);

typedef enum {
  Dart_CObject_kNull = 0,
  Dart_CObject_kBool,
  Dart_CObject_kInt32,
  Dart_CObject_kInt64,
  Dart_CObject_kDouble,
  Dart_CObject_kString,
  Dart_CObject_kArray,
  Dart_CObject_kTypedData,
  Dart_CObject_kExternalTypedData,
  Dart_CObject_kSendPort,
  Dart_CObject_kCapability,
  Dart_CObject_kUnsupported,
  Dart_CObject_kNumberOfTypes
} Dart_CObject_Type;

typedef struct _Dart_CObject {
  Dart_CObject_Type type;
  union {
    bool as_bool;
    int32_t as_int32;
    int64_t as_int64;
    double as_double;
    char* as_string;
    struct {
      Dart_Port id;
      Dart_Port origin_id;
    } as_send_port;
    struct {
      int64_t id;
    } as_capability;
    struct {
      intptr_t length;
      struct _Dart_CObject** values;
    } as_array;
    struct {
      Dart_TypedData_Type type;
      intptr_t length;
      uint8_t* values;
    } as_typed_data;
    struct {
      Dart_TypedData_Type type;
      intptr_t length;
      uint8_t* data;
      void* peer;
      Dart_WeakPersistentHandleFinalizer callback;
    } as_external_typed_data;
  } value;
} Dart_CObject;

typedef bool (*Dart_PostCObjectType)(Dart_Port port_id, Dart_CObject* message);

#ifdef __cplusplus
extern "C" {
#endif

Dart_PostCObjectType g_dart_post_C_object = NULL;
Dart_Port g_callback_port;

DLLEXPORT void RegisterDartPostCObject(
    Dart_PostCObjectType dart_post_C_object) {
  g_dart_post_C_object = dart_post_C_object;
}

DLLEXPORT void RegisterDartCallbackPort(Dart_Port callback_port) {
  g_callback_port = callback_port;
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
      std::unique_ptr<Dart_CObject*[]>(new Dart_CObject*[length]);

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
      std::unique_ptr<Dart_CObject*[]>(new Dart_CObject*[length]);

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
