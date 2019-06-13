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

extension CGFloat {
    func map(from_start: CGFloat, from_end: CGFloat, to_start: CGFloat, to_end: CGFloat) -> CGFloat {
        let result = ((self - from_start) / (from_end - from_start)) * (to_end - to_start) + to_start
        return result
    }
}

extension Float {
    func map(from_start: Float, from_end: Float, to_start: Float, to_end: Float) -> Float {
        let result = ((self - from_start) / (from_end - from_start)) * (to_end - to_start) + to_start
        return result
    }
}

class SoundMap: UIView {
    
    var rotationAngle : Float =  0.0
    
    var viewCameraCone : UIView = UIView()
    var imageViewCameraPoint = UIImageView()
    var imageViewBackground = UIImageView()
    var layerCameraCone = CAShapeLayer()
    var viewsEncoders: [Encoder] = [Encoder]()
    var viewCircle : UIView = UIView()
    
    var selectedEncoder : Int = -1
    var closureSelectEncoder: ((Encoder?) -> ())? = nil
    
    required init(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)!

        setup()
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)

        setup()
    }
    
    func setup() {
        // gestures
        let tapOneGesture = UITapGestureRecognizer()
        tapOneGesture.addTarget(self, action: #selector(self.tapGestureAction(_:)))
        tapOneGesture.numberOfTapsRequired = 1
        self.addGestureRecognizer(tapOneGesture)
        
        let tapTwoGesture = UITapGestureRecognizer()
        tapTwoGesture.addTarget(self, action: #selector(self.tapGestureAction(_:)))
        tapTwoGesture.numberOfTapsRequired = 2
        self.addGestureRecognizer(tapTwoGesture)
        
        let longTapGesture = UILongPressGestureRecognizer()
        longTapGesture.addTarget(self, action: #selector(self.longTapGestureAction(_:)))
        longTapGesture.numberOfTapsRequired = 0
        longTapGesture.minimumPressDuration = 0.5
        self.addGestureRecognizer(longTapGesture)
        
        let panGesture = UIPanGestureRecognizer()
        panGesture.addTarget(self, action: #selector(self.panGestureAction(_:)))
        self.addGestureRecognizer(panGesture)
        self.isUserInteractionEnabled = true
        
        recreateView()
    }
    
    
    func recreateView() {
        
        
        imageViewBackground.removeFromSuperview()
        imageViewCameraPoint.removeFromSuperview()
        viewCameraCone.removeFromSuperview()
        viewCircle.removeFromSuperview()
        
        imageViewBackground = UIImageView()
        imageViewCameraPoint = UIImageView()
        viewCameraCone = UIView()
        viewCircle = UIView()
        
        // test border color
        self.layer.masksToBounds = true
        self.backgroundColor = UIColor( red: 0.0, green: 1.0, blue:0.0, alpha: 0.0 )
        //self.layer.borderColor = UIColor( red: 1.0, green: 0.0, blue:0.0, alpha: 1.0 ).cgColor
        //self.layer.borderWidth = 1.0
        
        // background
        imageViewBackground.frame = self.frame
        imageViewBackground.image = UIImage(named: "background.png")
        imageViewBackground.backgroundColor = UIColor( red: 0.0, green: 0.0, blue:0.0, alpha: 0.0 )
        imageViewBackground.contentMode = .scaleAspectFit
        imageViewBackground.clipsToBounds = true
        imageViewBackground.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        self.addSubview(imageViewBackground)
        
        // background circle
        /*
        let shapeCircle = UIBezierPath()
        shapeCircle.move(to: CGPoint(x: 141.5, y: 282))
        shapeCircle.addCurve(to: CGPoint(x: 282, y: 141.5), controlPoint1: CGPoint(x: 219.1, y: 282), controlPoint2: CGPoint(x: 282, y: 219.1))
        shapeCircle.addCurve(to: CGPoint(x: 141.5, y: 1), controlPoint1: CGPoint(x: 282, y: 63.9), controlPoint2: CGPoint(x: 219.1, y: 1))
        shapeCircle.addCurve(to: CGPoint(x: 1, y: 141.5), controlPoint1: CGPoint(x: 63.9, y: 1), controlPoint2: CGPoint(x: 1, y: 63.9))
        shapeCircle.addCurve(to: CGPoint(x: 141.5, y: 282), controlPoint1: CGPoint(x: 1, y: 219.1), controlPoint2: CGPoint(x: 63.9, y: 282))
        shapeCircle.close()
 
        var internalOffset : CGAffineTransform = CGAffineTransform.identity.translatedBy(x: -282/2, y: -282/2)
        let circleLayer = CAShapeLayer()
        circleLayer.path = shapeCircle.cgPath.copy(using: &internalOffset) // circleInternalPath.cgPath
        circleLayer.fillColor = UIColor.clear.cgColor
        circleLayer.strokeColor = UIColor(red: 0.4, green: 0.39, blue: 0.39, alpha: 1).cgColor
        circleLayer.lineWidth = 1.0
        circleLayer.transform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.scaledBy(x: self.frame.size.width/290, y: self.frame.size.height/290))
        
        viewCircle.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCircle.layer.addSublayer(circleLayer)
        self.addSubview(viewCircle)
         */

        // camera cone
        //viewCameraCone.frame = CGRect(x: -60, y: -60, width: 60 * 2, height: 60 * 2)
        viewCameraCone.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        viewCameraCone.backgroundColor  = UIColor( red: 0.4, green: 0.4, blue:0.0, alpha: 1.0 )
        //viewCameraCone.layer.anchorPoint = CGPoint(x: 0.5, y: 0.5);
        
        let shapeCameraCone = UIBezierPath()
        shapeCameraCone.move(to: CGPoint(x: 39, y: 5))
        shapeCameraCone.addLine(to: CGPoint(x: 21, y: 49))
        shapeCameraCone.addLine(to: CGPoint(x: 3, y: 5))
        shapeCameraCone.addLine(to: CGPoint(x: 4.94, y: 4.46))
        shapeCameraCone.addCurve(to: CGPoint(x: 37.06, y: 4.46), controlPoint1: CGPoint(x: 15.45, y: 1.54), controlPoint2: CGPoint(x: 26.55, y: 1.54))
        shapeCameraCone.addLine(to: CGPoint(x: 39, y: 5))
        shapeCameraCone.close()
        
        var offset : CGAffineTransform = CGAffineTransform.identity.translatedBy(x: -42/2, y: -49).translatedBy(x: viewCameraCone.frame.size.width / 2, y: viewCameraCone.frame.size.height / 2)
        layerCameraCone.path = shapeCameraCone.cgPath.copy(using: &offset)  //shapeCameraCone.cgPath
        layerCameraCone.lineWidth = 3
        layerCameraCone.strokeColor = UIColor(red: 0.59, green: 0.59, blue: 0.59, alpha: 1).cgColor
        layerCameraCone.fillColor = UIColor.clear.cgColor
        
        viewCameraCone.layer.addSublayer(layerCameraCone)
        self.addSubview(viewCameraCone)
        
        
        imageViewCameraPoint.frame = CGRect(x: -10, y: -10, width: 10 * 2, height: 10 * 2)
        imageViewCameraPoint.image = UIImage(named: "cameraPoint.png")
        imageViewCameraPoint.backgroundColor = UIColor( red: 0.0, green: 0.0, blue:0.0, alpha: 0.0 )
        imageViewCameraPoint.contentMode = .scaleAspectFit
        imageViewCameraPoint.clipsToBounds = true
        imageViewCameraPoint.center = CGPoint(x: self.frame.size.width / 2, y: self.frame.size.height / 2)
        self.addSubview(imageViewCameraPoint)

    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        recreateView()
    }
    
    func distance(_ a: CGPoint, _ b: CGPoint) -> CGFloat {
        let xDist = a.x - b.x
        let yDist = a.y - b.y
        return CGFloat(sqrt(xDist * xDist + yDist * yDist))
    }
    
    func selectEncoder(touchPoint: CGPoint) {
        selectedEncoder = -1
        if(viewsEncoders.count>0) {
            for i in 0...viewsEncoders.count-1 {
                if(distance(viewsEncoders[i].center,touchPoint) < 50 && selectedEncoder == -1) {
                    selectedEncoder = i
                    viewsEncoders[selectedEncoder].selected = true
                }
                else {
                    viewsEncoders[i].selected = false
                }
            }
        }
        
        if(selectedEncoder == -1) {
            closureSelectEncoder!(nil)
        }
        else {
            closureSelectEncoder!(viewsEncoders[selectedEncoder])
        }
    }
    
    
    @objc func longTapGestureAction(_ sender: UILongPressGestureRecognizer) {
        let touchPoint : CGPoint = sender.location(ofTouch: 0, in : self)
        
        if(sender.state ==  UIGestureRecognizerState.began) {
            selectEncoder(touchPoint: touchPoint)
            if(selectedEncoder != -1) {
                viewsEncoders[selectedEncoder].removeFromSuperview()
                viewsEncoders.remove(at: selectedEncoder)
                closureSelectEncoder!(nil)
                selectedEncoder = -1
            }
        }
    }
    
    @objc func tapGestureAction(_ sender: UITapGestureRecognizer) {
        var touchPoint : CGPoint = sender.location(ofTouch: 0, in : self)
        
        if(viewsEncoders.count>0) {
            for i in 0...viewsEncoders.count-1 {
                viewsEncoders[i].selected = false
            }
        }
        
        if(sender.numberOfTapsRequired == 1) {
            selectEncoder(touchPoint: touchPoint)
            
        }
        else  if(sender.numberOfTapsRequired == 2) {
            let encoderView : Encoder = Encoder()
            encoderView.frame.size.width = self.frame.size.width
            encoderView.frame.size.height = self.frame.size.height
            encoderView.center = CGPoint(x: touchPoint.x, y: touchPoint.y)
            encoderView.selected = true
            self.addSubview(encoderView)
            
            viewsEncoders.append(encoderView)
            
            closureSelectEncoder!(encoderView)
        }
        
        touchPoint.x /= self.frame.size.width
        touchPoint.y /= self.frame.size.height
        print("tap: ", touchPoint)
        
        // let someFrame = CGRect(x: 0, y: 0, width: self.frame.width, height: self.frame.height)
        // let isPointInFrame = someFrame.contains(touchPoint)
    }
    
    @objc func panGestureAction(_ sender: UIPanGestureRecognizer) {
        if (sender.state == UIGestureRecognizerState.began)
        {
            var touchPoint : CGPoint = sender.location(ofTouch: 0, in : self)
            print("pan start: ",  touchPoint)
            
            selectEncoder(touchPoint: touchPoint)
            
            touchPoint.x /= self.frame.size.width
            touchPoint.y /= self.frame.size.height
            // self.setNeedsDisplay()
        }
        else if (sender.state == UIGestureRecognizerState.changed)
        {
            var touchPoint : CGPoint = sender.location(ofTouch: 0, in : self)
            
            if(selectedEncoder != -1) {
  
                var vec : SCNVector3 = SCNVector3(2.0 * (Float(touchPoint.x  / (self.frame.width)) - 0.5), 2.0 * (Float(touchPoint.y / (self.frame.height)) - 0.5), 0.0)

                // limit encoder position
                /*
                let maxDist : Float = 0.95
                let dist : Float = vec.length()
                if(dist > maxDist) {
                    vec = vec.normalize() * maxDist
                }
                */
                
                let maxDist : Float = 1.0
                vec.x = min(max(vec.x, -1), 1)
                vec.y = min(max(vec.y, -1), 1)

                viewsEncoders[selectedEncoder].xInternal = vec.x * 1.0 / maxDist
                viewsEncoders[selectedEncoder].yInternal = vec.y * 1.0 / maxDist
                viewsEncoders[selectedEncoder].center = CGPoint(x: CGFloat(self.frame.width * 0.5) * CGFloat(1.0 + vec.x), y: CGFloat(self.frame.height * 0.5) * CGFloat(1.0 + vec.y))
                
                print("point changed: ",  viewsEncoders[selectedEncoder].xInternal,  viewsEncoders[selectedEncoder].yInternal )
            }
            
            touchPoint.x /= self.frame.size.width
            touchPoint.y /= self.frame.size.height
            
        }
    }
    
    func update(decodeArray: [Float], decodeType: Mach1DecodeAlgoType, rotationAngleForDisplay : Float) {
        self.rotationAngle = rotationAngleForDisplay
        
        if(viewsEncoders.count>0) {
            for i in 0...viewsEncoders.count-1 {
                viewsEncoders[i].update(decodeArray: decodeArray, decodeType: decodeType)
            }
        }
        
        self.setNeedsDisplay()
    }
    
    override func draw(_ rect: CGRect) {
        viewCameraCone.layer.sublayerTransform = CATransform3DMakeAffineTransform(CGAffineTransform.identity.rotated(by: CGFloat(rotationAngle)))
    }
    
}
