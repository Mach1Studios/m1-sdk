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

class YawMeter: UIView {
    
    var meter : Float =  0.0
    
    var viewYaw: UIView = UIView()
    var labelProgress : UIView = UIView()
    var shapeLayerLine : CAShapeLayer = CAShapeLayer()
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
        viewYaw.removeFromSuperview()
        
        viewYaw = UIView()
        
        // border color
        self.layer.masksToBounds = true
        self.backgroundColor = UIColor( red: 0.0, green: 1.0, blue:0.0, alpha: 0.0 )
        
        // pitch
        viewYaw.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewYaw.backgroundColor  = UIColor( red: 0.4, green: 0.4, blue:0.0, alpha: 1.0 )
        
        self.addSubview(viewYaw)
        
        // background
        let imageViewBackground = UIImageView()
        imageViewBackground.frame = CGRect(x: 7, y: 7, width: self.frame.width - 14, height: self.frame.height - 14)
        imageViewBackground.image = UIImage(named: "yawMeter.png")
        imageViewBackground.backgroundColor = UIColor( red: 0.0, green: 0.0, blue:0.0, alpha: 0.0 )
        imageViewBackground.contentMode = .scaleAspectFit
        imageViewBackground.clipsToBounds = true
        imageViewBackground.center = CGPoint(x: 0, y: 0)
        viewYaw.addSubview(imageViewBackground)
        
        // line
        let angle : Float = 3.8
        let dirX : CGFloat = CGFloat(sin(angle))
        let dirY : CGFloat = CGFloat(-cos(angle))
        
        let linePath = UIBezierPath()
        linePath.move(to: CGPoint(x: dirX * 15, y: dirY * 15))
        linePath.addLine(to: CGPoint(x: dirX * (self.frame.width / 2 - 24), y: dirY * (self.frame.height / 2 - 24)))
        linePath.close()
        
        shapeLayerLine = CAShapeLayer()
        shapeLayerLine.path = linePath.cgPath
        shapeLayerLine.fillColor = UIColor(red: 1, green: 0.78, blue: 0.13, alpha: 1).cgColor
        shapeLayerLine.strokeColor = UIColor(red: 1, green: 0.78, blue: 0.13, alpha: 1).cgColor
        shapeLayerLine.lineWidth = 4
        viewYaw.layer.addSublayer(shapeLayerLine)
        
        // circle
        let circlePath = UIBezierPath(arcCenter: CGPoint(x: 0,y: 0), radius: CGFloat(self.frame.width/4 - 4), startAngle: CGFloat(-Float.pi/2), endAngle:CGFloat(-Float.pi/2 + angle), clockwise: true)
        
        shapeLayerCircle = CAShapeLayer()
        shapeLayerCircle.path = circlePath.cgPath
        shapeLayerCircle.fillColor = UIColor.clear.cgColor
        shapeLayerCircle.strokeColor = UIColor(red: 1, green: 0.78, blue: 0.13, alpha: 1).cgColor
        shapeLayerCircle.lineWidth = 4
        viewYaw.layer.addSublayer(shapeLayerCircle)
        
        labelText = UILabel()
        labelText.font = labelValue.font.withSize(10)
        labelText.frame = self.frame //  CGRect(x: 0, y: 0, width: self.frame.width/2, height: self.frame.height)
        labelText.textColor = UIColor(red: 0.8, green: 0.8, blue: 0.79, alpha: 1)
        labelText.center = CGPoint(x: 0, y: self.frame.height/4 + 4)
        labelText.textAlignment = .center
        labelText.text = "Yaw"
        viewYaw.addSubview(labelText)

        labelValue  = UILabel()
        labelValue.font = labelValue.font.withSize(10)
        labelValue.frame = self.frame //  CGRect(x: self.frame.width/2, y: 0, width: self.frame.width/2, height: self.frame.height)
        labelValue.textColor = UIColor(red: 0.8, green: 0.8, blue: 0.79, alpha: 1)
        labelValue.center = CGPoint(x: 0, y: 0)
        labelValue.textAlignment = .center
        labelValue.text = "0º"
        viewYaw.addSubview(labelValue)
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
        // line
        let angle : Float = meter * Float.pi
        let dirX : CGFloat = CGFloat(sin(angle))
        let dirY : CGFloat = CGFloat(-cos(angle))
        
        let linePath = UIBezierPath()
        linePath.move(to: CGPoint(x: dirX * (self.frame.width/6), y: dirY * (self.frame.width/6)))
        linePath.addLine(to: CGPoint(x: dirX * (self.frame.width/4), y: dirY * (self.frame.width/4)))
        linePath.close()
        
        shapeLayerLine.path = linePath.cgPath
        
        // circle
        let circlePath = UIBezierPath(arcCenter: CGPoint(x: 0,y: 0), radius: CGFloat(self.frame.width/4 + 10), startAngle: CGFloat(-Float.pi/2), endAngle:CGFloat(-Float.pi/2 + angle), clockwise: (angle > 0))
        
        shapeLayerCircle.path = circlePath.cgPath
        
        labelValue.text = String(Int(180 * meter)) + "º"
    }
    
}


