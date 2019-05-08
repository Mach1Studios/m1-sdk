//
//  SoundMapView.swift
//  mach1-ios-encodeDecode-example
//
//  Created by User on 16/04/2019.
//  Copyright © 2019 User. All rights reserved.
//

import Foundation
import UIKit
import SceneKit
import QuartzCore

class PitchMeter: UIView {
    
    var meter : Float =  0.0
    
    var viewPitch: UIView = UIView()
    var labelProgress : UIView = UIView()
    var shapeLayerCircle : CAShapeLayer = CAShapeLayer()
    var labelValue: UILabel = UILabel()
    var labelText: UILabel = UILabel()
    
    required init(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)!
        
        setup()
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        setup()
    }
    
    func setup() {
        recreateView()
    }
    
    
    func recreateView() {
        viewPitch.removeFromSuperview()
        
        viewPitch = UIView()
        
        // test border color
        self.layer.masksToBounds = true
        self.backgroundColor = UIColor( red: 0.0, green: 1.0, blue:0.0, alpha: 0.0 )
        //self.layer.borderColor = UIColor( red: 1.0, green: 0.0, blue:0.0, alpha: 1.0 ).cgColor
        //self.layer.borderWidth = 1.0
        
        // pitch
        viewPitch.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewPitch.backgroundColor  = UIColor( red: 0.4, green: 0.4, blue:0.0, alpha: 1.0 )
        
        self.addSubview(viewPitch)
        
        /*
         viewProgress = UIView()
         viewProgress.frame = CGRect(x: 0, y: 0, width: 4, height: 1)
         viewProgress.center = CGPoint(x: viewPitch.frame.size.width / 2, y: viewPitch.frame.size.height / 2)
         viewProgress.backgroundColor = UIColor(red: 1, green: 0.8, blue: 0.4, alpha: 1)
         viewPitch.addSubview(viewProgress)
         
         let labelZero = UILabel()
         labelZero.frame = CGRect(x: 0, y: 0, width: 8, height: 1)
         labelZero.center = CGPoint(x: viewPitch.frame.size.width / 2, y: viewPitch.frame.size.height / 2)
         labelZero.backgroundColor = UIColor(red: 0.77, green: 0.77, blue: 0.77, alpha: 1)
         viewPitch.addSubview(labelZero)
         */
        
        
        
        let labelInfo = UILabel()
        labelInfo.frame = CGRect(x: 0, y: 0, width: 1, height: self.frame.height)
        labelInfo.backgroundColor = UIColor(red: 0.8, green: 0.8, blue: 0.79, alpha: 1)
        labelInfo.center = CGPoint(x: viewPitch.frame.size.width / 2, y: viewPitch.frame.size.height / 2)
        viewPitch.addSubview(labelInfo)
        
        //        labelInfo.text
        
        // circle
        let circlePath = UIBezierPath(arcCenter: CGPoint(x: 0,y: 0), radius: CGFloat(5), startAngle: CGFloat(0), endAngle:CGFloat(Double.pi * 2), clockwise: true)
        
        shapeLayerCircle = CAShapeLayer()
        shapeLayerCircle.path = circlePath.cgPath
        shapeLayerCircle.fillColor = UIColor(red: 1, green: 0.78, blue: 0.13, alpha: 1).cgColor
        shapeLayerCircle.strokeColor = UIColor.clear.cgColor
        viewPitch.layer.addSublayer(shapeLayerCircle)
        
        labelText = UILabel()
        labelText.font = labelValue.font.withSize(10)
        labelText.frame = self.frame //  CGRect(x: 0, y: 0, width: self.frame.width/2, height: self.frame.height)
        labelText.textColor = UIColor(red: 0.8, green: 0.8, blue: 0.79, alpha: 1)
        labelText.center = CGPoint(x: -self.frame.width/4 ,y: 0)
        labelText.textAlignment = .center
        labelText.text = "Pitch"
        viewPitch.addSubview(labelText)

        labelValue  = UILabel()
        labelValue.font = labelValue.font.withSize(10)
        labelValue.frame = self.frame //  CGRect(x: self.frame.width/2, y: 0, width: self.frame.width/2, height: self.frame.height)
        labelValue.textColor = UIColor(red: 0.8, green: 0.8, blue: 0.79, alpha: 1)
        labelValue.center = CGPoint(x: self.frame.width/4, y: 0)
        labelValue.textAlignment = .center
        labelValue.text = "0º"
        viewPitch.addSubview(labelValue)
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        recreateView()
    }
    
    func update(meter : Float) {
        self.meter = meter
        
        self.setNeedsDisplay()
    }
    
    override func draw(_ rect: CGRect) {
        shapeLayerCircle.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.translatedBy(x: 0, y: CGFloat(meter * Float(self.frame.height) / 2)))
        labelValue.text = String(Int(90 * meter)) + "º"
    }
    
}


