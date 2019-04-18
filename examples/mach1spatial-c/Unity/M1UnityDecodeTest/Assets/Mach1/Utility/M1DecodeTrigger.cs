using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class M1DecodeTrigger : MonoBehaviour
{
    public M1SpatialDecode[] m1objs;
    public static Action OnLoadComplete;
    public static Action OnTrigger;
    bool bLoadComplete;
    bool bNeedToPlay;
    bool bAudioPlayed;

    void Start()
    {
       // OnTrigger += NeedToPlay;
        bAudioPlayed = false;

        /*
        // Try to find all Mach1Spatial objects");
            sounds = GameObject.FindObjectsOfType<M1SpatialDecode>();
            if (sounds.Length == 0)
            {
                Debug.LogWarning("Warning: No Mach1Spatial Objects Detected");
            }

        */

        if (m1objs.Length == 0)
        {
            Debug.LogWarning("Warning: No Mach1Spatial Objects Detected");
        }
        else
        {
            for (int i = 0; i < m1objs.Length; i++)
            {
                //m1objs[i].PlayAudio();
            }
        }
    }

    public void NeedToPlay()
    {
        bNeedToPlay = true;
    }

    void Update()
    {
        if (!bLoadComplete)
        {
            CheckAudioReady();
        }
        else if (bNeedToPlay)
        {
            for (int i = 0; i < m1objs.Length; i++)
            {
                m1objs[i].PlayAudio();
            }

            //gameObject.SetActive(false);
            bNeedToPlay = false;
            bAudioPlayed = true;
            Debug.Log("play " + gameObject.name);
        }
    }

    public void loadAll()
    {
        if (!bAudioPlayed)
        {
            for (int i = 0; i < m1objs.Length; i++)
            {
                m1objs[i].LoadAudioData();
            }

            Debug.Log("load " + gameObject.name);
        }
    }

    public void unloadAll()
    {
        Debug.Log("Unload " + gameObject.name);
        for (int i = 0; i < m1objs.Length; i++)
        {
            m1objs[i].UnloadAudioData();
        }
    }

    public void StopAll()
    {
        if (bAudioPlayed)
        {
            for (int i =0; i < m1objs.Length; i++)
            {
                m1objs[i].StopAudio();
            }
        }
        else if (!bAudioPlayed)
        {
            Debug.LogWarning("Audio Not Played Yet");
        }
    }

    void CheckAudioReady()
    {
        bool bReady = true;
        for (int i = 0; i < m1objs.Length; i++)
        {
            if (!m1objs[i].IsReady())
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
            Debug.Log("Loaded " + gameObject.name);
        }
    }
}