#include "CustomVideoPlayer.h"

#include "CVTextureCache.h"
#include "CMVideoSampling.h"
#include "GlesHelper.h"

#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>


static void* _ObserveItemStatusContext = (void*)0x1;
static void* _ObservePlayerItemContext = (void*)0x2;


@implementation CustomVideoPlayerView
+ (Class)layerClass
{
    return [AVPlayerLayer class];
}

- (AVPlayer*)player
{
    return [(AVPlayerLayer*)[self layer] player];
}


- (void)setPlayer:(AVPlayer*)player
{
    [(AVPlayerLayer*)[self layer] setPlayer:player];
}

- (void)dealloc
{
	self.player = nil;
}
@end


@implementation CustomVideoPlayer
{
    AVPlayerItem*   _playerItem;
    AVPlayer*       _player;

    AVAssetReader*              _reader;
    AVAssetReaderTrackOutput*   _videoOut;
    AVPlayerItemVideoOutput*    videoOutput;

    CMSampleBufferRef           _cmSampleBuffer;
    CMSampleBufferRef           _cmSampleBuffer2;
    CMVideoSampling             _videoSampling;
    CMVideoSampling             _videoSamplingDummy;
    
    unsigned int _videoTexture;

    CMTime          _duration;
    CMTime          _curTime;
    CMTime          _curFrameTimestamp;
    CMTime          _lastFrameTimestamp;
    CGSize          _videoSize;

    BOOL            _playerReady;
    intptr_t        _textureID;

    // we need to have both because the order of asset/item getting ready is not strict
    BOOL            _assetReady;
    BOOL            _itemReady;
    BOOL            _error;
    
    BOOL _evertoon_was_unloaded;
}

@synthesize delegate;


- (BOOL)readyToPlay         { return _playerReady; }
- (CGSize)videoSize         { return _videoSize; }
- (CMTime)duration          { return _duration; }
- (float)durationSeconds    { return CMTIME_IS_VALID(_duration) ? (float)CMTimeGetSeconds(_duration) : 0.0f; }
- (float)curTimeSeconds     { return CMTIME_IS_VALID(_curTime) ? (float)
    CMTimeGetSeconds(_curTime) : 0.0f; }

// npot textures are supported on gles1 only with ext.
// so we support only gles2 for the sake of sanity;
+ (BOOL)CanPlayToTexture:(NSURL*)url    { return /*IsRunningWithGLES2() && [url isFileURL];*/  true; }


- (void)reportError:(NSError*)error category:(const char*)category
{
    ::printf_console("[%s]Error: %s\n", category, [[error localizedDescription] UTF8String]);
    ::printf_console("%s\n", [[error localizedFailureReason] UTF8String]);
}
- (void)reportErrorWithString:(const char*)error category:(const char*)category
{
    ::printf_console("[%s]Error: %s\n", category, error);
}

- (id)init
{
    if( (self = [super init]) )
    {
        _duration = _curTime = kCMTimeZero;
        _curFrameTimestamp = _lastFrameTimestamp = kCMTimeZero;
        _evertoon_was_unloaded = false;
    }
    return self;
}


- (void)cleanCache
{
    if(_videoSamplingDummy.cvTextureCache)
    {
        CFRelease(_videoSamplingDummy.cvTextureCache);
        _videoSamplingDummy.cvTextureCache = 0;
    }
}
- (void)cleanupCVTextureCache
{
    if(_cmSampleBuffer)
    {
        CFRelease(_cmSampleBuffer);
        _cmSampleBuffer = 0;
    }
    
    if(_cmSampleBuffer2)
    {
        CFRelease(_cmSampleBuffer2);
        _cmSampleBuffer2 = 0;
    }
    
    if(_videoSampling.cvImageBuffer)
    {
        CFRelease(_videoSampling.cvImageBuffer);
        _videoSampling.cvImageBuffer = 0;
    }
    if(_videoSampling.cvTextureCacheTexture)
    {
        CFRelease(_videoSampling.cvTextureCacheTexture);
        _videoSampling.cvTextureCacheTexture = 0;
    }
   
    
    
    if(_videoSampling.cvTextureCache !=0)
    {
        if(_videoSamplingDummy.cvTextureCache)
        {
            CFRelease(_videoSamplingDummy.cvTextureCache);
            _videoSamplingDummy.cvTextureCache = 0;
        }
        _videoSamplingDummy.cvTextureCache = _videoSampling.cvTextureCache;
        _videoSampling.cvTextureCache = 0;
    }
    
    //CMVideoSampling_Uninitialize(&_videoSampling);
}

- (void)cleanupAssetReader
{
	if(_reader)
		[_reader cancelReading];

	_reader = nil;
	_videoOut = nil;
}

- (void)cleanupPlayer
{
	if(_player)
	{
		[[NSNotificationCenter defaultCenter] removeObserver:self name:AVAudioSessionRouteChangeNotification object:nil];
		[_player.currentItem removeObserver:self forKeyPath:@"status"];
		[_player removeObserver:self forKeyPath:@"currentItem"];
		[_player pause];
		_player = nil;
	}

	if(_playerItem)
	{
		[[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:_playerItem];
		_playerItem = nil;
	}
}


- (void)unloadPlayer
{
    
    _evertoon_was_unloaded = true;
    
    [self cleanupCVTextureCache];
    [self cleanupAssetReader];
    [self cleanupPlayer];
    
    
    if(videoOutput)
    {
        videoOutput = nil;
    }
    
    if(_videoTexture != 0)
    {
        
        GLuint handle =_videoTexture;
        glDeleteTextures(1, &handle);
        _videoTexture = 0;
    }

    _videoSize = CGSizeMake(0,0);
    _duration = _curTime = kCMTimeZero;
    _curFrameTimestamp = _lastFrameTimestamp = kCMTimeZero;

    self->_playerReady = self->_assetReady = self->_itemReady = NO;
}


- (BOOL)loadVideo:(NSURL*)url
{
    _evertoon_was_unloaded = false;
    _error = false;
    
    AVURLAsset* asset = [AVURLAsset URLAssetWithURL:url options:nil];
    if(!asset) return NO;

	NSArray* requestedKeys = @[@"tracks", @"playable"];
	[asset loadValuesAsynchronouslyForKeys:requestedKeys completionHandler:
		^{
            dispatch_async(dispatch_get_main_queue(), ^{ [self prepareAsset:asset withKeys:requestedKeys : url]; });
		}
	];
	return YES;
}

- (BOOL)_play:(CustomVideoPlayerView*)view
{
    if(!_playerReady)
        return NO;

    if(view)    view.player = _player;
    else        [self prepareReader];

    // do not do seekTo and setRate here, it seems that http streaming may hang sometimes if you do so. go figure
    _curFrameTimestamp = _lastFrameTimestamp = kCMTimeZero;
    [_player play];

    return YES;
}

- (BOOL)_playloop:(CustomVideoPlayerView*)view
{
    if(!_playerReady)
        return NO;
    
    
    // do not do seekTo and setRate here, it seems that http streaming may hang sometimes if you do so. go figure
    _curFrameTimestamp = _lastFrameTimestamp = kCMTimeZero;
    [_player play];
    
    return YES;
}

- (void)setTextureID:(intptr_t)id { _textureID = id;}

- (BOOL)playToView:(CustomVideoPlayerView*)view   { return [self _play:view]; }
- (BOOL)playToTexture                       { return [self _play:nil]; }
- (BOOL)playToTextureloop                       { return [self _playloop:nil]; }

- (BOOL)isPlaying	{ return _playerReady && _player.rate != 0.0f; }
- (BOOL)getError           { return _error;}

- (void)pause	{ if(_playerReady && _player.rate != 0.0f) [_player pause]; }
- (void)resume	{ if(_playerReady && _player.rate == 0.0f) [_player play]; }

- (void)rewind						{ [self seekToTimestamp:kCMTimeZero]; }
- (void)seekTo:(float)timeSeconds	{ [self seekToTimestamp:CMTimeMakeWithSeconds(timeSeconds,  _player.currentItem.duration.timescale)]; }
- (void)setSpeed:(float)fSpeed { [_player setRate:fSpeed ]; }
- (void)seekToTimestamp:(CMTime)time
{
    
    
    if (!CMTIME_IS_INVALID(time)) {
        
        [_player seekToTime: time
            toleranceBefore: kCMTimeZero
             toleranceAfter: kCMTimeZero
          completionHandler: ^(BOOL finished)   {
              //
          }];
    }
    
    [self prepareReaderPos:time];
    _curFrameTimestamp = _lastFrameTimestamp = time;
    
}




- (intptr_t)curFrameTexture
{
    AVURLAsset* asset = (AVURLAsset*)_playerItem.asset;
    if(![asset.URL isFileURL])
    {
        intptr_t curTex = _videoTexture;

	CMTime time = [_player currentTime];
	if(CMTimeCompare(time, _curTime) == 0 )
		return curTex;

        
        if(_AudioRouteWasChanged )
        {
            _AudioRouteWasChanged = false;
            [_player setRate: 1.0f]; // _player.rate = 1.0f;
        }
        
        _curTime = time;
        
        unsigned char* pixelBufferBaseAddress = NULL;
        CVPixelBufferRef pixelBuffer;
        
        
        pixelBuffer = [videoOutput copyPixelBufferForItemTime:_player.currentItem.currentTime itemTimeForDisplay:nil];
        
        
        CVPixelBufferLockBaseAddress(pixelBuffer, 0);
        pixelBufferBaseAddress = (unsigned char*)CVPixelBufferGetBaseAddress(pixelBuffer);
        size_t w = CVPixelBufferGetWidth( pixelBuffer);
        size_t h = CVPixelBufferGetHeight( pixelBuffer);
        
        CGSize size = CGSizeMake(w, h);
        
        if( _videoSize.width != size.width)
        {
            _videoSize = size;
            
        }
        
        
        
        if (NULL != pixelBufferBaseAddress)
        {
            
            
            if(UnitySelectedRenderingAPI() == apiMetal || UnitySelectedRenderingAPI() == apiOpenGLES3 || UnitySelectedRenderingAPI() == apiOpenGLES2)
            {
                if(_videoSampling.cvTextureCacheTexture)
                {
                    CFRelease(_videoSampling.cvTextureCacheTexture);
                    FlushCVTextureCache(_videoSampling.cvTextureCache);
                }
                _videoSampling.cvTextureCacheTexture = CreateTextureFromCVTextureCache(_videoSampling.cvTextureCache, pixelBuffer, w, h);
                if(_videoSampling.cvTextureCacheTexture)
                    curTex = GetTextureFromCVTextureCache(_videoSampling.cvTextureCacheTexture);
                
                
                if(UnitySelectedRenderingAPI() == apiOpenGLES2 || UnitySelectedRenderingAPI() == apiOpenGLES3)
                {
                    (glBindTexture(GL_TEXTURE_2D, (GLuint)curTex));
                    (glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                    (glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                    (glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                    (glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                    (glBindTexture(GL_TEXTURE_2D, 0));
                }
                
                CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
                
                // curTex = retTex;
            }
            else
            {
                if (0 == _videoTexture) {
                    
                    GLuint handle;
                    glGenTextures(1, &handle);
                    glBindTexture(GL_TEXTURE_2D, handle);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    
                    _videoTexture = handle;
                }
                
                
                
                glBindTexture(GL_TEXTURE_2D, _videoTexture);
                const size_t bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer);
                
                if (bytesPerRow / 4 == (GLsizei)w) {
                    
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) w, (GLsizei) h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixelBufferBaseAddress);
                }
                else {
                    
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) w, (GLsizei) h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
                    
                    
                    for (int i = 0; i < _videoSize.height; ++i) {
                        GLubyte* line = pixelBufferBaseAddress + i * bytesPerRow;
                        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, (GLsizei) w, 1, GL_BGRA, GL_UNSIGNED_BYTE, line);
                    }
                }
                
                
                glBindTexture(GL_TEXTURE_2D, 0);
                
                
                CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
                
                curTex = _videoTexture;
            }
            
            
        }
        
        if (pixelBuffer) {
            CFRelease(pixelBuffer);
        }
        
        return curTex;
        
    }
    else
    {
        if(!_reader)
            return 0;
        
     
        

        intptr_t curTex = CMVideoSampling_LastSampledTexture(&_videoSampling);
        
        CMTime time = [_player currentTime];
        if(CMTimeCompare(time, _curTime) == 0 || _reader.status != AVAssetReaderStatusReading)
            return curTex;
        
        
        
        // if we have changed audio route and due to current category apple decided to pause playback - resume automatically
        if(_AudioRouteWasChanged )
        {
            _AudioRouteWasChanged = false;
            [_player setRate: 1.0f]; // _player.rate = 1.0f;
        }
        


        
        _curTime = time;
        while(_reader.status == AVAssetReaderStatusReading && CMTimeCompare(_curFrameTimestamp, _curTime) <= 0)
        {
            if(_cmSampleBuffer2)
                CFRelease(_cmSampleBuffer2);
            
            _cmSampleBuffer2 = _cmSampleBuffer;
            
            // TODO: properly handle ending
            
            _cmSampleBuffer = [_videoOut copyNextSampleBuffer];
            if(_cmSampleBuffer == 0)
            {
                //[self cleanupCVTextureCache];
                return 0;
            }
            
            _curFrameTimestamp = CMSampleBufferGetPresentationTimeStamp(_cmSampleBuffer);
        }
        
        if(CMTimeCompare(_lastFrameTimestamp, _curFrameTimestamp) < 0)
        {
            _lastFrameTimestamp = _curFrameTimestamp;
            size_t w, h;
            if(_cmSampleBuffer)
                curTex = CMVideoSampling_SampleBuffer(&_videoSampling, _cmSampleBuffer, &w, &h);
            _videoSize = CGSizeMake(w, h);
        }
        
        return curTex;
        
    }
    
    
    return 0;
}

- (BOOL)setAudioVolume:(float)volume
{
    if(!_playerReady)
        return NO;
    
    
    if(_player != NULL)
        [_player setVolume:volume];
    

    NSArray* audio = [_playerItem.asset tracksWithMediaType:AVMediaTypeAudio];
    NSMutableArray* params = [NSMutableArray array];
    
    for(AVAssetTrack* track in audio)
    {
      
        AVMutableAudioMixInputParameters* inputParams = [AVMutableAudioMixInputParameters audioMixInputParameters];
        [inputParams setVolume:volume atTime:kCMTimeZero];
        [inputParams setTrackID:[track trackID]];
        [params addObject:inputParams];
    }

    AVMutableAudioMix* audioMix = [AVMutableAudioMix audioMix];
    [audioMix setInputParameters:params];

    [_playerItem setAudioMix:audioMix];
    
    

    return YES;
}

- (void)playerItemDidReachEnd:(NSNotification*)notification
{
    [delegate onPlayerDidFinishPlayingVideo];
}

static bool _AudioRouteWasChanged = false;
- (void)audioRouteChanged:(NSNotification*)notification
{
	_AudioRouteWasChanged = true;
}


- (void)observeValueForKeyPath:(NSString*)path ofObject:(id)object change:(NSDictionary*)change context:(void*)context
{
    BOOL reportPlayerReady = NO;

	if(context == _ObserveItemStatusContext)
	{
		AVPlayerStatus status = (AVPlayerStatus)[[change objectForKey:NSKeyValueChangeNewKey] integerValue];
		switch(status)
		{
			case AVPlayerStatusUnknown:
			break;

			case AVPlayerStatusReadyToPlay:
			{
				NSArray* video = [_playerItem.asset tracksWithMediaType:AVMediaTypeVideo];
				if([video count])
					_videoSize = [(AVAssetTrack*)[video objectAtIndex:0] naturalSize];
                
                

				_duration			= [_playerItem duration];
				_assetReady			= YES;
				reportPlayerReady	= _itemReady;
			}
			break;

            case AVPlayerStatusFailed:
            {
                AVPlayerItem *playerItem = (AVPlayerItem*)object;
                [self reportError:playerItem.error category:"prepareAsset"];
                _error = true;
            }
            break;
        }
    }
    else if(context == _ObservePlayerItemContext)
    {
        if( [change objectForKey:NSKeyValueChangeNewKey] != (id)[NSNull null] )
        {
            _itemReady = YES;
            reportPlayerReady = _assetReady;
        }
    }
    else
    {
        [super observeValueForKeyPath:path ofObject:object change:change context:context];
    }

    if(reportPlayerReady)
    {
        _playerReady = YES;
        [delegate onPlayerReady];
    }
}

- (void)prepareAsset:(AVAsset*)asset withKeys:(NSArray*)requestedKeys : (NSURL *)url
{
    if (_evertoon_was_unloaded) {
                NSLog(@"### Video already unloaded - ignoring.");
                return;
    }
    
    for(NSString* key in requestedKeys)
    {
        NSError* error = nil;
        AVKeyValueStatus keyStatus = [asset statusOfValueForKey:key error:&error];
        if(keyStatus == AVKeyValueStatusFailed)
        {
            [self reportError:error category:"prepareAsset"];
            return;
        }
    }

    if(!asset.playable)
    {
   
        [self reportErrorWithString:"Item cannot be played" category:"prepareAsset"];
        _error = true;
        return;
    }

	if(_playerItem)
	{
        
		[_playerItem removeObserver:self forKeyPath:@"status"];
		[[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:_playerItem];

		_playerItem = nil;
	}
    
  
   
    _playerItem = [AVPlayerItem playerItemWithAsset:asset];
    
	[_playerItem	addObserver:self forKeyPath:@"status"
					options:NSKeyValueObservingOptionInitial | NSKeyValueObservingOptionNew
					context:_ObserveItemStatusContext
	];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerItemDidReachEnd:)
										  name:AVPlayerItemDidPlayToEndTimeNotification object:_playerItem
	];

	if(!_player)
	{
      
		_player = [AVPlayer playerWithPlayerItem:_playerItem];
		[_player	addObserver:self forKeyPath:@"currentItem"
					options:NSKeyValueObservingOptionInitial | NSKeyValueObservingOptionNew
					context:_ObservePlayerItemContext
		];

		[_player setAllowsExternalPlayback:NO];

		// we want to subscribe to route change notifications, for that we need audio session active
		// and in case FMOD wasnt used up to this point it is still not active
		[[AVAudioSession sharedInstance] setActive:YES error:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(audioRouteChanged:)
											  name:AVAudioSessionRouteChangeNotification object:nil
		];
	}

    if(_player.currentItem != _playerItem)
        [_player replaceCurrentItemWithPlayerItem:_playerItem];
    else
    	[_player seekToTime:kCMTimeZero];
    
    
    
}

- (BOOL)prepareReader
{
    if(!_playerReady)
        return NO;

    [self cleanupAssetReader];

    AVURLAsset* asset = (AVURLAsset*)_playerItem.asset;
    
    if(![asset.URL isFileURL])
    {
        
    
  
        NSDictionary *settings = @{(id) kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA)};
        
        //AVPlayerItemVideoOutput *output = [[[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:settings] autorelease];
        AVPlayerItemVideoOutput *output = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:settings];
        videoOutput = output;
        
        
        //NSArray *videoTracks = [asset tracksWithMediaType:AVMediaTypeVideo];
        AVAssetTrack *videoTrack = [[[_playerItem tracks] objectAtIndex:0] assetTrack];;
        _videoSize = videoTrack.naturalSize;
        printf("\nprepareReader %d %d", (GLsizei)_videoSize.width, (GLsizei)_videoSize.height);
        _duration = [asset duration];
        
 
        NSArray * arrayTracks = [asset tracksWithMediaType:AVMediaTypeAudio];
        if (0 < [arrayTracks count]) {
     
            AVAssetTrack* assetTrackAudio = arrayTracks[0];
            
            AVMutableAudioMixInputParameters* audioInputParams = [AVMutableAudioMixInputParameters audioMixInputParameters];
            [audioInputParams setVolume:1.0f atTime:kCMTimeZero];
            [audioInputParams setTrackID:[assetTrackAudio trackID]];
            
            NSArray* audioParams = @[audioInputParams];
            AVMutableAudioMix* audioMix = [AVMutableAudioMix audioMix];
            [audioMix setInputParameters:audioParams];
            
            AVPlayerItem* item = [_player currentItem];
            [item setAudioMix:audioMix];
           
        }
        
         [[_player currentItem] addOutput:videoOutput];
        
        
    }
    else
    {
       
        NSError* error = nil;
        _reader = [AVAssetReader assetReaderWithAsset:_playerItem.asset error:&error];
        if(error)
            [self reportError:error category:"prepareReader"];
        
        _reader.timeRange = CMTimeRangeMake(kCMTimeZero, _duration);
        AVAssetTrack* videoTrack = [[_playerItem.asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0];
        
        NSDictionary* options = @{ (NSString*)kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA) };
        _videoOut = [[AVAssetReaderTrackOutput alloc] initWithTrack:videoTrack outputSettings:options];
        _videoOut.alwaysCopiesSampleData = NO;
        
        if(![_reader canAddOutput:_videoOut])
        {
            [self reportErrorWithString:"canAddOutput returned false" category:"prepareReader"];
            _error = true;
            return NO;
        }
        [_reader addOutput:_videoOut];
        
        if(![_reader startReading])
        {
            [self reportError:[_reader error] category:"prepareReader"];
            _error = true;
            return NO;
        }
    
    }

	
    


	

    [self cleanupCVTextureCache];
    CMVideoSampling_Initialize(&_videoSampling);

    return NO;
}

- (BOOL)prepareReaderPos : (CMTime)time
{
    if(!_playerReady)
        return NO;
    
    [self cleanupAssetReader];
    
    AVURLAsset* asset = (AVURLAsset*)_playerItem.asset;
    if(![asset.URL isFileURL])
    {
        //[self reportErrorWithString:"non-file url. no video to texture." category:"prepareReader"];
        //return NO;
      /*  NSDictionary *settings = @{(id) kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA)};
        //AVPlayerItemVideoOutput *output = [[[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:settings] autorelease];
        AVPlayerItemVideoOutput *output = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:settings];
        videoOutput = output;
        
        
        NSArray *videoTracks = [asset tracksWithMediaType:AVMediaTypeVideo];
       // AVAssetTrack *videoTrack = videoTracks[0];
        AVAssetTrack *videoTrack = [[[_playerItem tracks] objectAtIndex:0] assetTrack];
        _videoSize = videoTrack.naturalSize;
        
        _duration = [asset duration];
        
        
        
        // ===== Audio =====
        // Get the first audio track
        NSArray * arrayTracks = [asset tracksWithMediaType:AVMediaTypeAudio];
        if (0 < [arrayTracks count]) {
            
            AVAssetTrack* assetTrackAudio = arrayTracks[0];
            
            AVMutableAudioMixInputParameters* audioInputParams = [AVMutableAudioMixInputParameters audioMixInputParameters];
            [audioInputParams setVolume:1.0f atTime:time];
            [audioInputParams setTrackID:[assetTrackAudio trackID]];
            
            NSArray* audioParams = @[audioInputParams];
            AVMutableAudioMix* audioMix = [AVMutableAudioMix audioMix];
            [audioMix setInputParameters:audioParams];
            
            AVPlayerItem* item = [_player currentItem];
            [item setAudioMix:audioMix];
            
        }
        
        [[_player currentItem] addOutput:videoOutput];*/
        
        
    }
    else
    {
        NSError* error = nil;
        _reader = [AVAssetReader assetReaderWithAsset:_playerItem.asset error:&error];
        if(error){
            [self reportError:error category:"prepareReader"];
            _error = true;
        }
        
        _reader.timeRange = CMTimeRangeMake(time, _duration);
        
        
        
        AVAssetTrack* videoTrack = [[_playerItem.asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0];
        
        NSDictionary* options = @{ (NSString*)kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA) };
        _videoOut = [[AVAssetReaderTrackOutput alloc] initWithTrack:videoTrack outputSettings:options];
        _videoOut.alwaysCopiesSampleData = NO;
        
        if(![_reader canAddOutput:_videoOut])
        {
            [self reportErrorWithString:"canAddOutput returned false" category:"prepareReader"];
            _error = true;
            return NO;
        }
        [_reader addOutput:_videoOut];
        
        if(![_reader startReading])
        {
            [self reportError:[_reader error] category:"prepareReader"];
            _error = true;
            return NO;
        }
        
    
    }
    
    
    //[self cleanupCVTextureCache];
    //CustomCMVideoSampling_Initialize(&_videoSampling);
    return NO;
}

@end
