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

class GameViewController: UIViewController, SCNSceneRendererDelegate {
    
    var scene:SCNScene! = nil
    var axisNode: SCNNode! = nil
    var cameraConeNode: SCNNode! = nil
    var volumeBoxNode: SCNNode! = nil
    var volumeHelperBoxNode: SCNNode! = nil
    var lineCameraVolumeNode: SCNNode! = nil
    var sphereLeftNodes : [SCNNode ] = []
    var sphereRightNodes : [SCNNode ] = []
    var size : Int = 30
    
    func deg2rad(_ number: Float) -> Float {
        return number * .pi / 180
    }
    
    func renderer(_ renderer: SCNSceneRenderer, updateAtTime time: TimeInterval)
    {
        let angleSet = m1obj.getCurrentAngle()
        
        var angleVolume:Mach1Point3D = m1obj.getVolumeRotation();
        
        volumeHelperBoxNode.eulerAngles = SCNVector3(x: deg2rad(angleVolume.x), y: deg2rad(angleVolume.y), z: deg2rad(angleVolume.z))

        cameraConeNode.eulerAngles =  SCNVector3(x: deg2rad(cameraPitch), y: deg2rad(cameraYaw), z: deg2rad(cameraRoll))
        cameraConeNode.position =  SCNVector3(x: cameraPosition.x * Float(size)/2, y: cameraPosition.y * Float(size)/2, z: -cameraPosition.z * Float(size)/2)

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
        
        volumeBoxNode = SCNNode.createCube(cubeSize: CGFloat(size))
        scene.rootNode.addChildNode(volumeBoxNode)
        
        cameraConeNode = SCNNode.createCone(coneSize: 10, coneLength: 12)
        scene.rootNode.addChildNode(cameraConeNode)
    
        volumeHelperBoxNode = SCNNode.createCube(cubeSize: CGFloat(size))
        scene.rootNode.addChildNode(volumeHelperBoxNode)
        
        lineCameraVolumeNode = SCNNode.createLineNode(from: SCNVector3(0, 0, 0), to: SCNVector3(0, 0, 0))
        scene.rootNode.addChildNode(lineCameraVolumeNode)
        
        axisNode = SCNNode.createAxis(axisLength: CGFloat(size) * 0.2)
        axisNode.position = SCNVector3(0, -CGFloat(size) / 2, 0)
        scene.rootNode.addChildNode(axisNode)

        let scnView = self.view as! SCNView
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
        let sceneView = self.view as! SCNView
        let location = gestureRecognize.location(in: sceneView)
        let hitResults = sceneView.hitTest(location, options: [SCNHitTestOption.searchMode : 1])
        
        if hitResults.count > 0 {
            let result = hitResults[0]
            
            if  result.node == cameraConeNode {
                cameraConeNode.geometry?.firstMaterial!.diffuse.contents = UIColor.green.cgColor
            }
        }
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





