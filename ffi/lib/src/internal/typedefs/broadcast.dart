import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef BroadcastCreateCXX = Void Function(
    Int32 id,
    Pointer<Utf8> type,
    Pointer<Utf8> resource,
    Pointer<Utf8> access,
    Pointer<Utf8> mux,
    Pointer<Utf8> dst,
    Pointer<Utf8> vcodec,
    Int32 vb,
    Pointer<Utf8> acodec,
    Int32 ab);
typedef BroadcastCreateDart = void Function(
    int id,
    Pointer<Utf8> type,
    Pointer<Utf8> resource,
    Pointer<Utf8> access,
    Pointer<Utf8> mux,
    Pointer<Utf8> dst,
    Pointer<Utf8> vcodec,
    int vb,
    Pointer<Utf8> acodec,
    int ab);
typedef BroadcastStartCXX = Void Function(Int32 id);
typedef BroadcastStartDart = void Function(int id);
typedef BroadcastDisposeCXX = Void Function(Int32 id);
typedef BroadcastDisposeDart = void Function(int id);
