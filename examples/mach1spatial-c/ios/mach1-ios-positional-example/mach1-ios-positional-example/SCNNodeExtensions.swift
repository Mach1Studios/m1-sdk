import Foundation
import SceneKit

extension SCNNode {
    static func createLineNode(from: SCNVector3, to: SCNVector3, radius: CGFloat = 0.25) -> SCNNode {
        let vector = to - from
        let height = vector.length()
        let cylinder = SCNCylinder(radius: radius, height: CGFloat(height))
        cylinder.radialSegmentCount = 4
        let node = SCNNode(geometry: cylinder)
        node.position = (to + from) / 2
        node.eulerAngles = SCNVector3.lineEulerAngles(vector: vector)
        return node
    }
    
    static func createCube(cubeSize: CGFloat = 10.0) -> SCNNode {
        let cubeNode = SCNNode()
        cubeNode.position = SCNVector3Make(0, 0, 0)
        
        let size = cubeSize / 2
        var points1 = [
            SCNVector3(-size, -size, size),
            SCNVector3(size, -size, size),
            SCNVector3(size, -size, -size),
            SCNVector3(-size, -size, -size),
            ]
        
        var points2 = [
            SCNVector3(-size, size, size),
            SCNVector3(size, size, size),
            SCNVector3(size, size, -size),
            SCNVector3(-size, size, -size),
            ]
        
        for i in 0...3 {
            var j = i + 1
            if i == 3 {
                j = 0
            }
            let line1 = SCNNode.createLineNode(from: points1[i], to: points1[j], radius: 0.2)
            cubeNode.addChildNode(line1)
            
            let line12 = SCNNode.createLineNode(from: points1[i], to: points2[i], radius: 0.2)
            cubeNode.addChildNode(line12)
            
            let line2 = SCNNode.createLineNode(from: points2[i], to: points2[j], radius: 0.2)
            cubeNode.addChildNode(line2)
            
        }
        
        return cubeNode
    }
    
    static func createCone(coneSize: CGFloat = 10.0, coneLength: CGFloat = 10.0) -> SCNNode {
        let coneNode = SCNNode()
        coneNode.position = SCNVector3Make(0, 0, 0)
        
        let size = coneSize / 2
        var points1 = [
            SCNVector3(-size, size, -coneLength),
            SCNVector3(size, size, -coneLength),
            SCNVector3(size, -size, -coneLength),
            SCNVector3(-size, -size, -coneLength),
            ]
        
        for i in 0...3 {
            var j = i + 1
            if i == 3 {
                j = 0
            }
            let line1 = SCNNode.createLineNode(from: points1[i], to: points1[j], radius: 0.2)
            coneNode.addChildNode(line1)
             
            let line2 = SCNNode.createLineNode(from: points1[i], to: SCNVector3(0, 0, 0), radius: 0.2)
            coneNode.addChildNode(line2)
            
        }
        
        return coneNode
    }
    
    static func createGrid(gridSize: Float = 10.0, elements: Int = 2) -> SCNNode {
        let gridNode = SCNNode()
        gridNode.position = SCNVector3Make(0, 0, 0)
        let scale = gridSize * 1.0 / Float(elements)
        let cnt : Int = elements/2
        for x in -cnt...cnt {
            for z in -cnt...cnt {
                if x != cnt {
                    let line = SCNNode.createLineNode(from: SCNVector3(Float(x) * scale, 0, Float(z) * scale), to: SCNVector3((Float(x) + 1) * scale, 0, Float(z) * scale), radius: 0.2)
                    gridNode.addChildNode(line)
                }
                if z != cnt {
                    let line = SCNNode.createLineNode(from: SCNVector3(Float(x) * scale, 0, Float(z) * scale), to: SCNVector3(Float(x) * scale, 0, (Float(z) + 1) * scale), radius: 0.2)
                    gridNode.addChildNode(line)
                }
            }
        }
        
        return gridNode
    }
    
}

extension SCNVector3 {
    static func lineEulerAngles(vector: SCNVector3) -> SCNVector3 {
        let height = vector.length()
        let lxz = sqrtf(vector.x * vector.x + vector.z * vector.z)
        let pitchB = vector.y < 0 ? Float.pi - asinf(lxz/height) : asinf(lxz/height)
        let pitch = vector.z == 0 ? pitchB : sign(vector.z) * pitchB
        
        var yaw: Float = 0
        if vector.x != 0 || vector.z != 0 {
            let inner = vector.x / (height * sinf(pitch))
            if inner > 1 || inner < -1 {
                yaw = Float.pi / 2
            } else {
                yaw = asinf(inner)
            }
        }
        return SCNVector3(CGFloat(pitch), CGFloat(yaw), 0)
    }
}

