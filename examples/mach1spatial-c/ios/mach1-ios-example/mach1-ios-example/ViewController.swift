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
var m1LPlayer0 = AVAudioPlayer(), m1LPlayer1 = AVAudioPlayer(), m1LPlayer2 = AVAudioPlayer(), m1LPlayer3 = AVAudioPlayer(), m1LPlayer4 = AVAudioPlayer(), m1LPlayer5 = AVAudioPlayer(), m1LPlayer6 = AVAudioPlayer(), m1LPlayer7 = AVAudioPlayer()
var m1RPlayer0 = AVAudioPlayer(), m1RPlayer1 = AVAudioPlayer(), m1RPlayer2 = AVAudioPlayer(), m1RPlayer3 = AVAudioPlayer(), m1RPlayer4 = AVAudioPlayer(), m1RPlayer5 = AVAudioPlayer(), m1RPlayer6 = AVAudioPlayer(), m1RPlayer7 = AVAudioPlayer()

var m1obj = Mach1Decode()
var stereoActive = false
var isYawActive = true
var isPitchActive = false
var isRollActive = false

class ViewController: UIViewController {
    
    @IBOutlet weak var yaw: UILabel!
    @IBOutlet weak var pitch: UILabel!
    @IBOutlet weak var roll: UILabel!
    @IBAction func playButton(_ sender: Any) {
        if (!m1LPlayer0.isPlaying){
            let shortStartDelay:TimeInterval = 1.0
            m1LPlayer0.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1RPlayer0.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1LPlayer1.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1RPlayer1.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1LPlayer2.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1RPlayer2.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1LPlayer3.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1RPlayer3.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1LPlayer4.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1RPlayer4.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1LPlayer5.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1RPlayer5.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1LPlayer6.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1RPlayer6.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1LPlayer7.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
            m1RPlayer7.play(atTime: m1RPlayer7.deviceCurrentTime + shortStartDelay)
        
            stereoPlayer.play()
            print("isPlaying")
        }else{
            }
    }
    @IBAction func stopButton(_ sender: Any) {
        if (m1LPlayer0.isPlaying){
            m1LPlayer0.pause()
            m1RPlayer0.pause()
            m1LPlayer1.pause()
            m1RPlayer1.pause()
            m1LPlayer2.pause()
            m1RPlayer2.pause()
            m1LPlayer3.pause()
            m1RPlayer3.pause()
            m1LPlayer4.pause()
            m1RPlayer4.pause()
            m1LPlayer5.pause()
            m1RPlayer5.pause()
            m1LPlayer6.pause()
            m1RPlayer6.pause()
            m1LPlayer7.pause()
            m1RPlayer7.pause()
            
            stereoPlayer.pause()
        } else {
        }
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
        
        //Set device angularSettings in m1obj
        m1obj.setAngularSettingsType(type: AngularSettingsType.m1oFEasyCam)

        //TODO: load multichannel audio instead
//            audioPlayer = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "Guitar-8ch", ofType: "wav")!))
        do {
                m1LPlayer0 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "000", ofType: "aif")!))
                m1RPlayer0 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "000", ofType: "aif")!))
                m1LPlayer1 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "001", ofType: "aif")!))
                m1RPlayer1 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "001", ofType: "aif")!))
                m1LPlayer2 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "002", ofType: "aif")!))
                m1RPlayer2 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "002", ofType: "aif")!))
                m1LPlayer3 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "003", ofType: "aif")!))
                m1RPlayer3 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "003", ofType: "aif")!))
                m1LPlayer4 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "004", ofType: "aif")!))
                m1RPlayer4 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "004", ofType: "aif")!))
                m1LPlayer5 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "005", ofType: "aif")!))
                m1RPlayer5 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "005", ofType: "aif")!))
                m1LPlayer6 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "006", ofType: "aif")!))
                m1RPlayer6 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "006", ofType: "aif")!))
                m1LPlayer7 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "007", ofType: "aif")!))
                m1RPlayer7 = try AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: "007", ofType: "aif")!))
            }
        } catch {
            print (error)
        }
        
        m1LPlayer0.prepareToPlay()
        m1RPlayer0.prepareToPlay()
        m1LPlayer1.prepareToPlay()
        m1RPlayer1.prepareToPlay()
        m1LPlayer2.prepareToPlay()
        m1RPlayer2.prepareToPlay()
        m1LPlayer3.prepareToPlay()
        m1RPlayer3.prepareToPlay()
        m1LPlayer4.prepareToPlay()
        m1RPlayer4.prepareToPlay()
        m1LPlayer5.prepareToPlay()
        m1RPlayer5.prepareToPlay()
        m1LPlayer6.prepareToPlay()
        m1RPlayer6.prepareToPlay()
        m1LPlayer7.prepareToPlay()
        m1RPlayer7.prepareToPlay()
        
        m1LPlayer0.pan = -1.0;
        m1RPlayer0.pan = 1.0;
        m1LPlayer1.pan = -1.0;
        m1RPlayer1.pan = 1.0;
        m1LPlayer2.pan = -1.0;
        m1RPlayer2.pan = 1.0;
        m1LPlayer3.pan = -1.0;
        m1RPlayer3.pan = 1.0;
        m1LPlayer4.pan = -1.0;
        m1RPlayer4.pan = 1.0;
        m1LPlayer5.pan = -1.0;
        m1RPlayer5.pan = 1.0;
        m1LPlayer6.pan = -1.0;
        m1RPlayer6.pan = 1.0;
        m1LPlayer7.pan = -1.0;
        m1RPlayer7.pan = 1.0;
        
        print(m1LPlayer0)
        print(m1RPlayer0)
        print(m1LPlayer1)
        print(m1RPlayer1)
        print(m1LPlayer2)
        print(m1RPlayer2)
        print(m1LPlayer3)
        print(m1RPlayer3)
        print(m1LPlayer4)
        print(m1RPlayer4)
        print(m1LPlayer5)
        print(m1RPlayer5)
        print(m1LPlayer6)
        print(m1RPlayer6)
        print(m1LPlayer7)
        print(m1RPlayer7)
        
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
//                        let devicePitch = 0.0
//                        let deviceRoll = attitude.roll * 180/M_PI
                        let deviceRoll = 0.0
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
                    m1LPlayer0.volume = decodeArray[0]
                    m1RPlayer0.volume = decodeArray[1]
                    m1LPlayer1.volume = decodeArray[2]
                    m1RPlayer1.volume = decodeArray[3]
                    m1LPlayer2.volume = decodeArray[4]
                    m1RPlayer2.volume = decodeArray[5]
                    m1LPlayer3.volume = decodeArray[6]
                    m1RPlayer3.volume = decodeArray[7]
                    m1LPlayer4.volume = decodeArray[8]
                    m1RPlayer4.volume = decodeArray[9]
                    m1LPlayer5.volume = decodeArray[10]
                    m1RPlayer5.volume = decodeArray[11]
                    m1LPlayer6.volume = decodeArray[12]
                    m1RPlayer6.volume = decodeArray[13]
                    m1LPlayer7.volume = decodeArray[14]
                    m1RPlayer7.volume = decodeArray[15]
                    
                    //print coeffs
                    print("channel 0 L: ", m1LPlayer0.volume)
                    print("channel 0 R: ", m1RPlayer0.volume)
                    print("channel 1 L: ", m1LPlayer1.volume)
                    print("channel 1 R: ", m1RPlayer1.volume)
                    print("channel 2 L: ", m1LPlayer2.volume)
                    print("channel 2 R: ", m1RPlayer2.volume)
                    print("channel 3 L: ", m1LPlayer3.volume)
                    print("channel 3 R: ", m1RPlayer3.volume)
                    print("channel 4 L: ", m1LPlayer4.volume)
                    print("channel 4 R: ", m1RPlayer4.volume)
                    print("channel 5 L: ", m1LPlayer5.volume)
                    print("channel 5 R: ", m1RPlayer5.volume)
                    print("channel 6 L: ", m1LPlayer6.volume)
                    print("channel 6 R: ", m1RPlayer6.volume)
                    print("channel 7 L: ", m1LPlayer7.volume)
                    print("channel 7 R: ", m1RPlayer7.volume)
                }
            })
            print("Device motion started")
        } else {
            print("Device motion unavailable");
        }

    }

}
