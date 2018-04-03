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
import AudioKit

var motionManager = CMMotionManager()
var stereoPlayer = AVAudioPlayer()
var m1obj = Mach1Decode()
var stereoActive = false
var isYawActive = true
var isPitchActive = false
var isRollActive = false

var players: [AKPlayer] = []

var mixer: AKMixer = AKMixer()

class ViewController: UIViewController {
    
    
    
    @IBOutlet weak var yaw: UILabel!
    @IBOutlet weak var pitch: UILabel!
    @IBOutlet weak var roll: UILabel!
    @IBAction func playButton(_ sender: Any) {
        //        if (!m1LPlayer0.isPlaying){
        let shortStartDelay:TimeInterval = 0.1
        
        
        var startTime = AVAudioTime.now() + 0.25
        for audioPlayer in players {
            audioPlayer.play(at: startTime)
        }
        
        
        
        //stereoPlayer.play()
        print("isPlaying")
        //        }else{
        //        }
    }
    @IBAction func stopButton(_ sender: Any) {
        for i in 0...7 {
            do {
                players[i * 2].stop()
                players[i * 2 + 1].stop()
            }
        }
        stereoPlayer.pause()
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
        // Do any additional setup after loading the view, typically from a nib.
        
        //TODO: load multichannel audio instead
        //            audioPlayer = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "Guitar-8ch", ofType: "wav")!))
        
        mixer = AKMixer()
        
        mixer.volume = 1.0
        AudioKit.output = mixer
        
        do {
            try AudioKit.start()
        } catch {
            print (error)
        }
        
        do {
            for i in 0...7 {
                players.append(try AKPlayer(url: URL.init(fileURLWithPath: Bundle.main.path(forResource: "00" + String(i), ofType: "aif")!))!)
                players.append(try AKPlayer(url: URL.init(fileURLWithPath: Bundle.main.path(forResource: "00" + String(i), ofType: "aif")!))!)
                
                players[i * 2].isLooping = true
                players[i * 2 + 1].isLooping = true
                
                players[i * 2].pan = -1.0;
                players[i * 2 + 1].pan = 1.0;
                
                
                players[i * 2].outputNode.connect(to: mixer.inputNode)
                players[i * 2 + 1].outputNode.connect(to: mixer.inputNode)
            }
            
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
                if let attitude = motion?.attitude {
                    //Device orientation management
                    let deviceYaw = attitude.yaw * 180/M_PI
                    let devicePitch = attitude.pitch * 180/M_PI
                    //                    let devicePitch = 0.0
                    let deviceRoll = attitude.roll * 180/M_PI
                    //                    let deviceRoll = 0.0
                    //                    print("Yaw: ", deviceYaw)
                    //                    print("Pitch: ", devicePitch)
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
                    let decodeArray: [Float]  = m1obj.spatialAlgo(Yaw: Float(deviceYaw), Pitch: Float(devicePitch), Roll: Float(deviceRoll))
                    //                    print(decodeArray)
                    
                    //Use each coeff to decode multichannel Mach1 Spatial mix
                    for i in 0...7 {
                        players[i * 2].volume = Double(decodeArray[i * 2])
                        players[i * 2 + 1].volume = Double(decodeArray[i * 2 + 1])
                        
                        print(String(players[i * 2].currentTime) + " ; " + String(i * 2))
                        print(String(players[i * 2 + 1].currentTime) + " ; " + String(i * 2 + 1))
                    }
                    
                    //print coeffs
                    
                }
            })
            print("Device motion started")
        } else {
            print("Device motion unavailable");
        }
        
    }
    
}

