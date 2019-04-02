//
//  ViewController.swift
//  Example
//
//  Created by User on 02/04/2019.
//  Copyright © 2019 User. All rights reserved.
//

import UIKit
import Mach1SpatialAPI

var m1obj = Mach1DecodePositional()
var cameraPosition: Mach1Point3D = Mach1Point3D(x: 0, y: 0, z: 0.5)
var objectPosition: Mach1Point3D = Mach1Point3D(x: 0, y: 0, z: 0)

var cameraPitch : Float = 0
var cameraYaw : Float = 0
var cameraRoll : Float = 0

var isYawActive = true
var isPitchActive = false
var isRollActive = false

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        
        
        //Mach1 Decode Setup
        //Setup the correct angle convention for orientation Euler input angles
        m1obj.setPlatformType(type: Mach1PlatformiOS)
        //Setup the expected spatial audio mix format for decoding
        m1obj.setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoSpatial)
        //Setup for the safety filter speed:
        //1.0 = no filter | 0.1 = slow filter
        m1obj.setFilterSpeed(filterSpeed: 1.0)
        
        //Mach1 Decode Positional Setup
        //Advanced Setting: used for blending 2 m1obj for crafting room ambiences
        m1obj.setUseBlendMode(useBlendMode: false)
        //Advanced Setting: ignore movements on height plane
        m1obj.setIgnoreTopBottom(ignoreTopBottom: false)
        //Setting: mute audio when setCameraPosition position is outside of m1obj volume
        //based on setDecoderAlgoPosition & setDecoderAlgoScale
        m1obj.setMuteWhenOutsideObject(muteWhenOutsideObject: false)
        //Setting: mute audio when setCameraPosition position is inside of m1obj volume
        //based on setDecoderAlgoPosition & setDecoderAlgoScale
        m1obj.setMuteWhenInsideObject(muteWhenInsideObject: true)
        //Setting: turn on/off distance attenuation of m1obj
        m1obj.setUseFalloff(useFalloff: false)
        //Advanced Setting: when on, positional rotation is calculated from the closest point
        //of the m1obj's volume and not rotation from the center of m1obj.
        //use this if you want the positional rotation tracking to be from a plane instead of from a point
        m1obj.setUseClosestPointRotationMuteInside(bool: false)
        
         //Send device orientation to m1obj with the preferred algo
        m1obj.setCameraPosition(point: (cameraPosition))
        m1obj.setCameraRotation(point: Mach1Point3D(x: cameraPitch, y: cameraYaw, z: cameraRoll))
        m1obj.setDecoderAlgoPosition(point: (objectPosition))
        m1obj.setDecoderAlgoRotation(point: Mach1Point3D(x: 0, y: 0, z: 0))
        m1obj.setDecoderAlgoScale(point: Mach1Point3D(x: 0.1, y: 0.1, z: 0.1))
        //Setting: on/off yaw rotations from position
        m1obj.setUseYawForRotation(bool: isYawActive)
        //Setting: on/off pitch rotations from position
        m1obj.setUsePitchForRotation(bool: isPitchActive)
        //Setting: on/off roll rotations frok om position
        m1obj.setUseRollForRotation(bool: isRollActive)
        
        m1obj.evaluatePositionResults()
        
        var decodeArray: [Float] = Array(repeating: 0.0, count: 18)
        m1obj.getVolumesWalls(result: &decodeArray)
        print(decodeArray)


        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

