import Foundation
import SceneKit

extension SCNQuaternion {
    public var q:(Float,Float,Float,Float) {
        return (self.x, self.y, self.z, self.w)
    }
    public init(q:(Float,Float,Float,Float)) {
        self.init(x: q.0, y: q.1, z: q.2, w: q.3)
    }
    
    public func toGLK() -> GLKQuaternion {
        return GLKQuaternion(q: self.q)
    }
}
