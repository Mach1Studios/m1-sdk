//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Mach1Point4D
    {
        public float x;
        public float y;
        public float z;
        public float w;

        public Mach1Point4D(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
    }
}
