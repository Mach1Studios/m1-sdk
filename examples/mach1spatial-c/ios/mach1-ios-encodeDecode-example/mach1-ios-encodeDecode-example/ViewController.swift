//
//  ViewController.swift
//  mach1-ios-encodeDecode-example
//
//  Created by User on 16/04/2019.
//  Copyright © 2019 User. All rights reserved.
//

// add library search path
// add bridging header


import UIKit
import CoreMotion

var motionManager = CMMotionManager()
var deviceYaw : Float = 0.0
var devicePitch : Float = 0.0
var deviceRoll : Float = 0.0

var soundFiles: [[String]]  = [
    ["Nature_Stereo1","Nature_Stereo1"],
    ["Nature_Stereo2","Nature_Stereo2"],
    ["Nature_Stereo3","Nature_Stereo3"],
    ["SciFi_Stereo3","SciFi_Stereo3"],
    ["SciFi_Stereo12","SciFi_Stereo12"],
    ["SciFi_Stereo12","SciFi_Stereo12"],
]

class ViewController: UIViewController {
    
    @IBOutlet var soundTypeSegmentedControl: UISegmentedControl?
    @IBOutlet var volumeSliderControl: UISlider?
    @IBOutlet var pitchSliderControl: UISlider?
    @IBOutlet var soundMapViewControl: SoundMapView?
    
    @IBAction func VolumeSliderChanged(_ sender: UISlider) {
        if(soundMapViewControl?.selectedEncoder != -1) {
            encoderCurrent?.volume = sender.value
        }
    }
    
    @IBAction func PitchSliderChanged(_ sender: UISlider) {
        if(soundMapViewControl?.selectedEncoder != -1) {
            encoderCurrent?.pitch = sender.value
        }
    }
    
    @IBAction func SegmentedControlValueChanged(_ sender: Any) {
        if(soundMapViewControl?.selectedEncoder != -1) {
            encoderCurrent?.soundIndex = (soundTypeSegmentedControl?.selectedSegmentIndex)!
            encoderCurrent?.setupPlayers()
        }
    }
    

    var m1Decode : Mach1Decode?
    var encoderCurrent: Encoder?
    
    func closureSelectEncoder (encoder: Encoder ) -> () {
        volumeSliderControl?.value = encoder.volume
        pitchSliderControl?.value = encoder.pitch
        soundTypeSegmentedControl?.selectedSegmentIndex = encoder.soundIndex
        self.encoderCurrent = encoder
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        soundMapViewControl?.closureSelectEncoder = closureSelectEncoder
        soundTypeSegmentedControl?.setTitleTextAttributes([kCTForegroundColorAttributeName:UIColor.white], for:.selected)
        
        if motionManager.isDeviceMotionAvailable == true {
            motionManager.deviceMotionUpdateInterval = 0.01;
            let queue = OperationQueue()
            motionManager.startDeviceMotionUpdates(to: queue, withHandler: { [weak self] (motion, error) -> Void in
                
                // Get the attitudes of the device
                let attitude = motion?.attitude
                //Device orientation management
                let _deviceYaw:Float = Float(attitude!.yaw) * 180/Float.pi
                let _devicePitch:Float = Float(attitude!.pitch) * 180/Float.pi
                let _deviceRoll:Float = Float(attitude!.roll) * 180/Float.pi
                
                // Please notice that you're expected to correct the correct the angles you get from
                // the device's sensors to provide M1 Library with accurate angles in accordance to documentation.
                // (documentation URL here)
                
                
                deviceYaw = _deviceYaw
                devicePitch = _devicePitch
                deviceRoll = _deviceRoll
                
                //Send device orientation to m1obj with the preferred algo
                /*
                 m1obj.beginBuffer()
                 let decodeArray: [Float]  = m1obj.decode(Yaw: Float(deviceYaw), Pitch: Float(devicePitch), Roll: Float(deviceRoll))
                 m1obj.endBuffer()
                 
                 //Use each coeff to decode multichannel Mach1 Spatial mix
                 for i in 0...7 {
                 players[i * 2].setVolume(Float(decodeArray[i * 2]), fadeDuration: 0)
                 players[i * 2 + 1].setVolume(Float(decodeArray[i * 2 + 1]), fadeDuration: 0)
                 
                 print(String(players[i * 2].currentTime) + " ; " + String(i * 2))
                 print(String(players[i * 2 + 1].currentTime) + " ; " + String(i * 2 + 1))
                 }
                 */
            })
            print("Device motion started")
        } else {
            print("Device motion unavailable");
        }
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func UpdateEncodeDecode() {
        
    }
    
    
}


