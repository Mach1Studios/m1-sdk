//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

using UnityEngine;
using System.Collections;
using System.IO;

public class M1HorizonDecode :  M1Base
{
    public M1HorizonDecode()
    {
        InitComponents(4);

        m1Decode.setDecodeAlgoType(Mach1.Mach1DecodeAlgoType.Mach1DecodeAlgoHorizon);
    }
}
