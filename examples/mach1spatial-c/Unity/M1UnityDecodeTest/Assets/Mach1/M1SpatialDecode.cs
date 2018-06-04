//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

using UnityEngine;
using System.Collections;
using System.IO;

public class M1SpatialDecode :  M1Base
{
    public M1SpatialDecode()
    {
        InitComponents(8);

        m1Decode.setDecodeAlgoType(Mach1.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
    }
}
