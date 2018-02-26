#import "Mach1Decode.h"

@implementation Mach1Decode

void* M1obj;

-(id)init
{
    if(self = [super init])
    {
        M1obj = Mach1DecodeCAPI_create();
    }
    return self;
}

-(void)dealloc {
    Mach1DecodeCAPI_delete(M1obj);
}

-(void) setAngularSettingsType:(AngularSettingsType)type {
    Mach1DecodeCAPI_setAngularSettingsType(M1obj, (int)type);
}

-(void) beginBuffer {
    Mach1DecodeCAPI_beginBuffer(M1obj);
}
-(void) endBuffer {
    Mach1DecodeCAPI_endBuffer(M1obj);
}

-(NSNumber*) getCurrentTime {
    NSNumber* t = [NSNumber numberWithLong:Mach1DecodeCAPI_getCurrentTime(M1obj)];
    return t;
}
-(NSString*) getLog {
    NSString* str = [NSString stringWithFormat:@"%s", Mach1DecodeCAPI_getLog(M1obj)];
    return str;
}

-(NSMutableArray*) horizonAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex {
    float* data = Mach1DecodeCAPI_horizonAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);
    
    NSMutableArray *array = [[NSMutableArray alloc] init];
    for (int i=0; i<10; i++) {
        [array addObject:[NSNumber numberWithFloat:data[i]]];
    }
    return array;
}
-(NSMutableArray*) horizonPairsAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex {
    float* data = Mach1DecodeCAPI_horizonPairsAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);
    
    NSMutableArray *array = [[NSMutableArray alloc] init];
    for (int i=0; i<8; i++) {
        [array addObject:[NSNumber numberWithFloat:data[i]]];
    }
    return array;
}
-(NSMutableArray*) spatialAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex {
    float* data = Mach1DecodeCAPI_spatialAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);

    NSMutableArray *array = [[NSMutableArray alloc] init];
    for (int i=0; i<18; i++) {
        [array addObject:[NSNumber numberWithFloat:data[i]]];
    }
    return array;
}
-(NSMutableArray*) spatialAltAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex {
    float* data = Mach1DecodeCAPI_spatialAltAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);

    NSMutableArray *array = [[NSMutableArray alloc] init];
    for (int i=0; i<18; i++) {
        [array addObject:[NSNumber numberWithFloat:data[i]]];
    }
    return array;
}
-(NSMutableArray*) spatialPairsAlgo:(float)Yaw :(float)Pitch :(float)Roll :(int)bufferSize :(int)sampleIndex {
    float* data = Mach1DecodeCAPI_spatialPairsAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);
 
    NSMutableArray *array = [[NSMutableArray alloc] init];
    for (int i=0; i<16; i++) {
        [array addObject:[NSNumber numberWithFloat:data[i]]];
    }
    return array;
}

-(NSMutableArray*) horizonAlgo:(float)Yaw :(float)Pitch :(float)Roll{
    NSMutableArray *array = [self horizonAlgo:Yaw :Pitch :Roll :0 :0];
    return array;
}
-(NSMutableArray*) horizonPairsAlgo:(float)Yaw :(float)Pitch :(float)Roll{
    NSMutableArray *array = [self horizonPairsAlgo:Yaw :Pitch :Roll :0 :0];
    return array;
}
-(NSMutableArray*) spatialAlgo:(float)Yaw :(float)Pitch :(float)Roll{
    NSMutableArray *array = [self spatialAlgo:Yaw :Pitch :Roll :0 :0];
    return array;
}
-(NSMutableArray*) spatialAltAlgo:(float)Yaw :(float)Pitch :(float)Roll{
    NSMutableArray *array = [self spatialAltAlgo:Yaw :Pitch :Roll :0 :0];
    return array;
}
-(NSMutableArray*) spatialPairsAlgo:(float)Yaw :(float)Pitch :(float)Roll{
    NSMutableArray *array = [self spatialPairsAlgo:Yaw :Pitch :Roll :0 :0];
    return array;
}

@end


