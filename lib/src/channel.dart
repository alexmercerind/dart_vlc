import 'package:flutter/services.dart';
import 'package:dart_vlc/src/player.dart';
import 'package:dart_vlc/src/playerState/playerState.dart';

/// Internally used map to keep [Player] instances & manage event streams.
Map<int, Player> players = {};

/// Platform channel for invoking methods & handling calls.
final MethodChannel channel = new MethodChannel('dart_vlc')
  ..setMethodCallHandler((MethodCall methodCall) async {
    switch (methodCall.method) {

      /// [Player] event.
      case 'playerState':
        {
          int id = methodCall.arguments['id'];
          PlayerState state = PlayerState.fromMap(methodCall.arguments);

          /// Notify the stream of the [Player] with respective [id].
          players[id].streamController.add(state);

          /// Update static state of the [Player].
          players[id].state = state;
        }
    }
  });
