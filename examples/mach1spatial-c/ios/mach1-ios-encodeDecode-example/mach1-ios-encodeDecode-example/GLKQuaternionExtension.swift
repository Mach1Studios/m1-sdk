import Foundation
import SceneKit

extension GLKQuaternion {
    public func toSCN() -> SCNQuaternion {
        return SCNQuaternion(q: self.q)
    }
}
