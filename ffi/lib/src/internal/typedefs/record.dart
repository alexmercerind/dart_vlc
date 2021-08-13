import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef RecordCreateCXX = Void Function(Int32 id, Pointer<Utf8> savingFile,
    Pointer<Utf8> type, Pointer<Utf8> resource);
typedef RecordCreateDart = void Function(int id, Pointer<Utf8> savingFile,
    Pointer<Utf8> type, Pointer<Utf8> resource);
typedef RecordStartCXX = Void Function(Int32 id);
typedef RecordStartDart = void Function(int id);
typedef RecordDisposeCXX = Void Function(Int32 id);
typedef RecordDisposeDart = void Function(int id);
