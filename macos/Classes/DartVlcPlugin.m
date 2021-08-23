#import "DartVlcPlugin.h"
#import "VideoOutlet.h"

@implementation DartVlcPlugin {
  FlutterMethodChannel* _methodChannel;
  id _textures;
  NSMutableDictionary* _videoOutlets;
}

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  FlutterMethodChannel* channel =
      [FlutterMethodChannel methodChannelWithName:@"dart_vlc"
                                  binaryMessenger:[registrar messenger]];

  DartVlcPlugin* instance = [[DartVlcPlugin alloc] initWithChannel:channel
                                                          textures:[registrar textures]];

  [registrar addMethodCallDelegate:instance channel:channel];
}

- (instancetype)initWithChannel:(FlutterMethodChannel*)channel
                       textures:(NSObject<FlutterTextureRegistry>*)textures {
  self = [super init];

  if (self) {
    _methodChannel = channel;
    _textures = textures;
    _videoOutlets = [[NSMutableDictionary alloc] init];
  }

  return self;
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
  if ([@"PlayerRegisterTexture" isEqualToString:call.method]) {
    id playerId = call.arguments[@"playerId"];
    VideoOutlet* outlet = [VideoOutlet outletForPlayer:[playerId intValue]
                                   withTextureRegistry:_textures];
    if (!outlet) {
      result([FlutterError errorWithCode:@"create_outlet_failed"
                                 message:@"Creating video outlet failed"
                                 details:nil]);
      return;
    }

    [_videoOutlets setObject:outlet forKey:playerId];

    result(@([outlet textureId]));
  } else if ([@"PlayerUnregisterTexture" isEqualToString:call.method]) {
    id playerId = call.arguments[@"playerId"];

    id outlet = [_videoOutlets objectForKey:playerId];
    if (outlet != nil) {
      [outlet dispose];
      [_videoOutlets removeObjectForKey:playerId];
    }
  }

  else {
    result(FlutterMethodNotImplemented);
  }
}

@end
