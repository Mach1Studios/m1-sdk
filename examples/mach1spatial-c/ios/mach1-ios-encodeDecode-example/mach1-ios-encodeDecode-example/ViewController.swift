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
import AVFoundation

var motionManager = CMMotionManager()
var stereoPlayer = AVAudioPlayer()
var deviceYaw = 0.0
var devicePitch = 0.0
var deviceRoll = 0.0
//var segmentedControl: UISegmentedControl?

class ViewController: UIViewController {

    @IBOutlet var _segmentedControl: UISegmentedControl?
    @IBOutlet var _volumeSliderControl: UISlider?
    @IBOutlet var _pitchSliderControl: UISlider?

    //var m1Encode : Mach1Encode?
    //var m1Decode : Mach1Decode?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
//        segmentedControl = _segmentedControl
//        segmentedControl?.setTitleTextAttributes([kCTForegroundColorAttributeName:UIColor.white], for:.selected)
        
        if motionManager.isDeviceMotionAvailable == true {
            motionManager.deviceMotionUpdateInterval = 0.01;
            let queue = OperationQueue()
            motionManager.startDeviceMotionUpdates(to: queue, withHandler: { [weak self] (motion, error) -> Void in
                
                // Get the attitudes of the device
                let attitude = motion?.attitude
                //Device orientation management
                var _deviceYaw = attitude!.yaw * 180/Double.pi
                var _devicePitch = attitude!.pitch * 180/Double.pi
                var _deviceRoll = attitude!.roll * 180/Double.pi
                
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

