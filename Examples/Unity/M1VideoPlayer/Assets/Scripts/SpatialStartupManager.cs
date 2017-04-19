using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class SpatialStartupManager : MonoBehaviour
{
    /// <summary>
    /// This holds references to the sounds we want to load and then play in unison
    /// </summary>
    public List<M1SpatialDecode> sounds = new List<M1SpatialDecode>();
    public List<MoviePlayerSample> movies = new List<MoviePlayerSample>();

    bool bLoadComplete;

    void Start()
    {
        if (movies.Count == 0)
        {
            Debug.LogWarning("Error: No MoviePlayerSamples assigned");
        }

        if (sounds.Count == 0)
        {
            Debug.LogWarning("Warning: No DirectionalSounds assigned");
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (!bLoadComplete)
        {
            CheckAudioReady();
        }
        else
        {
            for (int i = 0; i < movies.Count; i++)
            {
                movies[i].PlayVideo();
            }
            for (int i = 0; i < sounds.Count; i++)
            {
                sounds[i].PlayAudio();
            }
            gameObject.SetActive(false);
        }
    }

    /// <summary>
    /// Query our directional sounds to see if the audio was loaded
    /// </summary>
    void CheckAudioReady()
    {
        bool bReady = true;
        for (int i = 0; i < sounds.Count; i++)
        {
            if (!sounds[i].IsReady())
            {
                bReady = false;
            }
        }

        if (bReady)
        {
            bLoadComplete = true;
        }
    }
}
