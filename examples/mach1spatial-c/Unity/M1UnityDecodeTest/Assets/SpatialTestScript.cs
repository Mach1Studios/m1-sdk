using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;

public class SpatialTestScript : MonoBehaviour {

    private Mach1.Mach1Decode m1Decode;

    // Use this for initialization
    void Start() {
        m1Decode = new Mach1.Mach1Decode();

        m1Decode.setPlatformType(Mach1.Mach1PlatformType.Mach1PlatformUnity);
        m1Decode.setDecodeAlgoType(Mach1.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
    }

    float a = 0;
    
    // Update is called once per frame
    void Update() {
        //m1.beginBuffer(); // ???
        float[] data = new float[18];
        m1Decode.decode(a, a, 0, ref data);

        a++;
        if (a > 360) a = 0;

        GameObject.Find("Text").GetComponent<Text>().text = "a: " + a + ", data: " + string.Join(", ", data.Select(f => f.ToString()).ToArray());
        //Debug.Log();
    }
}