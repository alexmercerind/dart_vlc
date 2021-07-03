import 'package:dart_vlc_ffi/source/internal/dart_vlc.dart';

Future main() async {
  receiver.listen((event) => print(event));
  CallbackFFI.initialize();
  PlayerFFI.create(0, 320, 240, 0, <String>[].toNativeUtf8Array());
  PlayerFFI.open(
    0, 1,
    ['0', 'MediaType.file', '/home/alexmercerind/video.mp4'].toNativeUtf8Array(),
    1
  );
  await Future.delayed(Duration(seconds: 5));
}
