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

class EncoderView: UIView {
    
    var volume : Double = 0.0
    var pitch : Double = 0.0
    var selected : Bool = false

    //var players: [AVAudioPlayer] = []

    let circleInternalLayer = CAShapeLayer()
    let circleExternalLayer = CAShapeLayer()

    required init(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)!
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
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
        let scale : CGFloat = CGFloat(2 + 1 * volume)
        
        let color : CGColor = selected ? UIColor(red: 1, green: 0.8, blue: 0.4, alpha: 1).cgColor : UIColor(red: 0.4, green: 0.39, blue: 0.39, alpha: 1).cgColor;
        
        circleInternalLayer.fillColor = color
        circleExternalLayer.strokeColor = color
        circleExternalLayer.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.scaledBy(x: scale, y: scale))
    }
    
}




