//
//  EncoderView
//  mach1-ios-encodeDecode-example
//
//  Created by User on 16/04/2019.
//  Copyright © 2019 User. All rights reserved.
//

import Foundation
import UIKit
import AVFoundation
import SceneKit

class Encoder: UIView {
    
    var volume : Float = 1.0
    var height : Float = 0.0
    var soundIndex : Int = 0
    var selected : Bool = false
    
    var players: [AVAudioPlayer] = []
    var m1Encode : Mach1Encode!
    
    var xInternal : Float = 0.0
    var yInternal : Float = 0.0
    var stereoSpread : Float = 0.0
    var type : Mach1EncodeInputModeType = Mach1EncodeInputModeMono
 
    let circleInternalLayer = CAShapeLayer()
    let circleExternalLayer = CAShapeLayer()

    let circleLeftLayer = CAShapeLayer()
    let circleRightLayer = CAShapeLayer()

    required init(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)!
        
        setup()
        setupPlayers()
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        setup()
        setupPlayers()
    }
    
    func setup() {
        m1Encode = Mach1Encode()
    }
    
    func setupPlayers() {
      
        for i in 0..<players.count {
            players[i].stop()
        }

        if(soundFiles[soundIndex].count == 1) {
            type = Mach1EncodeInputModeMono
            players = [AVAudioPlayer(), AVAudioPlayer()]
            try! players[0] = AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: soundFiles[soundIndex][0], ofType: "wav")!))
            try! players[1] = AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: soundFiles[soundIndex][0], ofType: "wav")!))
        }
        else if(soundFiles[soundIndex].count == 2) {
            type = Mach1EncodeInputModeStereo
            players = [AVAudioPlayer(), AVAudioPlayer(), AVAudioPlayer(), AVAudioPlayer()]
            try! players[0] = AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: soundFiles[soundIndex][0], ofType: "wav")!))
            try! players[1] = AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: soundFiles[soundIndex][0], ofType: "wav")!))
            try! players[2] = AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: soundFiles[soundIndex][1], ofType: "wav")!))
            try! players[3] = AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: soundFiles[soundIndex][1], ofType: "wav")!))
        }
        
        for i in stride(from: 0, to: players.count, by: 2) {
            players[i + 0].pan = -1.0
            players[i + 1].pan = 1.0
            
            players[i + 0].volume = 0.0
            players[i + 1].volume = 0.0
            
            players[i + 0].prepareToPlay()
            players[i + 1].prepareToPlay()
            
            players[i + 0].numberOfLoops = -1
            players[i + 1].numberOfLoops = -1
            
            players[i + 0].isMeteringEnabled = true
            players[i + 1].isMeteringEnabled = true
        }
        
        let startDelayTime = 1.0
        let now = players[0].deviceCurrentTime
        let startTime = now + startDelayTime
        
        for i in 0..<players.count {
            players[i].play(atTime: startTime)
        }

        circleLeftLayer	.isHidden = (type == Mach1EncodeInputModeMono)
        circleRightLayer.isHidden = (type == Mach1EncodeInputModeMono)
        
        print (startTime)
   }
    
    func update(decodeArray: [Float], decodeType: Mach1DecodeAlgoType) {
        let rotation : Float = fmodf(atan2(-xInternal,yInternal) / (2 * Float.pi) + 0.5, 1.0) // 0 - 1
        let diverge : Float = sqrt(powf(xInternal,2) + powf(yInternal,2)) / sqrt(2) // diagonal
        
        m1Encode.setRotation(rotation: rotation)
        m1Encode.setDiverge(diverge: diverge)
        m1Encode.setPitch(pitch: height)
        m1Encode.setAutoOrbit(setAutoOrbit: true)
        m1Encode.setIsotropicEncode(setIsotropicEncode: true)
        m1Encode.setInputMode(inputMode: type)
        m1Encode.setStereoSpread(setStereoSpread: stereoSpread)
        
        m1Encode.generatePointResults()
        
        //Use each coeff to decode multichannel Mach1 Spatial mix
        var volumes : [Float] = m1Encode.getResultingVolumesDecoded(decodeType: decodeType, decodeResult: decodeArray)

        for i in 0..<players.count {
            players[i].volume = volumes[i] * volume
        }
        
        self.setNeedsDisplay()
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        // test border color
        self.layer.masksToBounds = true
        self.backgroundColor = UIColor.clear // UIColor( red: 1.0, green: 0.0, blue:0.0, alpha: 1.0 )
        
        // circle internal
        let circlePath = UIBezierPath()
        circlePath.move(to: CGPoint(x: 6, y: 12))
        circlePath.addCurve(to: CGPoint(x: 12, y: 6), controlPoint1: CGPoint(x: 9.31, y: 12), controlPoint2: CGPoint(x: 12, y: 9.31))
        circlePath.addCurve(to: CGPoint(x: 6, y: 0), controlPoint1: CGPoint(x: 12, y: 2.69), controlPoint2: CGPoint(x: 9.31, y: 0))
        circlePath.addCurve(to: CGPoint(x: 0, y: 6), controlPoint1: CGPoint(x: 2.69, y: 0), controlPoint2: CGPoint(x: 0, y: 2.69))
        circlePath.addCurve(to: CGPoint(x: 6, y: 12), controlPoint1: CGPoint(x: 0, y: 9.31), controlPoint2: CGPoint(x: 2.69, y: 12))
        circlePath.close()
        
        // circle external
        var externalOffset : CGAffineTransform = CGAffineTransform.identity.translatedBy(x: -6, y: -6)
        circleExternalLayer.path = circlePath.cgPath.copy(using: &externalOffset) // circleInternalPath.cgPath
        circleExternalLayer.lineWidth = 0.5
        
        let viewCircleExternal : UIView = UIView()
        viewCircleExternal.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCircleExternal.layer.addSublayer(circleExternalLayer)
        
        // circle internal
        var internalOffset : CGAffineTransform = CGAffineTransform.identity.translatedBy(x: -6, y: -6)
        circleInternalLayer.path = circlePath.cgPath.copy(using: &internalOffset) // circleInternalPath.cgPath
        
        let viewCircleInternal : UIView = UIView()
        viewCircleInternal.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCircleInternal.layer.addSublayer(circleInternalLayer)
        
        // circle left
        var leftOffset : CGAffineTransform = CGAffineTransform.identity.translatedBy(x: -6, y: -6)//.translatedBy(x: -24, y: 0)
        circleLeftLayer.path = circlePath.cgPath.copy(using: &leftOffset) // circleInternalPath.cgPath
        circleLeftLayer.fillColor = UIColor( red: 114.0/255, green: 114.0/255, blue:114.0/255, alpha: 1.0 ).cgColor

        let viewCircleLeft : UIView = UIView()
        viewCircleLeft.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCircleLeft.layer.addSublayer(circleLeftLayer)
        
        // circle left
        var rightOffset : CGAffineTransform = CGAffineTransform.identity.translatedBy(x: -6, y: -6)//.translatedBy(x: 24, y: 0)
        circleRightLayer.path = circlePath.cgPath.copy(using: &rightOffset) // circleInternalPath.cgPath
        circleRightLayer.fillColor = UIColor( red: 114.0/255, green: 114.0/255, blue:114.0/255, alpha: 1.0 ).cgColor
        
        let viewCircleRight : UIView = UIView()
        viewCircleRight.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCircleRight.layer.addSublayer(circleRightLayer)
        
        self.addSubview(viewCircleLeft)
        self.addSubview(viewCircleRight)
        self.addSubview(viewCircleExternal)
        self.addSubview(viewCircleInternal)
    }
    
    func dBToAmplitude(dB : Float) -> Float
    {
        return pow(10.0, dB / 20.0)
    }
    
    func deg2rad(_ number: Double) -> Double {
        return number * .pi / 180
    }
    
    override func draw(_ rect: CGRect) {
        var amp : Float = 0.0
        if(players.count > 0) {
            for i in stride(from: 0, to: players.count, by: 2) {
                amp = amp + players[i + 0].averagePower(forChannel: 0)
                amp = amp + players[i + 1].averagePower(forChannel: 1)

                players[i + 0].updateMeters()
                players[i + 1].updateMeters()
            }
            amp = 10000 * dBToAmplitude(dB: amp) / Float(players.count)
        }
        
        let scale : CGFloat = CGFloat(2 + 1 * amp)
        
        let selectedColor : CGColor = UIColor(red: 1, green: 0.8, blue: 0.4, alpha: 1).cgColor
        circleInternalLayer.fillColor = selected ? selectedColor : UIColor(red: 90.0/255, green: 90.0/255, blue: 90.0/255, alpha: 1).cgColor
        circleExternalLayer.fillColor = UIColor( red: 114.0/255, green: 114.0/255, blue:114.0/255, alpha: selected ? 0.0 : 1.0 ).cgColor
        circleExternalLayer.strokeColor = selected ? selectedColor : UIColor(red: 151.0/255, green: 151.0/255, blue: 151.0/255, alpha: 1).cgColor
        circleExternalLayer.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.scaledBy(x: scale, y: scale))
        
        if(m1Encode.getPointsCount() == 2) {
            var points : [SCNVector3] = m1Encode.getPoints()
            
            // Further Debug functions
            print("points names: ", m1Encode.getPointsNames())
            print("getGains: ", m1Encode.getGains())
            print("getPointsCount: ", m1Encode.getPointsCount())
            print("getGainsForInputChannelNamed: ", m1Encode.getGainsForInputChannelNamed(pointName: "R"))
            
            if(circleLeftLayer.isHidden == false) {
                circleLeftLayer.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.translatedBy(x: -self.center.x +  CGFloat((points[0].z) * Float((self.superview?.frame.width)!)), y: -self.center.y + CGFloat((1-points[0].x) * Float((self.superview?.frame.height)!))))
            }
            if(circleRightLayer.isHidden == false) {
                circleRightLayer.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.translatedBy(x: -self.center.x +  CGFloat((points[1].z) * Float((self.superview?.frame.width)!)), y: -self.center.y + CGFloat((1-points[1].x) * Float((self.superview?.frame.height)!))))
            }
        }

    }
}





