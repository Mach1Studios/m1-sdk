using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;

public class TestScript : MonoBehaviour {

    private Mach1.M1DSPAlgorithms m1;

    // Use this for initialization
    void Start() {
        m1 = new Mach1.M1DSPAlgorithms();
    }

    // Update is called once per frame
    void Update() {
        float[] data = m1.eightChannelsIsotropicAlgorithm(0, 0, 0, false);
        GameObject.Find("Text").GetComponent<Text>().text = string.Join(", ", data.Select(f => f.ToString()).ToArray());
        //Debug.Log();
    }
}