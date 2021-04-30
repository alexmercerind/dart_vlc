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
import 'dart:io';
import 'package:path/path.dart' as path;


final DynamicLibrary dynamicLibrary = DynamicLibrary.open(
  path.join(Directory.current.path, 'dart_vlc.so'),
);
