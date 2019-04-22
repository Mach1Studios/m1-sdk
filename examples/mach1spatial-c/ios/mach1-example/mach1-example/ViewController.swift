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
    @IBOutlet var soundMap: SoundMap?
    
    @IBAction func VolumeSliderChanged(_ sender: UISlider) {
        if(encoderCurrent != nil) {
            encoderCurrent?.volume = sender.value
        }
    }
    
    @IBAction func PitchSliderChanged(_ sender: UISlider) {
        if(encoderCurrent != nil) {
            encoderCurrent?.pitch = sender.value
        }
    }
    
    @IBAction func SegmentedControlValueChanged(_ sender: Any) {
        if(encoderCurrent != nil) {
            encoderCurrent?.soundIndex = (soundTypeSegmentedControl?.selectedSegmentIndex)!
            encoderCurrent?.setupPlayers()
        }
    }
  
    var m1Decode : Mach1Decode!
    var encoderCurrent: Encoder?
    var deviceYaw : Float = 0.0
    var devicePitch : Float = 0.0
    var deviceRoll : Float = 0.0

    func closureSelectEncoder (encoder: Encoder? ) -> () {
        if(encoder != nil) {
            volumeSliderControl?.value = (encoder?.volume)!
            pitchSliderControl?.value = (encoder?.pitch)!
            soundTypeSegmentedControl?.selectedSegmentIndex = (encoder?.soundIndex)!
        }
        self.encoderCurrent = encoder
    }
    
    @objc func update() {
        m1Decode.beginBuffer()
        let decodeArray: [Float]  = m1Decode.decode(Yaw: Float(deviceYaw), Pitch: Float(devicePitch), Roll: Float(deviceRoll))
        m1Decode.endBuffer()
        
        // print(deviceYaw)
        // print(decodeArray)
        
        soundMap?.update(decodeArray: decodeArray, rotationAngleForDisplay: -deviceYaw * Float.pi/180)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        m1Decode = Mach1Decode()
        //Mach1 Decode Setup
        //Setup the correct angle convention for orientation Euler input angles
        m1Decode.setPlatformType(type: Mach1PlatformiOS)
        //Setup the expected spatial audio mix format for decoding
        m1Decode.setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoSpatial)
        //Setup for the safety filter speed:
        //1.0 = no filter | 0.1 = slow filter
        m1Decode.setFilterSpeed(filterSpeed: 1.0)
       
        // timer for draw update
        Timer.scheduledTimer(timeInterval: 1.0 / 60.0, target: self, selector: (#selector(ViewController.update)), userInfo: nil, repeats: true)
        
        soundMap?.closureSelectEncoder = closureSelectEncoder
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
                
                self?.deviceYaw = _deviceYaw
                self?.devicePitch = _devicePitch
                self?.deviceRoll = _deviceRoll
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
    
}


