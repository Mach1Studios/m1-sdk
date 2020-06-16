//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

import Foundation
import SceneKit

public class Mach1Transcode {
    var M1obj : UnsafeMutableRawPointer

    public init() {
        M1obj = Mach1TranscodeCAPI_create()
    }

    deinit {
        Mach1TranscodeCAPI_delete(M1obj)
    }

    public func getInputNumChannels() -> Int {
        let count = Mach1TranscodeCAPI_getInputNumChannels(M1obj)
        return Int(count)
        /// Returns the number of input channels for indicated input format
        ///
        /// - Parameters:
        ///     - integer of number of input channels
    }

    public func getOutputNumChannels() -> Int {
        let count = Mach1TranscodeCAPI_getOutputNumChannels(M1obj)
        return Int(count)
        /// Returns the number of output channels for indicated output format
        ///
        /// - Parameters:
        ///     - integer of number of output channels
    }
}
