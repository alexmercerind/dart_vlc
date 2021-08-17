import 'dart:ffi';

typedef DartInitializeApiDLCXX = Void Function(Pointer<Void> nativePort);
typedef DartInitializeApiDLDart = void Function(Pointer<Void> nativePort);

typedef RegisterPostCObjectCXX = Void Function(
    Pointer<NativeFunction<Int8 Function(Int64, Pointer<Dart_CObject>)>>
        functionPointer);
typedef RegisterPostCObjectDart = void Function(
    Pointer<NativeFunction<Int8 Function(Int64, Pointer<Dart_CObject>)>>
        functionPointer);
typedef RegisterCallbackPortCXX = Void Function(Int64 nativePort);
typedef RegisterCallbackPortDart = void Function(int nativePort);
