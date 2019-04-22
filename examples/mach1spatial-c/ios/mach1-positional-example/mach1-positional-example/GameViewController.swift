//
//  GameViewController.swift
//  testSceneKit2
//
//  Created by User on 05/03/2019.
//  Copyright © 2019 User. All rights reserved.
//

import UIKit
import QuartzCore
import SceneKit
import GLKit




class GameViewController: UIViewController, SCNSceneRendererDelegate {
    
    var scene:SCNScene! = nil
    var axisNode: SCNNode! = nil
    var planeNode: SCNNode! = nil
    var cameraConeNode: SCNNode! = nil
    var volumeBoxNode: SCNNode! = nil
    var volumeHelperBoxNode: SCNNode! = nil
    var lineCameraVolumeNode: SCNNode! = nil
    var sphereLeftNodes : [SCNNode ] = []
    var sphereRightNodes : [SCNNode ] = []
    var size : Int = 30
    
    var dragEnable : Bool = false

    var scnView : SCNView! = nil
    
    func deg2rad(_ number: Float) -> Float {
        return number * .pi / 180
    }
    
    func renderer(_ renderer: SCNSceneRenderer, updateAtTime time: TimeInterval)
    {
        let angleSet = m1obj.getCurrentAngle()
        
        var angleVolume:Mach1Point3D = m1obj.getVolumeRotation();
        
        cameraConeNode.eulerAngles =  SCNVector3(x: deg2rad(cameraPitch), y: deg2rad(cameraYaw), z: deg2rad(cameraRoll))
        cameraConeNode.position =  SCNVector3(x: cameraPosition.x * Float(size)/2, y: cameraPosition.y * Float(size)/2, z: -cameraPosition.z * Float(size)/2)

        volumeHelperBoxNode.eulerAngles = SCNVector3(x: deg2rad(angleVolume.x), y: deg2rad(angleVolume.y), z: deg2rad(angleVolume.z))
        volumeHelperBoxNode.orientation = GLKQuaternionMultiply(volumeHelperBoxNode.orientation.toGLK(), GLKQuaternionInvert(cameraConeNode.orientation.toGLK())).toSCN()
        volumeHelperBoxNode.position = cameraConeNode.position

        // recreate line
        lineCameraVolumeNode.removeFromParentNode()
        lineCameraVolumeNode = SCNNode.createLineNode(from: cameraConeNode.position, to: volumeHelperBoxNode.position)
        lineCameraVolumeNode.geometry?.firstMaterial?.diffuse.contents = UIColor(red: 1.0, green: 1.0, blue: 0.0, alpha: 1)
        scene.rootNode.addChildNode(lineCameraVolumeNode)
        
        var decodeArray: [Float] = Array(repeating: 0.0, count: 18)
        m1obj.getVolumesWalls(result: &decodeArray)
        for i in 0...7 {
            let volumeLeft = Float(decodeArray[i * 2])
            let volumeRight = Float(decodeArray[i * 2 + 1])
            sphereLeftNodes[i].scale = SCNVector3(x: volumeLeft,
                                               y: volumeLeft,
                                               z: volumeLeft)
            sphereRightNodes[i].scale = SCNVector3(x: volumeRight,
                                               y: volumeRight,
                                               z: volumeRight)
        }
        
        
    }
    
    @IBAction func goBack(_ sender: Any) {
        dismiss(animated: true, completion: nil)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        scene = SCNScene()
        
        let cameraNode = SCNNode()
        cameraNode.camera = SCNCamera()
        cameraNode.camera?.zNear = 0.01;
        cameraNode.camera?.zFar = 1000;
        cameraNode.position = SCNVector3(x: 0, y: 0, z: 100)
        scene.rootNode.addChildNode(cameraNode)
        
        var points:[SCNVector3] = []
        
        points.append(SCNVector3(-size, -size, -size)/2);
        points.append(SCNVector3(size, -size, -size)/2);
        points.append(SCNVector3(-size, -size, size)/2);
        points.append(SCNVector3(size, -size, size)/2);
            
        points.append(SCNVector3(-size, size, -size)/2);
        points.append(SCNVector3(size, size, -size)/2);
        points.append(SCNVector3(-size, size, size)/2);
        points.append(SCNVector3(size, size, size)/2);
        
        for i in 0...points.count-1 {
            let sphere1Geometry = SCNSphere(radius: 1.5)
            sphere1Geometry.firstMaterial?.diffuse.contents = UIColor.blue
            let sphere1Node = SCNNode(geometry: sphere1Geometry)
            sphere1Node.position = points[i] + SCNVector3(-1, 0, 0)
            scene.rootNode.addChildNode(sphere1Node)
            sphereLeftNodes.append(sphere1Node)
            
            let sphere2Geometry = SCNSphere(radius: 1.5)
            sphere2Geometry.firstMaterial?.diffuse.contents = UIColor.red
            let sphere2Node = SCNNode(geometry: sphere2Geometry)
            sphere2Node.position = points[i] + SCNVector3(1, 0, 0)
            scene.rootNode.addChildNode(sphere2Node)
            sphereRightNodes.append(sphere2Node)
            
            let text = SCNText(string: String(i), extrusionDepth:0)
            text.font = UIFont (name: "Arial", size: 3)
            text.firstMaterial!.diffuse.contents = UIColor.white
            text.firstMaterial!.specular.contents = UIColor.white
            let textNode = SCNNode(geometry: text)
            textNode.position = points[i] + SCNVector3(0, -5, 0)
            scene.rootNode.addChildNode(textNode)
        }
        
        let gridNode = SCNNode.createGrid(gridSize: Float(size) * 5, elements: 4 * 5, color: UIColor(red: 0.4, green: 0.4, blue: 0.4, alpha: 1))
        gridNode.position = SCNVector3(x: 0, y: -Float(size) / 2, z: 0)
        scene.rootNode.addChildNode(gridNode)
        
        // plane for drag
        let plane = SCNBox(width: 100, height: 0, length: 100, chamferRadius: 0) //SCNPlane(width: 100, height: 100)
        planeNode = SCNNode(geometry: plane)
        planeNode.position = SCNVector3(0,0,0)
        //planeNode.rotation = SCNVector4Make(1, 0, 0, Float.pi/2)
        planeNode.isHidden = true
        scene.rootNode.addChildNode(planeNode)
        
        volumeBoxNode = SCNNode.createCube(cubeSize: CGFloat(size))
        scene.rootNode.addChildNode(volumeBoxNode)
        
        cameraConeNode = SCNNode.createCone(coneSize: 10, coneLength: 12)
        let box = SCNBox(width: 10, height: 10, length: 12, chamferRadius: 0)
        let node = SCNNode(geometry: box)
        node.position = SCNVector3(0,0,-6)
        node.isHidden = true
        cameraConeNode.addChildNode(node)
        scene.rootNode.addChildNode(cameraConeNode)

        
        
        volumeHelperBoxNode = SCNNode.createCube(cubeSize: CGFloat(size))
        scene.rootNode.addChildNode(volumeHelperBoxNode)
        
        lineCameraVolumeNode = SCNNode.createLineNode(from: SCNVector3(0, 0, 0), to: SCNVector3(0, 0, 0))
        scene.rootNode.addChildNode(lineCameraVolumeNode)
        
        axisNode = SCNNode.createAxis(axisLength: CGFloat(size) * 0.2)
        axisNode.position = SCNVector3(0, -CGFloat(size) / 2, 0)
        scene.rootNode.addChildNode(axisNode)

        scnView = self.view as! SCNView
        scnView.scene = scene
        scnView.allowsCameraControl = true
        scnView.showsStatistics = true
        scnView.backgroundColor = UIColor.black
        scnView.delegate = self
        scnView.isPlaying = true
        
        // add a tap gesture recognizer
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(handleTap(_:)))
        scnView.addGestureRecognizer(tapGesture)
    }
    
    @objc
    func handleTap(_ gestureRecognize: UIGestureRecognizer) {
        
    }
  
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        let touch  = (touches as NSSet).anyObject() as! UITouch

        let sceneView = self.view as! SCNView
        let location = touch.location(in: sceneView)
        
        let hitResults = sceneView.hitTest(location, options: [SCNHitTestOption.searchMode : SCNHitTestSearchMode.all.rawValue, SCNHitTestOption.boundingBoxOnly : true, SCNHitTestOption.rootNode : cameraConeNode, SCNHitTestOption.ignoreHiddenNodes: false])
        
        if hitResults.count > 0 {
            let result = hitResults[0]
            if  result.node.parent == cameraConeNode {
                dragEnable = true
                print("result 1" )
            }
            else {
                dragEnable = false
                print("result 2" )
            }
        }
        sceneView.allowsCameraControl = !dragEnable

    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        
        let touch    = (touches as NSSet).anyObject() as! UITouch
        let location = touch.location(in: scnView)
        
        if(dragEnable) {
            planeNode.position = SCNVector3(0, cameraConeNode.position.y, 0)
            let hitResults = scnView.hitTest(location, options: [SCNHitTestOption.searchMode : SCNHitTestSearchMode.all.rawValue, SCNHitTestOption.boundingBoxOnly : true, SCNHitTestOption.rootNode : planeNode, SCNHitTestOption.ignoreHiddenNodes: false])
            if hitResults.count > 0 {
                let result = hitResults[0]
                
                cameraPositionOffset.x = 2 * result.localCoordinates.x / Float(size)
                cameraPositionOffset.z = -2 * result.localCoordinates.z / Float(size)
                
                //print("result: " + String(format: "%.2f", cameraPositionOffset.x) + " , " + String(format: "%.2f", cameraPositionOffset.y) + " , " + String(format: "%.2f", cameraPositionOffset.z))
            }
        }
     }

    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        dragEnable = false
        scnView.allowsCameraControl = true
    }

    override var shouldAutorotate: Bool {
        return false
    }
    
    override var prefersStatusBarHidden: Bool {
        return true
    }
    
    override var supportedInterfaceOrientations: UIInterfaceOrientationMask {
        return .portrait
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Release any cached data, images, etc that aren't in use.
    }
    
}





