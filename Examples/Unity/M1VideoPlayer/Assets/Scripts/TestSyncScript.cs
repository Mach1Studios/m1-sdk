using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestSyncScript : MonoBehaviour {

    public List<M1SpatialDecode> sounds = new List<M1SpatialDecode>();
    public List<MediaPlayerCtrl> movies = new List<MediaPlayerCtrl>();

    // Use this for initialization
    void Start () {

		
	}
	
	// Update is called once per frame
	void Update () {
        for (int i = 0; i < sounds.Count; i++)
        {
            print("audio time " + i + ": " + sounds[i].GetPositon());
        }

        for (int i = 0; i < movies.Count; i++)
        {
            print("movie time " + i + ": " + movies[i].GetSeekPosition());
        }

    }
}
