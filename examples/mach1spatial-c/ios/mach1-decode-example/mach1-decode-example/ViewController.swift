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
var m1obj = Mach1Decode()
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
            m1obj.setPlatformType(type: Mach1PlatformDefault)
            //Setup the expected spatial audio mix format for decoding
            m1obj.setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoSpatial)
            //Setup for the safety filter speed:
            //1.0 = no filter | 0.1 = slow filter
            m1obj.setFilterSpeed(filterSpeed: 1.0)
            
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
                let deviceYaw = -(attitude!.yaw * 180/Double.pi)
                let devicePitch = -(attitude!.pitch * 180/Double.pi)
                let deviceRoll = -(attitude!.roll * 180/Double.pi)
                print("Yaw: ", deviceYaw)
                print("Pitch: ", devicePitch)
                print("Roll: ", deviceRoll)

                // Please notice that you're expected to correct the angles you get from
                // the device's sensors to provide Mach1DecodeCAPI with accurate angles in accordance to documentation.
                // http://dev.mach1.tech/#mach1-internal-angle-standard
                
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

