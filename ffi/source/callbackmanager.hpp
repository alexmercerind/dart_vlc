#ifndef EXPORT
#ifdef __WIN32
#define EXPORT declspec(__dllexport)
#else
#define EXPORT
#endif
#endif

#include <iostream>

#ifndef CallbackManager_h
#define CallbackManager_h


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


typedef void (*Dart_WeakPersistentHandleFinalizer) (void* isolate_callback_data, Dart_WeakPersistentHandle handle, void* peer);


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

Dart_PostCObjectType dartPostCObject = NULL;
Dart_Port callbackPort;

EXPORT void RegisterDart_PostCObject(Dart_PostCObjectType _dartPostCObject) {
    dartPostCObject = _dartPostCObject;
}

EXPORT void RegisterDart_CallbackPort(Dart_Port _callbackPort) {
    callbackPort = _callbackPort;
}

EXPORT void callbackInt32(int32_t value) {
    Dart_CObject dart_object;
    dart_object.type = Dart_CObject_kInt32;
    dart_object.value.as_int32 = value;
    dartPostCObject(callbackPort, &dart_object);
}

EXPORT void callbackStringArray(int length, char** values) {
    Dart_CObject **valueObjects = new Dart_CObject *[length];
    for (int i = 0; i < length; i++) {
        Dart_CObject *valueObject = new Dart_CObject;
        valueObject->type = Dart_CObject_kString;
        valueObject->value.as_string = values[i];
        valueObjects[i] = valueObject;
    }
    Dart_CObject dart_object;
    dart_object.type = Dart_CObject_kArray;
    dart_object.value.as_array.length = length;
    dart_object.value.as_array.values = valueObjects;
    dartPostCObject(callbackPort, &dart_object);
    for (int i = 0; i < length; i++) {
        delete valueObjects[i];
    }
    delete[] valueObjects;
}

#ifdef __cplusplus
}
#endif

#endif