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

class Encoder: UIView {
    
    var volume : Float = 1.0
    var height : Float = 0.0
    var soundIndex : Int = 0
    var selected : Bool = false
    
    var players: [AVAudioPlayer] = []
    var m1Encode : Mach1Encode!
    
    var xInternal : Float = 0.0
    var yInternal : Float = 0.0
    

    let circleInternalLayer = CAShapeLayer()
    let circleExternalLayer = CAShapeLayer()
    
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
        if(players.count != 0) {
            players[0].stop()
            players[1].stop()
        }
        
        players = [AVAudioPlayer(), AVAudioPlayer()]
        try! players[0] = AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: soundFiles[soundIndex][0], ofType: "wav")!))
        try! players[1] = AVAudioPlayer(contentsOf: URL.init(fileURLWithPath: Bundle.main.path(forResource: soundFiles[soundIndex][1], ofType: "wav")!))
        
        players[0].pan = -1.0
        players[1].pan = 1.0
        
        players[0].volume = 0.0
        players[1].volume = 0.0
        
        players[0].prepareToPlay()
        players[1].prepareToPlay()
        
        players[0].numberOfLoops = -1
        players[1].numberOfLoops = -1
        
        players[0].isMeteringEnabled = true
        players[1].isMeteringEnabled = true
        
        let startDelayTime = 1.0
        let now = players[0].deviceCurrentTime
        let startTime = now + startDelayTime
        players[0].play(atTime: startTime)
        players[1].play(atTime: startTime)
        print (startTime)

    }
    
    func update(decodeArray: [Float]) {
        
        var volumes : [Float] = [ 0, 0 ]
        
        let rotation : Float = fmodf(atan2(-xInternal,yInternal) / (2 * Float.pi) + 0.5, 1.0) // 0 - 1
        let diverge : Float = min( sqrt(powf(xInternal,2) + powf(yInternal,2)), 1.0) *  0.707 // 0 - 0.707
        
        m1Encode.setRotation(rotation: rotation)
        m1Encode.setDiverge(diverge: diverge)
        m1Encode.setPitch(pitch: height)
        m1Encode.setAutoOrbit(setAutoOrbit: true)
        m1Encode.setIsotropicEncode(setIsotropicEncode: true)
        m1Encode.setInputMode(inputMode: Mach1EncodeInputModeMono)
        
        m1Encode.generatePointResults()
        
        let gains = m1Encode.getGains()
        
        //Use each coeff to decode multichannel Mach1 Spatial mix
        for i in 0...7 {
            volumes[0] += decodeArray[i * 2] * gains[0][i]
            volumes[1] += decodeArray[i * 2 + 1] * gains[0][i]
        }
        
        players[0].volume = volumes[0] * volume
        players[1].volume = volumes[1] * volume
        
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
      
        
        self.addSubview(viewCircleExternal)
        self.addSubview(viewCircleInternal)
    }
    
    func dBToAmplitude(dB : Float) -> Float
    {
        return pow(10.0, dB / 20.0)
    }
    
    override func draw(_ rect: CGRect) {
        let amp = 10000 * dBToAmplitude(dB: players[0].averagePower(forChannel: 0) + players[1].averagePower(forChannel: 1))/2
        let scale : CGFloat = CGFloat(2 + 1 * amp)
        
        players[0].updateMeters()
        players[1].updateMeters()
        
        let selectedColor : CGColor = UIColor(red: 1, green: 0.8, blue: 0.4, alpha: 1).cgColor

        circleInternalLayer.fillColor = selected ? selectedColor : UIColor(red: 90.0/255, green: 90.0/255, blue: 90.0/255, alpha: 1).cgColor

        circleExternalLayer.fillColor = UIColor( red: 114.0/255, green: 114.0/255, blue:114.0/255, alpha: selected ? 0.0 : 1.0 ).cgColor
        circleExternalLayer.strokeColor = selected ? selectedColor : UIColor(red: 151.0/255, green: 151.0/255, blue: 151.0/255, alpha: 1).cgColor
        circleExternalLayer.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.scaledBy(x: scale, y: scale))
    }
}





