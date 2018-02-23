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

class ViewController: UIViewController {
    
    //MARK: test
    @IBOutlet weak var yaw: UILabel!
    @IBOutlet weak var pitch: UILabel!
    @IBOutlet weak var roll: UILabel!
    
        override func viewDidLoad() {
            super.viewDidLoad()
            // Do any additional setup after loading the view, typically from a nib.
            
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
                        print("Yaw: ", deviceYaw)
                        print("Pitch: ", devicePitch)
                        DispatchQueue.main.async() {
                            self?.yaw.text = String(deviceYaw)
                            self?.pitch.text = String(devicePitch)
                            self?.roll.text = String(deviceRoll)
                        }
                    }
                })
                print("Device motion started")
            } else {
                print("Device motion unavailable");
            }
    }

}
