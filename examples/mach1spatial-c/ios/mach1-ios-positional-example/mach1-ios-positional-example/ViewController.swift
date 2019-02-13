//
//  ViewController.swift
//  mach1-ios-example
//
//  Created by Dylan Marcus on 2/19/18.
//  Copyright © 2018 Mach1. All rights reserved.
//

import UIKit
import CoreMotion
import AVFoundation

var motionManager = CMMotionManager()
var stereoPlayer = AVAudioPlayer()
var m1obj = Mach1DecodePositional()
var stereoActive = false
var isYawActive = true
var isPitchActive = false
var isRollActive = false
var isPlaying = false

private var audioEngine: AVAudioEngine = AVAudioEngine()
private var mixer: AVAudioMixerNode = AVAudioMixerNode()
var players: [AVAudioPlayer] = []

class ViewController: UIViewController {
    
    @IBOutlet weak var yaw: UILabel!
    @IBOutlet weak var pitch: UILabel!
    @IBOutlet weak var roll: UILabel!
    @IBAction func playButton(_ sender: Any) {
        if !isPlaying {
            var startDelayTime = 1.0
            var now = players[0].deviceCurrentTime
            var startTime = now + startDelayTime
            print (startTime)
            for audioPlayer in players {
                audioPlayer.play(atTime: startTime)
            }
            //stereoPlayer.play()
            print("isPlaying")
            isPlaying = true
        }
    }
    @IBAction func stopButton(_ sender: Any) {
        for audioPlayer in players {
            audioPlayer.stop()
        }
        isPlaying = false
        //stereoPlayer.stop()
        // prep files for next play
        for i in 0...7 {
            players[i * 2].prepareToPlay()
            players[i * 2 + 1].prepareToPlay()
        }
        //stereoPlayer.prepareToPlay()
    }
    @IBAction func staticStereoActive(_ sender: Any) {
        stereoActive = !stereoActive
    }
    @IBAction func yawActive(_ sender: Any) {
        isYawActive = !isYawActive
    }
    @IBAction func pitchActive(_ sender: Any) {
        isPitchActive = !isPitchActive
    }
    @IBAction func rollActive(_ sender: Any) {
        isRollActive = !isRollActive
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        
        do {
            for i in 0...7 {
                //load in the individual streams of audio from a Mach1 Spatial encoded audio file
                //this example assumes you have decoded the multichannel (8channel) audio file into individual streams
                players.append(try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "00" + String(i), ofType: "aif")!)))
                players.append(try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "00" + String(i), ofType: "aif")!)))
                
                players[i * 2].numberOfLoops = 10
                players[i * 2 + 1].numberOfLoops = 10
                
                //the Mach1Decode function 8*2 channels to correctly recreate the stereo image
                players[i * 2].pan = -1.0;
                players[i * 2 + 1].pan = 1.0;
                
                players[i * 2].prepareToPlay()
                players[i * 2 + 1].prepareToPlay()
                
            }
            
            //Mach1 Decode Setup
            //Setup the correct angle convention for orientation Euler input angles
            m1obj.setPlatformType(type: Mach1PlatformiOSLandscape)
            //Setup the expected spatial audio mix format for decoding
            m1obj.setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoSpatial)
            //Setup for the safety filter speed:
            //1.0 = no filter | 0.1 = slow filter
            m1obj.setFilterSpeed(filterSpeed: 1.0)
            
            //Mach1 Decode Positional Setup
            //Advanced Setting: used for blending 2 m1obj for crafting room ambiences
            m1obj.setUseBlendMode(useBlendMode: false);
            //Advanced Setting: ignore movements on height plane
            m1obj.setIgnoreTopBottom(ignoreTopBottom: false);
            //Setting: mute audio when setCameraPosition position is outside of m1obj volume
            //based on setDecoderAlgoPosition & setDecoderAlgoScale
            m1obj.setMuteWhenOutsideObject(muteWhenOutsideObject: false);
            //Setting: mute audio when setCameraPosition position is inside of m1obj volume
            //based on setDecoderAlgoPosition & setDecoderAlgoScale
            m1obj.setMuteWhenInsideObject(muteWhenInsideObject: false);
            //Setting: turn on/off distance attenuation of m1obj
            m1obj.setUseFalloff(useFalloff: true);
            //Advanced Setting: when on, positional rotation is calculated from the closest point
            //of the m1obj's volume and not rotation from the center of m1obj.
            //use this if you want the positional rotation tracking to be from a plane instead of from a point
            m1obj.setUseClosestPointRotationMuteInside(bool: false);
            //Setting: on/off yaw rotations from position
            m1obj.setUseYawForRotation(bool: true);
            //Setting: on/off pitch rotations from position
            m1obj.setUsePitchForRotation(bool: false);
            //Setting: on/off roll rotations from position
            m1obj.setUseRollForRotation(bool: false);
            
//            m1obj.setCameraPosition(ConvertToMach1Point3D(camera.transform.position));
//            m1obj.setCameraRotationQuat(ConvertToMach1Point4D(camera.transform.rotation));
//            m1obj.setDecoderAlgoPosition(ConvertToMach1Point3D(gameObject.transform.position));
//            m1obj.setDecoderAlgoRotationQuat(ConvertToMach1Point4D(gameObject.transform.rotation));
//            m1obj.setDecoderAlgoScale(ConvertToMach1Point3D(gameObject.transform.lossyScale));
//            m1obj.evaluatePostionResults();
            
        } catch {
            print (error)
        }
        
        
        //Static Stereo
        do{
            stereoPlayer = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "stereo", ofType: "wav")!))
        } catch {
            print(error)
        }
        stereoPlayer.prepareToPlay()
        print(stereoPlayer)
        
        //TODO: split audio channels for independent coeffs from our lib
        //let channelCount = audioPlayer.numberOfChannels
        //print("Channel Count: ", channelCount)
        
        //Allow audio to play when app closes
        let audioSession = AVAudioSession.sharedInstance()
        do {
            try audioSession.setCategory(AVAudioSessionCategoryPlayback)
        } catch {
            print(error)
        }
        
        // Ensure to keep a strong reference to the motion manager otherwise you won't get updates
        if motionManager.isDeviceMotionAvailable == true {
            motionManager.deviceMotionUpdateInterval = 0.01;
            let queue = OperationQueue()
            motionManager.startDeviceMotionUpdates(to: queue, withHandler: { [weak self] (motion, error) -> Void in
                
                // Get the attitudes of the device
                let attitude = motion?.attitude
                //Device orientation management
                var deviceYaw = attitude!.yaw * 180/M_PI
                var devicePitch = attitude!.pitch * 180/M_PI
                //                    let devicePitch = 0.0
                var deviceRoll = attitude!.roll * 180/M_PI
                //                    let deviceRoll = 0.0
                //                    print("Yaw: ", deviceYaw)
                //                    print("Pitch: ", devicePitch)

                // Please notice that you're expected to correct the correct the angles you get from
                // the device's sensors to provide M1 Library with accurate angles in accordance to documentation.
                // (documentation URL here)
                switch UIDevice.current.orientation{
                    case .portrait:
                        deviceYaw += 90
                        devicePitch -= 90
                    case .portraitUpsideDown:
                        deviceYaw -= 90
                        devicePitch += 90
                    case .landscapeLeft:
                        deviceRoll += 90
                    case .landscapeRight:
                        deviceYaw += 180
                        deviceRoll -= 90
//                    default:
                    
                    default: break
                    //
                }
                
                DispatchQueue.main.async() {
                    self?.yaw.text = String(deviceYaw)
                    self?.pitch.text = String(devicePitch)
                    self?.roll.text = String(deviceRoll)
                }
                //Mute stereo if off
                if (stereoActive) {
                    stereoPlayer.setVolume(1.0, fadeDuration: 0.1)
                } else if (!stereoActive) {
                    stereoPlayer.setVolume(0.0, fadeDuration: 0.1)
                }
                
                //Send device orientation to m1obj with the preferred algo
                m1obj.beginBuffer()
                let decodeArray: [Float]  = m1obj.decode(Yaw: Float(deviceYaw), Pitch: Float(devicePitch), Roll: Float(deviceRoll))
                m1obj.endBuffer()
                //                    print(decodeArray)
                
                //Use each coeff to decode multichannel Mach1 Spatial mix
                for i in 0...7 {
                    players[i * 2].setVolume(Float(decodeArray[i * 2]), fadeDuration: 0)
                    players[i * 2 + 1].setVolume(Float(decodeArray[i * 2 + 1]), fadeDuration: 0)
                    
                    print(String(players[i * 2].currentTime) + " ; " + String(i * 2))
                    print(String(players[i * 2 + 1].currentTime) + " ; " + String(i * 2 + 1))
                }
                
                
            })
            print("Device motion started")
        } else {
            print("Device motion unavailable");
        }
        
    }
    
}

