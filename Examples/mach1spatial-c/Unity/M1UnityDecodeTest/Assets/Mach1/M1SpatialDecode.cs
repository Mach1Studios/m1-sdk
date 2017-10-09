//Mach1
//2016
//
//Set each audioSource item calls script

using UnityEngine;
using System.Collections;
using System.IO;

public class M1SpatialDecode :  M1Base
{
    public M1SpatialDecode()
    {
        InitComponents(8); 
    }

    public override float[] SoundAlgorithm(float Yaw, float Pitch, float Roll)
    {
        return m1Decode.spatialAlgo(Yaw, Pitch, Roll);
    } 
}
