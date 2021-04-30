/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC & libVLC++.
 * 
 * Hitesh Kumar Saini, Domingo Montesdeoca Gonzalez & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 * 
 * GNU Lesser General Public License v2.1
 */

import 'dart:ffi';

typedef RegisterPostCObjectCXX = Void Function(Pointer<NativeFunction<Int8 Function(Int64, Pointer<Dart_CObject>)>> functionPointer);
typedef RegisterPostCObjectDart = void Function(Pointer<NativeFunction<Int8 Function(Int64, Pointer<Dart_CObject>)>> functionPointer);

typedef RegisterCallbackPortCXX = Void Function(Int64 nativePort);
typedef RegisterCallbackPortDart = void Function(int nativePort);
