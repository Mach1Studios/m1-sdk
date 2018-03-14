//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

using UnityEngine;
using System.Collections;
using System.IO;

public class M1StSP : MonoBehaviour
{
    public AudioClip[] audioClip;

    [Space(10)]
    public string externalAudioPath = "file:///";
    public bool isFromStreamingAssets = true;
    public string[] externalAudioFilename;

    private const int MAX_SOUNDS_PER_CHANNEL = 1;

    [Space(10)]
    public bool autoPlay;
    private bool isPlaying;

    [Space(10)]
    public bool useFalloff = false;
    public AnimationCurve falloffCurve;

    private AudioSource[] audioSource;
    private int loadedCount;

    /*
    [Range(-1, 1)]
    public float Spatialize = 0;
    */

    public bool drawHelpers = true;

    M1StSP()
    {
        // Falloff
        Keyframe[] keyframes = new Keyframe[3];
        for (int i = 0; i < keyframes.Length; i++)
        {
            keyframes[i] = new Keyframe(i * 10, 1 - 1.0f * i / (keyframes.Length - 1));
        }

        falloffCurve = new AnimationCurve(keyframes);
        for (int i = 0; i < keyframes.Length; i++)
        {
            falloffCurve.SmoothTangents(i, 0);
        }

        // Init filenames
        externalAudioFilename = new string[MAX_SOUNDS_PER_CHANNEL];
        for (int i = 0; i < externalAudioFilename.Length; i++)
        {
            externalAudioFilename[i] = (i + 1) + ".wav";
        }

        // audioClip
        audioClip = new AudioClip[MAX_SOUNDS_PER_CHANNEL];
    }

    void Awake()
    {
    }

    void Start()
    {
        // Sounds
        audioSource = new AudioSource[MAX_SOUNDS_PER_CHANNEL * 2];

        loadedCount = 0;

        for (int i = 0; i < externalAudioFilename.Length; i++)
        {
            StartCoroutine(LoadAudio(Path.Combine(externalAudioPath, externalAudioFilename[i]), i, isFromStreamingAssets));
        }
    }

    // Helper function to add audio clip to source, and add this to scene
    AudioSource AddAudio(AudioClip clip, bool loop, bool playAwake, float vol)
    {
        AudioSource source = gameObject.AddComponent<AudioSource>();

        source.hideFlags = HideFlags.HideInInspector;
        source.clip = clip;
        source.loop = loop;
        source.playOnAwake = playAwake;
        source.volume = vol;

        return source;
    }

    // Draw gizmo in editor (you may display this also in game windows if set "Gizmo" button)
    void OnDrawGizmos()
    {
        if (drawHelpers)
        {
            Gizmos.color = Color.gray;
            Gizmos.DrawIcon(transform.position, "sound_icon.png", true);
        }
    }

    // Load audio
    IEnumerator LoadAudio(string url, int n, bool isFromAssets)
    {
        AudioClip clip = null;

        if (isFromAssets)
        {
            clip = audioClip[n];// Resources.Load<AudioClip>(url);
        }
        else
        {
            WWW www = new WWW(url);
            yield return www;
            if (www.error == null)
            {
                clip = www.GetAudioClip(false, false);
            }
            else
            {
                Debug.Log("WWW Error: " + www.error);
            }
        }

        if (clip != null)
        {
            float[] bufMid = new float[clip.samples * clip.channels];
            float[] rawWaveData = new float[clip.samples * clip.channels];

            clip.GetData(rawWaveData, 0);

            float l, r;
            float mid, slide;

            for (int position = 0; position < rawWaveData.Length; position += clip.channels)
            {
                l = rawWaveData[position + 0];
                r = rawWaveData[position + 1];

                mid = (l + r) / 2;
                slide = (l - r) / 2;

                bufMid[position + 0] = mid;
                bufMid[position + 1] = mid;
            }

            // Init sound
            AudioClip clipMid = AudioClip.Create("clipMid", clip.samples, clip.channels, clip.frequency, false);
            clipMid.SetData(bufMid, 0);

            audioSource[0] = AddAudio(clipMid, false, true, 1.0f);
            audioSource[0].spatialize = true;

            audioSource[1] = AddAudio(clip, false, true, 1.0f);
            audioSource[1].panStereo = 0;

            audioSource[0].loop = true;
            audioSource[1].loop = true;

            loadedCount++;
        }

        yield break;
    }

    public bool IsReady()
    {
        return loadedCount == MAX_SOUNDS_PER_CHANNEL;
    }

    public void PlayAudio()
    {
        if (IsReady())
        {
            for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
            {
                audioSource[i].Play();
            }
        }
        else
        {
            Debug.LogError("Audio was not loaded");
        }
    }

    public float Map(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    // Update is called once per frame
    void Update()
    {
        if (IsReady())
        {
            if (autoPlay && !isPlaying)
            {
                isPlaying = true;
                PlayAudio();
            }

            float volume = 1.0f;

            if (useFalloff)
            {
                volume = volume * falloffCurve.Evaluate(Vector3.Distance(Camera.main.transform.position, transform.position));
            }

            /*
            float s = Map(Spatialize, -1.0f, 1.0f, 0.0f, 1.0f);

            float panL = Mathf.Cos(s * (0.5f * Mathf.PI));
            float panR = Mathf.Cos((1.0f - s) * (0.5f * Mathf.PI));

            audioSource[0].volume = volume * panL;
            audioSource[1].volume = volume;
            audioSource[2].volume = volume * panR;
            */
        }

    }

}
