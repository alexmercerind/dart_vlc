import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Following typedef is used for:
/// Record::create
typedef RecordCreateCXX = Void Function(Int32 id, Pointer<Utf8> savingFile,
    Pointer<Utf8> type, Pointer<Utf8> resource);
typedef RecordCreateDart = void Function(int id, Pointer<Utf8> savingFile,
    Pointer<Utf8> type, Pointer<Utf8> resource);

/// Following typedef is used for:
/// Record::start
typedef RecordStartCXX = Void Function(Int32 id);
typedef RecordStartDart = void Function(int id);

/// Following typedef is used for:
/// Record::dispose
typedef RecordDisposeCXX = Void Function(Int32 id);
typedef RecordDisposeDart = void Function(int id);
