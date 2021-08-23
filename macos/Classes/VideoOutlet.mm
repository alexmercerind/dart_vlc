#import "VideoOutlet.h"

#include "player.h"

@implementation VideoOutlet {
  Player* _player;
  CVPixelBufferRef _buffer;
  size_t _last_width;
  size_t _last_height;
}

@synthesize textureRegistry = _textures;
@synthesize textureId = _textureId;

+ (instancetype)outletForPlayer:(int)playerId
            withTextureRegistry:(id<FlutterTextureRegistry>)textureRegistry {
  Player* player = g_players->Get(playerId);
  if (!player) {
    return nil;
  }

  return [[self alloc] initWithPlayer:player textureRegistry:textureRegistry];
}

- (instancetype)initWithPlayer:(Player*)player
               textureRegistry:(id<FlutterTextureRegistry>)textureRegistry {
  self = [super init];

  if (self) {
    _player = player;
    _textures = textureRegistry;
    _buffer = nil;
    _last_width = 0;
    _last_height = 0;

    _textureId = [textureRegistry registerTexture:self];

    player->OnVideo([self](uint8_t* frame, int32_t width, int32_t height) {
      [self onFrame:frame width:width height:height];
    });
  }

  return self;
}

- (void)dealloc {
  if (_buffer != nil) {
    CVBufferRelease(_buffer);
  }
}

- (void)dispose {
  _player->OnVideo(nullptr);
  [_textures unregisterTexture:_textureId];
}

- (void)onFrame:(uint8_t*)buffer width:(size_t)width height:(size_t)height {
  if (buffer == nullptr) {
    return;
  }

  if (_buffer == nil || width != _last_width || height != _last_height) {
    if (_buffer != nil) {
      CVBufferRelease(_buffer);
    }

    NSDictionary* options = @{
      (id)kCVPixelBufferIOSurfacePropertiesKey : @{}
      //(NSString*)kCVPixelBufferMetalCompatibilityKey : @YES
    };

    CVReturn status =
        CVPixelBufferCreate(kCFAllocatorDefault, width, height, kCVPixelFormatType_32BGRA,
                            (__bridge CFDictionaryRef)options, &_buffer);
    if (status != kCVReturnSuccess) {
      _last_width = _last_height = 0;
      return;
    }

    _last_width = width;
    _last_height = height;
  }

  CVPixelBufferLockBaseAddress(_buffer, 0);
  void* buf = CVPixelBufferGetBaseAddress(_buffer);

  // TODO: Get rid of this memcpy
  // by making the player directly write into the CVPixelBuffer
  memcpy(buf, buffer, width * height * 4);

  CVPixelBufferUnlockBaseAddress(_buffer, 0);

  __weak VideoOutlet* weakSelf = self;
  dispatch_async(dispatch_get_main_queue(), ^{
    __strong VideoOutlet* strongSelf = weakSelf;
    if (strongSelf) {
      [strongSelf.textureRegistry textureFrameAvailable:strongSelf.textureId];
    }
  });
}

- (CVPixelBufferRef)copyPixelBuffer {
  if (_buffer != nil) {
    CVBufferRetain(_buffer);
    return _buffer;
  }

  return nil;
}

@end
