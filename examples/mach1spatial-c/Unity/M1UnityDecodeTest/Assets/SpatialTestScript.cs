using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;

public class SpatialTestScript : MonoBehaviour {

    private Mach1.M1Decode m1;

    // Use this for initialization
    void Start() {
        m1 = new Mach1.M1Decode();
        m1.setAngularSettingsType(Mach1.M1Decode.AngularSettingsType.m1Unity);
    }

    float a = 0;
    
    // Update is called once per frame
    void Update() {
        //m1.beginBuffer(); // ???
        float[] data = m1.spatialAlgo(a, a, 0);
        a++;
        if (a > 360) a = 0;

        GameObject.Find("Text").GetComponent<Text>().text = "a: " + a + ", data: " + string.Join(", ", data.Select(f => f.ToString()).ToArray());
        //Debug.Log();
    }
}