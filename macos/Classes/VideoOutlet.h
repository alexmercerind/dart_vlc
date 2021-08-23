#import <FlutterMacOS/FlutterTexture.h>

@interface VideoOutlet : NSObject <FlutterTexture>

@property(nonatomic, weak) id<FlutterTextureRegistry> textureRegistry;
@property(nonatomic) int64_t textureId;

+ (instancetype)outletForPlayer:(int)playerId
            withTextureRegistry:(id<FlutterTextureRegistry>)textureRegistry;
- (void)dispose;

@end
