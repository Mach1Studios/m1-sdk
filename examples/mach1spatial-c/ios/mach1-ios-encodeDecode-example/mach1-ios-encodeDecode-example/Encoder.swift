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
    
    var volume : Float = 0.0
    var pitch : Float = 0.0
    var soundIndex : Int = 0
    var selected : Bool = false
    
    var players: [AVAudioPlayer] = []
    var m1Encode : Mach1Encode?

    let circleInternalLayer = CAShapeLayer()
    let circleExternalLayer = CAShapeLayer()

    required init(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)!
        
        setupPlayers()
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        setupPlayers()
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
        
        players[0].prepareToPlay()
        players[1].prepareToPlay()
        
        players[0].numberOfLoops = -1
        players[1].numberOfLoops = -1
        
        players[0].play()
        players[1].play()
        
        updateVolumes()
    }

    func updateVolumes() {
        players[0].volume = 1.0
        players[1].volume = 1.0
        
        // m1Encode set position
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
         // test border color
        self.layer.masksToBounds = true
        self.backgroundColor = UIColor.clear // UIColor( red: 1.0, green: 0.0, blue:0.0, alpha: 1.0 )
  
        let circleInternalPath = UIBezierPath()
        circleInternalPath.move(to: CGPoint(x: 6, y: 12))
        circleInternalPath.addCurve(to: CGPoint(x: 12, y: 6), controlPoint1: CGPoint(x: 9.31, y: 12), controlPoint2: CGPoint(x: 12, y: 9.31))
        circleInternalPath.addCurve(to: CGPoint(x: 6, y: 0), controlPoint1: CGPoint(x: 12, y: 2.69), controlPoint2: CGPoint(x: 9.31, y: 0))
        circleInternalPath.addCurve(to: CGPoint(x: 0, y: 6), controlPoint1: CGPoint(x: 2.69, y: 0), controlPoint2: CGPoint(x: 0, y: 2.69))
        circleInternalPath.addCurve(to: CGPoint(x: 6, y: 12), controlPoint1: CGPoint(x: 0, y: 9.31), controlPoint2: CGPoint(x: 2.69, y: 12))
        circleInternalPath.close()
   
        var internalOffset : CGAffineTransform = CGAffineTransform.identity.translatedBy(x: -6, y: -6)
        circleInternalLayer.path = circleInternalPath.cgPath.copy(using: &internalOffset) // circleInternalPath.cgPath
        circleInternalLayer.fillColor = UIColor( red: 0.4, green: 0.4, blue:0.0, alpha: 1.0 ).cgColor

        let viewCircleInternal : UIView = UIView()
        viewCircleInternal.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCircleInternal.layer.addSublayer(circleInternalLayer)
  
        var externalOffset : CGAffineTransform = CGAffineTransform.identity.translatedBy(x: -6, y: -6)
        circleExternalLayer.path = circleInternalPath.cgPath.copy(using: &externalOffset) // circleInternalPath.cgPath
        circleExternalLayer.fillColor = UIColor.clear.cgColor
        circleExternalLayer.strokeColor = UIColor(red: 0.4, green: 0.39, blue: 0.39, alpha: 1).cgColor
        circleExternalLayer.lineWidth = 1.0
       
        let viewCircleExternal : UIView = UIView()
        viewCircleExternal.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCircleExternal.layer.addSublayer(circleExternalLayer)
 
        self.addSubview(viewCircleInternal)
        self.addSubview(viewCircleExternal)
    }
    
    override func draw(_ rect: CGRect) {
        let amp = 1.0
        let scale : CGFloat = CGFloat(2 + 1 * amp)
        
        let color : CGColor = selected ? UIColor(red: 1, green: 0.8, blue: 0.4, alpha: 1).cgColor : UIColor(red: 0.4, green: 0.39, blue: 0.39, alpha: 1).cgColor;
        
        circleInternalLayer.fillColor = color
        circleExternalLayer.strokeColor = color
        circleExternalLayer.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.scaledBy(x: scale, y: scale))
    }
}




