//Mach1
//Utility script to ensure all media is in sync upon play

using UnityEngine;
using System;
using System.Collections.Generic;

public class StartupManager : MonoBehaviour
{
    /// <summary>
    /// This holds references to the sounds we want to load and then play in unison
    /// </summary>

    public M1HorizonDecode[] sounds;
	//public M1HorizonDecode[] sounds2;

    //public MoviePlayerSample[] movies;

    public static Action OnLoadComplete;
    public GameObject loadingScreen;

    bool bLoadComplete;

    void Start()
    {
		sounds = GameObject.FindObjectsOfType<M1HorizonDecode>();
		//sounds2 = GameObject.FindObjectsOfType<M1HorizonDecode> ();

        // enable loading screen
        if (loadingScreen != null) loadingScreen.SetActive(true);

        /*
        if (movies.Count == 0)
        {
            Debug.LogWarning("Error: No MoviePlayerSamples assigned");
        }
        */

        if (sounds.Length == 0)
        {
            Debug.LogWarning("Warning: No DirectionalSounds assigned");
        }
    }

    /// <summary>
    /// This method has 3 stages:
    /// 1. Wait until audio is complete loading
    /// 2. Once ready, play media in the same frame
    /// 3. After that, disable self
    /// </summary>
    void Update()
    {
        if (!bLoadComplete)
        {
            CheckAudioReady();
        }
        else
        {
            /*
            for (int i = 0; i < movies.Count; i++)
            {
                movies[i].PlayVideo();
            }
            */
            for (int i = 0; i < sounds.Length; i++)
            {
                sounds[i].PlayAudio();
				//sounds2 [i].PlayAudio ();
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
        for (int i = 0; i < sounds.Length; i++)
        {
			if (!sounds[i].IsReady() )
            {
                bReady = false;
            }
        }

        if (bReady)
        {
            if (OnLoadComplete != null)
            {
                OnLoadComplete();
            }
            bLoadComplete = true;
            if (loadingScreen != null) loadingScreen.SetActive(false);
        }
    }
}
