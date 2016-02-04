//
//  MusicPlayerItem.h
//  3DMusicPlayer
//


#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#define DeviceMotionUpdateNotificaion @"DeviceMotionUpdateNotificaion"

@interface MusicPlayerItem : NSObject
@property (strong, nonatomic) NSURL *audioURL;

@property (assign, nonatomic) CGPoint  audioAngle;
@property (assign, nonatomic) CGPoint  minAngle;
@property (assign, nonatomic) CGPoint  maxAngle;
@property (assign, nonatomic) CGPoint  minFalloff;
@property (assign, nonatomic) CGPoint  maxFalloff;

-(void)play;

@end
