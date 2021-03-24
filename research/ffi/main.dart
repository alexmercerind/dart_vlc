import 'dart:io';
import 'dart:ffi';

final DynamicLibrary library = DynamicLibrary.open(
  Directory.current.path + '/' + 'source/dll/dart_vlc.dll'
);

