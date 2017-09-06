//Mach1
//2016
//
//Set each audioSource item calls script

using UnityEngine;
using System.Collections;
using System.IO;

public class M1HorizonDecode :  M1Base
{
    public M1HorizonDecode()
    {
        InitComponents(4); 
    }

    public override float[] SoundAlgorithm(float Yaw, float Pitch, float Roll)
    {
        return m1DSPAlgorithms.fourChannelAlgorithm(Yaw, Pitch, Roll, false);
    } 
}
