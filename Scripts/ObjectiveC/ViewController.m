//
//  ViewController.m
//  3DMusicPlayer
//


#define kUpdateInterval (1.0f / 60.0f)
#define radiansToDegrees(x) (180/M_PI)*x

#import "ViewController.h"
#import <CoreMotion/CoreMotion.h>
#import <AVFoundation/AVFoundation.h>
#import "MusicPlayerItem.h"

@interface ViewController ()
@property (strong, nonatomic) CMMotionManager  *motionManager;
@property (strong, nonatomic) NSOperationQueue *queue;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *yViewHeightConstraint;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *xViewHeightConstraint;
@property (strong, nonatomic) NSArray  *playerItems;
@end

@implementation ViewController

- (void)viewDidLoad {
    
    [super viewDidLoad];
    self.motionManager = [[CMMotionManager alloc]  init];
    self.queue         = [[NSOperationQueue alloc] init];
    
    self.motionManager.accelerometerUpdateInterval = kUpdateInterval;

    /*To use MusicPlayerItem, just create object set below properties on that
     @property (assign, nonatomic) CGPoint  audioAngle;
     @property (assign, nonatomic) CGPoint  minAngle;
     @property (assign, nonatomic) CGPoint  maxAngle;
     @property (assign, nonatomic) CGPoint  minFalloff;
     @property (assign, nonatomic) CGPoint  maxFalloff;
     I have used same name as per your snippets
     & call play.
     These objects will keep playing media file and will fluctuate volume depending upon x,y angles provided by motion manager.
     */
    NSURL * url = [[NSBundle mainBundle] URLForResource:@"AMB_Bedroom_000" withExtension:@"mp3"];
    MusicPlayerItem * item = [[MusicPlayerItem alloc] init];
    item.audioAngle = CGPointMake(0, 0);
    [item setAudioURL:url];
    [item play];
    
    url = [[NSBundle mainBundle] URLForResource:@"AMB_Bedroom_090" withExtension:@"mp3"];
    MusicPlayerItem * item1 = [[MusicPlayerItem alloc] init];
    item1.audioAngle = CGPointMake(90, 0);
    [item1 setAudioURL:url];
    [item1 play];
    
    url = [[NSBundle mainBundle] URLForResource:@"AMB_Bedroom_180" withExtension:@"mp3"];
    MusicPlayerItem * item2 = [[MusicPlayerItem alloc] init];
    item2.audioAngle = CGPointMake(180, 0);
    [item2 setAudioURL:url];
    [item2 play];
    
    url = [[NSBundle mainBundle] URLForResource:@"AMB_Bedroom_270" withExtension:@"mp3"];
    MusicPlayerItem * item3 = [[MusicPlayerItem alloc] init];
    item3.audioAngle = CGPointMake(270, 0);
    [item3 setAudioURL:url];
    [item3 play];
    
    self.playerItems = @[item,item1,item2,item3];
    
    //Motion manager which keeps on updating about euler angle changes
    [self.motionManager startDeviceMotionUpdatesToQueue:self.queue withHandler:^(CMDeviceMotion * _Nullable motion, NSError * _Nullable error) {
        
        dispatch_async(dispatch_get_main_queue(), ^{
            
            CMQuaternion quat = self.motionManager.deviceMotion.attitude.quaternion;
            CGFloat yAngle = radiansToDegrees(atan2(2*(quat.y*quat.w - quat.x*quat.z), 1 - 2*quat.y*quat.y - 2*quat.z*quat.z)) ;
            CGFloat xAngle = radiansToDegrees(atan2(2*(quat.x*quat.w + quat.y*quat.z), 1 - 2*quat.x*quat.x - 2*quat.z*quat.z));

            if(xAngle < 0){
                
                xAngle = 360 +xAngle;
            }

            if(yAngle < 0){
                
                yAngle = 360 +yAngle;
            }
            [self updateYAngleBar:yAngle];
            [self updateXAngleBar:xAngle];

            NSLog(@"X- %f, Y - %f",xAngle,yAngle);
            NSDictionary * dict = @{@"xAngle":[NSNumber numberWithFloat:xAngle],
                                    @"yAngle":[NSNumber numberWithFloat:yAngle]};
            [[NSNotificationCenter defaultCenter] postNotificationName:DeviceMotionUpdateNotificaion object:dict];
        });
    }];
}

//updates angle bar across Y axis
- (void)updateYAngleBar:(float) volume {
    
    self.yViewHeightConstraint.constant = (volume/360)*([[UIScreen mainScreen] bounds].size.height-100);;
    [self.view layoutIfNeeded];
}

//updates angle bar across X axis
- (void)updateXAngleBar:(float) volume {

    self.xViewHeightConstraint.constant = (volume/360)*([[UIScreen mainScreen] bounds].size.height-100);;
    [self.view layoutIfNeeded];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

@end
