//
//  MusicPlayerItem.m
//  3DMusicPlayer
//


#import "MusicPlayerItem.h"

@interface MusicPlayerItem()
@property (strong, nonatomic) AVPlayer *player;

@end

@implementation MusicPlayerItem
- (instancetype)init
{
    self = [super init];
    if (self) {
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceMotionUpdate:) name:DeviceMotionUpdateNotificaion object:nil];
        
        //Default values/ these values can be set from outside
        self.audioAngle = CGPointMake(60, 60);
        self.minAngle = CGPointMake(1, 1);
        self.maxAngle = CGPointMake(360, 360);
        self.minFalloff = CGPointMake(1, 1);
        self.maxFalloff = CGPointMake(1, 1);

    }
    return self;
}

-(void)setAudioURL:(NSURL *)audioURL{
    
    _audioURL = audioURL;
    self.player = [[AVPlayer alloc]initWithURL:_audioURL];

}

-(void)deviceMotionUpdate:(NSNotification *)note{

    NSDictionary * dict= note.object;
    CGFloat xAngle = [[dict objectForKey:@"xAngle"] floatValue];
    CGFloat yAngle = [[dict objectForKey:@"yAngle"] floatValue];
    //calculate audio level using all values and pass volume
    
    
    if (yAngle > self.audioAngle.y + self.minAngle.y && yAngle < self.audioAngle.y +  self.maxAngle.y && xAngle > self.audioAngle.x + self.minAngle.x && xAngle < self.audioAngle.x +  self.maxAngle.x) {
        //
        float yVol = 0;
        float xVol = 0;
        // calculate y volume, if attuation is greater than 0 than add a falloff
        if (yAngle < self.audioAngle.y){
            yVol = 1;
            if (self.minFalloff.y > 0){
                yVol -= (fabs((self.audioAngle.y - yAngle) / (self.minAngle.y)));
            }
        }
        else {
            yVol = 1;
            if (self.maxFalloff.y > 0){
                yVol -= (fabs ((yAngle - self.audioAngle.y) / (self.maxAngle.y)));
            }
                
        }
        
        // calculate the x volume
        if (xAngle < self.audioAngle.x){
            xVol = 1;
            if (self.minFalloff.x > 0){
                xVol -= (fabs ((self.audioAngle.x - xAngle) / (self.minAngle.x)));
            }
        }
        else {
            xVol = 1;
            if (self.maxFalloff.x > 0){
                xVol -= (fabs ((xAngle - self.audioAngle.x) / (self.maxAngle.x)));
            }
        }
        
        float finalvol = yVol * xVol;
       [self adjustAudioLevel:finalvol];
    } else {
        [self adjustAudioLevel:0.0];
    }
}

-(void)adjustAudioLevel:(CGFloat)volume{
    
    NSArray *audioTracks = [self.player.currentItem.asset tracksWithMediaType:AVMediaTypeAudio];
    // Mute all the audio tracks
    NSMutableArray *allAudioParams = [NSMutableArray array];
    for (AVAssetTrack *track in audioTracks) {
        AVMutableAudioMixInputParameters *audioInputParams =[AVMutableAudioMixInputParameters audioMixInputParameters];
        [audioInputParams setVolume:volume atTime:kCMTimeZero];
        [audioInputParams setTrackID:[track trackID]];
        [allAudioParams addObject:audioInputParams];
    }
    AVMutableAudioMix *audioMix = [AVMutableAudioMix audioMix];
    [audioMix setInputParameters:allAudioParams];
    [[self.player currentItem] setAudioMix:audioMix];
}

-(void)play{
    
    [self.player play];
}
@end
