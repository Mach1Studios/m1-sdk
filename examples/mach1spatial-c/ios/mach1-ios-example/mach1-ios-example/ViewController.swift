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
var audioPlayer = AVAudioPlayer()
var m1obj = Mach1Decode()

class ViewController: UIViewController {
    
    @IBOutlet weak var yaw: UILabel!
    @IBOutlet weak var pitch: UILabel!
    @IBOutlet weak var roll: UILabel!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        do {
            audioPlayer = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "sample", ofType: "aif")!))
            audioPlayer.prepareToPlay()
            print(audioPlayer)
            
            let channelCount = audioPlayer.numberOfChannels
            print("Channel Count: ", channelCount)
            
            //TODO: split audio channels for independent coeffs from our lib
            
            //Allow audio to play when app closes
            let audioSession = AVAudioSession.sharedInstance()
            do {
                try audioSession.setCategory(AVAudioSessionCategoryPlayback)
            } catch {
            }
            
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
                    let deviceRoll = attitude.roll * 180/M_PI
//                    print("Yaw: ", deviceYaw)
//                    print("Pitch: ", devicePitch)
                    DispatchQueue.main.async() {
                        self?.yaw.text = String(deviceYaw)
                        self?.pitch.text = String(devicePitch)
                        self?.roll.text = String(deviceRoll)
                        
                        
                        let array: [Float]  = m1obj.spatialAlgo(Yaw: Float(deviceYaw), Pitch: Float(devicePitch), Roll: Float(deviceRoll))
                        print(array)
                    }
                }
            })
            print("Device motion started")
        } else {
            print("Device motion unavailable");
        }
    }
    
    @IBAction func playButton(_ sender: Any) {
        audioPlayer.play()
        print("isPlaying")
    }
    @IBAction func stopButton(_ sender: Any) {
        if audioPlayer.isPlaying{
            audioPlayer.stop()
        } else {
        }
    }

}
