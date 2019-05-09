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
import SceneKit

var motionManager = CMMotionManager()

var soundFiles: [[String]]  = [
    ["Nature_Mono01","Nature_Mono01"],
    ["Nature_Mono02","Nature_Mono02"],
    ["Nature_Mono03","Nature_Mono03"],
    ["SciFi_Mono01","SciFi_Mono01"],
    ["SciFi_Mono02","SciFi_Mono02"],
    ["SciFi_Mono03","SciFi_Mono03"],
]

class ViewController: UIViewController {
    
    @IBOutlet var soundTypeSegmentedControl: UISegmentedControl?
    @IBOutlet var volumeSliderControl: UISlider?
    @IBOutlet var heightSliderControl: UISlider?
    @IBOutlet var soundMap: SoundMap?
    @IBOutlet var yawMeter: YawMeter?
    @IBOutlet var rollMeter: RollMeter?
    @IBOutlet var pitchMeter: PitchMeter?
    @IBOutlet var labelInfo: UILabel?

    @IBAction func VolumeSliderChanged(_ sender: UISlider) {
        if(encoderCurrent != nil) {
            encoderCurrent?.volume = sender.value
            // This slider is for changing the selected
            // m1encode_obj's input gain / volume
        }
    }
    
    @IBAction func HeightSliderChanged(_ sender: UISlider) {
        if(encoderCurrent != nil) {
            encoderCurrent?.height = sender.value
            // This slider is for setting the m1encode_obj's
            // height
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
    var cameraYaw : Float = 0.0
    var cameraPitch : Float = 0.0
    var cameraRoll : Float = 0.0

    func closureSelectEncoder (encoder: Encoder? ) -> () {
        if(encoder != nil) {
            volumeSliderControl?.value = (encoder?.volume)!
            heightSliderControl?.value = (encoder?.height)!
            soundTypeSegmentedControl?.selectedSegmentIndex = (encoder?.soundIndex)!
        }
        self.encoderCurrent = encoder
        
        DispatchQueue.global(qos: .userInitiated).sync {
            
            // Disabling sliders if nothing is selected,
            // enabling them once we selected something.
            
            if (self.encoderCurrent == nil) {
                volumeSliderControl!.isEnabled = false
                heightSliderControl!.isEnabled = false
                soundTypeSegmentedControl!.isEnabled = false
            } else {
                volumeSliderControl!.isEnabled = true
                heightSliderControl!.isEnabled = true
                soundTypeSegmentedControl!.isEnabled = true
            }
        }
    }
    
    @objc func update() {
        m1Decode.beginBuffer()
        let decodeArray: [Float]  = m1Decode.decode(Yaw: Float(cameraYaw), Pitch: Float(cameraPitch), Roll: Float(cameraRoll))
        m1Decode.endBuffer()
        
        soundMap?.update(decodeArray: decodeArray, rotationAngleForDisplay: -cameraPitch * Float.pi/180)
    }
    
    func getEuler(q1 : SCNVector4) -> float3
    {
        var res = float3(0,0,0)
        
        let test = q1.x * q1.y + q1.z * q1.w
        if (test > 0.499) // singularity at north pole
        {
            return float3(
                0,
                Float(2 * atan2(q1.x, q1.w)),
                .pi / 2
                ) * 180 / .pi
        }
        if (test < -0.499) // singularity at south pole
        {
            return float3(
                0,
                Float(-2 * atan2(q1.x, q1.w)),
                -.pi / 2
                ) * 180 / .pi
        }
        
        let sqx = q1.x * q1.x
        let sqy = q1.y * q1.y
        let sqz = q1.z * q1.z
        
        res.x = Float(atan2(2 * q1.x * q1.w - 2 * q1.y * q1.z, 1 - 2 * sqx - 2 * sqz))
        res.y = Float(atan2(2 * q1.y * q1.w - 2 * q1.x * q1.z, 1 - 2 * sqy - 2 * sqz))
        res.z = Float(sin(2.0 * test))
        
        return res * 180 / .pi
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
                let quat = motion?.gaze(atOrientation: UIApplication.shared.statusBarOrientation)
                var angles = self!.getEuler(q1: quat!)
                
                self?.cameraYaw = angles.x
                self?.cameraPitch = angles.y
                self?.cameraRoll = angles.z
                
                DispatchQueue.main.async {
                    self?.yawMeter?.update(meter: -angles.y / 180)
                    self?.rollMeter?.update(meter: -angles.z / 90)
                    self?.pitchMeter?.update(meter: -angles.x / 90)
                    self?.labelInfo?.text = "Yaw: " + String(format: "%.3f", angles.x) + "°" + "\r\n" +
                        "Pitch: " + String(format: "%.3f", angles.y) + "°" + "\r\n" +
                        "Roll: " + String(format: "%.3f", angles.z) + "°"
                }

            })
            print("Device motion started")
        } else {
            print("Device motion unavailable");
        }
        
        heightSliderControl!.isEnabled = false
        volumeSliderControl!.isEnabled = false
        soundTypeSegmentedControl?.isEnabled = false
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
}


