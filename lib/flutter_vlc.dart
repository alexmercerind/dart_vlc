
import 'dart:async';

import 'package:flutter/services.dart';

class FlutterVlc {
  static const MethodChannel _channel =
      const MethodChannel('flutter_vlc');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
