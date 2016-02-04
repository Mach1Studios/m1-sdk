/*
 
 Mach1 - M1
 Example scripts deployed on iOS platform
 
 */


#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#define DeviceMotionUpdateNotificaion @"DeviceMotionUpdateNotificaion"

@interface PlayerItem : NSObject
@property (strong, nonatomic) NSURL *audioURL;

@property (assign, nonatomic) CGPoint  audioAngle;
@property (assign, nonatomic) CGPoint  minAngle;
@property (assign, nonatomic) CGPoint  maxAngle;
@property (assign, nonatomic) CGPoint  minFalloff;
@property (assign, nonatomic) CGPoint  maxFalloff;

-(void)play;

@end
