#import <Foundation/Foundation.h>

typedef enum {
  m1Default = 0, m1Unity, m1UE, m1oFEasyCam, m1Android, m1iOSPortrait, m1iOSLandscape
} AngularSettingsType;

@interface Mach1Decode : NSObject

-(id) init;
-(void) dealloc;

-(void) setAngularSettingsType:(AngularSettingsType)type;
-(void) setFilterSpeed:(float)filterSpeed;

-(void) beginBuffer;
-(void) endBuffer;

-(NSNumber*) getCurrentTime;
-(NSString*) getLog;

-(NSMutableArray*) horizonAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex;
-(NSMutableArray*) horizonPairsAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex;
-(NSMutableArray*) spatialAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex;
-(NSMutableArray*) spatialAltAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex;
-(NSMutableArray*) spatialPairsAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex;

-(NSMutableArray*) horizonAlgo:(float)Yaw :(float)Pitch :(float)Roll;
-(NSMutableArray*) horizonPairsAlgo:(float)Yaw :(float)Pitch :(float)Roll;
-(NSMutableArray*) spatialAlgo:(float)Yaw :(float)Pitch :(float)Roll;
-(NSMutableArray*) spatialAltAlgo:(float)Yaw :(float)Pitch :(float)Roll;
-(NSMutableArray*) spatialPairsAlgo:(float)Yaw :(float)Pitch :(float)Roll;

@end



