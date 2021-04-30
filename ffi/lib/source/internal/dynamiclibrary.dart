import 'dart:ffi';
import 'dart:io';
import 'package:path/path.dart' as path;


final DynamicLibrary dynamicLibrary = DynamicLibrary.open(
  path.join(Directory.current.path, 'dart_vlc.so'),
);
