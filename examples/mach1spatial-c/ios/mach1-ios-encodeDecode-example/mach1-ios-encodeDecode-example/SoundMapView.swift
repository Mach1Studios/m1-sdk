//
//  SoundMapView.swift
//  mach1-ios-encodeDecode-example
//
//  Created by User on 16/04/2019.
//  Copyright © 2019 User. All rights reserved.
//

import Foundation
import UIKit

class SoundMapView: UIView {
    
    var degrees : Double = 0.0
    let viewCameraCone : UIView = UIView()
    let layerCameraCone = CAShapeLayer()
    let tap = UITapGestureRecognizer()

    required init(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)!
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
    }
    
    @objc func AddEncoder(_ sender: UITapGestureRecognizer) {
        var touchPoint : CGPoint = sender.location(ofTouch: 0, in : self)
        touchPoint.x /= viewCameraCone.frame.size.width
        touchPoint.y /= viewCameraCone.frame.size.height
        print( touchPoint)
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        // gestures
        tap.addTarget(self, action: #selector(self.AddEncoder(_:)))
        self.addGestureRecognizer(tap)
        self.isUserInteractionEnabled = true

        // timer for draw update
        Timer.scheduledTimer(withTimeInterval: 1.0 / 60.0, repeats: true, block: { (timer) in self.setNeedsDisplay() })
        
        // test border color
        self.layer.masksToBounds = true
        self.layer.borderColor = UIColor( red: 1.0, green: 0.0, blue:0.0, alpha: 1.0 ).cgColor
        self.layer.borderWidth = 1.0
        
        // camera cone
        viewCameraCone.frame = CGRect(x: -42, y: 52, width: 42 * 2, height: 52 * 2)
        viewCameraCone.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCameraCone.backgroundColor  = UIColor( red: 0.4, green: 0.4, blue:0.0, alpha: 1.0 )
        viewCameraCone.layer.anchorPoint = CGPoint(x: 0.5, y: 0.5);
        
        let shapeCameraCone = UIBezierPath()
        shapeCameraCone.move(to: CGPoint(x: 39, y: 5))
        shapeCameraCone.addLine(to: CGPoint(x: 21, y: 49))
        shapeCameraCone.addLine(to: CGPoint(x: 3, y: 5))
        shapeCameraCone.addLine(to: CGPoint(x: 4.94, y: 4.46))
        shapeCameraCone.addCurve(to: CGPoint(x: 37.06, y: 4.46), controlPoint1: CGPoint(x: 15.45, y: 1.54), controlPoint2: CGPoint(x: 26.55, y: 1.54))
        shapeCameraCone.addLine(to: CGPoint(x: 39, y: 5))
        shapeCameraCone.close()

        layerCameraCone.path = shapeCameraCone.cgPath
        layerCameraCone.lineWidth = 3
        layerCameraCone.strokeColor = UIColor(red: 0, green: 0.8, blue: 0.4, alpha: 1).cgColor
        layerCameraCone.fillColor = UIColor.clear.cgColor
        
        viewCameraCone.layer.addSublayer(layerCameraCone)
        
        self.addSubview(viewCameraCone)
    }
    
    override func draw(_ rect: CGRect) {
        degrees += 1.0 * .pi/180
        
        layerCameraCone.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.translatedBy(x: viewCameraCone.frame.size.width/2, y: viewCameraCone.frame.size.height/2).rotated(by: CGFloat(degrees)).translatedBy(x: -39/2, y: -49 - 3))
        
    }
    
}



