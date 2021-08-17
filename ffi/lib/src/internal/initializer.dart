import 'dart:ffi';
import 'package:dart_vlc_ffi/src/internal/ffi.dart';
import 'package:dart_vlc_ffi/src/internal/dynamiclibrary.dart';
import 'package:dart_vlc_ffi/src/internal/typedefs/callback.dart';

class DartVLC {
  static void initialize(String dynamicLibraryPath) {
    if (!isInitialized) {
      dynamicLibrary = DynamicLibrary.open(dynamicLibraryPath);
      RegisterPostCObjectDart registerPostCObject = dynamicLibrary
          .lookup<NativeFunction<RegisterPostCObjectCXX>>(
              'RegisterDartPostCObject')
          .asFunction();
      RegisterCallbackPortDart registerCallbackPort = dynamicLibrary
          .lookup<NativeFunction<RegisterCallbackPortCXX>>(
              'RegisterDartCallbackPort')
          .asFunction();
      DartInitializeApiDLDart dartInitializeApiDL = dynamicLibrary
          .lookup<NativeFunction<DartInitializeApiDLCXX>>('DartInitializeApiDL')
          .asFunction();
      registerPostCObject(NativeApi.postCObject);
      registerCallbackPort(receiver.sendPort.nativePort);
      dartInitializeApiDL(NativeApi.initializeApiDLData);
      isInitialized = true;
    }
  }
}
