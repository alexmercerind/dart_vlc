import 'dart:ffi';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/dynamiclibrary.dart';

typedef InitializeDartApiCXX = Void Function(
    Pointer<NativeFunction<Int8 Function(Int64, Pointer<Dart_CObject>)>>
        postCObject,
    Int64 nativePort,
    Pointer<Void> initializeApiDLData);
typedef InitializeDartApiDart = void Function(
    Pointer<NativeFunction<Int8 Function(Int64, Pointer<Dart_CObject>)>>
        postCObject,
    int nativePort,
    Pointer<Void> initializeApiDLData);

class DartVLC {
  static void initialize(String dynamicLibraryPath) {
    if (!isInitialized) {
      dynamicLibrary = DynamicLibrary.open(dynamicLibraryPath);
      InitializeDartApiDart initializeDartApi = dynamicLibrary
          .lookup<NativeFunction<InitializeDartApiCXX>>('InitializeDartApi')
          .asFunction();
      initializeDartApi(NativeApi.postCObject, receiver.sendPort.nativePort,
          NativeApi.initializeApiDLData);
      isInitialized = true;
    }
  }
}
