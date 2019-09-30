//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

using System.Collections;
using System.IO;
using UnityEngine;

public class M1BaseEncode : MonoBehaviour
{
    [HideInInspector]
    public float[][] gains;

    [Header("Asset Source Settings")]
    public bool isFromAssets = true;
    public AudioClip[] audioClips;

    public string externalAudioPath = "file:///";
    public string[] externalAudioFilenameMain;

    [Header("Asset Load/Play Settings")]
    public bool autoPlay = false;
    public bool isLoop = false;
    public bool loadAudioOnStart = true;
    private bool isPlaying = false;

    [Header("Encode Settings")]
    // TODO

    private int MAX_SOUNDS;
    private M1BaseDecode m1BaseDecode;
    private bool needToPlay;

    protected Mach1.Mach1Encode m1Encode = new Mach1.Mach1Encode();

    static Mach1.Mach1Point3D ConvertToMach1Point3D(Vector3 vec)
    {
        return new Mach1.Mach1Point3D(vec.x, vec.y, vec.z);
    }

    static Mach1.Mach1Point4D ConvertToMach1Point4D(Vector4 vec)
    {
        return new Mach1.Mach1Point4D(vec.x, vec.y, vec.z, vec.w);
    }

    static Mach1.Mach1Point4D ConvertToMach1Point4D(Quaternion quat)
    {
        return new Mach1.Mach1Point4D(quat.x, quat.y, quat.z, quat.w);
    }

    static Vector3 ConvertFromMach1Point3D(Mach1.Mach1Point3D pnt)
    {
        return new Vector3(pnt.x, pnt.y, pnt.z);
    }

    AnimationCurve generateCurve(float length)
    {
        Keyframe[] keyframes = new Keyframe[3];
        for (int i = 0; i < keyframes.Length; i++)
        {
            keyframes[i] = new Keyframe(i * length / 2.0f, 1 - 1.0f * i / (keyframes.Length - 1));
        }

        AnimationCurve curve = new AnimationCurve(keyframes);
        for (int i = 0; i < keyframes.Length; i++)
        {
            curve.SmoothTangents(i, 0);
        }
        return curve;
    }

    public M1BaseEncode()
    {
        // TODO 
        m1Encode.setInputMode(Mach1.Mach1EncodeInputModeType.Mach1EncodeInputModeMono);
        m1Encode.setOutputMode(Mach1.Mach1EncodeOutputModeType.Mach1EncodeOutputMode8Ch);

     
    }

    protected void InitComponents(int MAX_SOUNDS_PER_CHANNEL)
    {
        this.MAX_SOUNDS = MAX_SOUNDS_PER_CHANNEL;

        // Init filenames
        externalAudioFilenameMain = new string[MAX_SOUNDS_PER_CHANNEL];
        for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
        {
            externalAudioFilenameMain[i] = (i + 1) + ".wav";
        }

        // audioClips
        audioClips = new AudioClip[MAX_SOUNDS_PER_CHANNEL];
    }

    void Awake()
    {
    }

    void Start()
    {
        if (loadAudioOnStart)
        {
            LoadAudioData();
        }

        attachM1BaseDecode();
    }

    public void LoadAudioData()
    {
        // Sounds
        for (int i = 0; i < Mathf.Max(externalAudioFilenameMain.Length, audioClips.Length); i++)
        {
            StartCoroutine(LoadAudio(Path.Combine(externalAudioPath, i < externalAudioFilenameMain.Length ? externalAudioFilenameMain[i] : ""), i, isFromAssets));
        }

        isPlaying = false;
    }

    public void UnloadAudioData()
    {
        if (isFromAssets)
        {
            for (int i = 0; i < audioClips.Length; i++)
            {
                audioClips[i].UnloadAudioData();
            }

        }
        else
        {
            for (int i = 0; i < audioClips.Length; i++)
            {
                AudioClip.Destroy(audioClips[i]);
            }
        }

        isPlaying = false;
    }

    public void attachM1BaseDecode()
    {
        m1BaseDecode = GameObject.FindObjectOfType<M1BaseDecode>();
    }

    // Draw gizmo in editor (you may display this also in game windows if set "Gizmo" button)
    void OnDrawGizmos()
    {
        Gizmos.color = Color.gray;
        Gizmos.matrix = gameObject.transform.localToWorldMatrix;
        Gizmos.DrawWireSphere(new Vector3(0, 0, 0), 0.5f);
    }

    string GetStreamingAssetsPath()
    {
        string path;
#if UNITY_EDITOR
        path = "file://" + Application.dataPath + "/StreamingAssets";
#elif UNITY_ANDROID
     path = "jar:file://"+ Application.dataPath + "!/assets";
#elif UNITY_IOS
     path = "file://" + Application.dataPath + "/Raw";
#else
     //Desktop (Mac OS or Windows)
     path = "file://"+ Application.dataPath + "/StreamingAssets";
#endif
        return path;
    }

    // Load audio
    IEnumerator LoadAudio(string url, int n, bool isFromAssets)
    {
        AudioClip clip = null;
        

        if (isFromAssets)
        {
            clip = audioClips[n];

            if (clip != null)
            { 
                clip.LoadAudioData();
            }
        }
        else
        {
            url = url.Replace("$CURDIR", "file:///" + Directory.GetCurrentDirectory());
            url = url.Replace("$STREAMINGASSETS", GetStreamingAssetsPath());

            //Debug.Log ("load audio : " + url);

            WWW www = new WWW(url);
            yield return www;
            if (www.error == null)
            {
                clip = www.GetAudioClip(false, false);
            }
            else
            {
                Debug.Log("WWW Error: " + www.error + " (" + url + ")");
            }
        }

        if (clip != null)
        {
            audioClips[n] = clip;
        }

        yield break;
    }

    public bool IsReady()
    { 
        if(isPlaying == false)
        {
            bool isLoaded = true;
            for (int i = 0; i < audioClips.Length; i++)
            {
                if (audioClips[i].loadState != AudioDataLoadState.Loaded)
                {
                    isLoaded = false;
                    break;
                } 
            }

            return isLoaded;
        }
        else
        {
            return true;
        }
    }

    public void PlayAudio()
    {
        needToPlay = true;
    }

    public void StopAudio()
    {
        if (isPlaying)
        {

        }
    }

    public void Seek(float timeInSeconds)
    {
        // Not inplemented yet
    }

   public float GetPosition()
    {
        // Not inplemented yet
        return 0;
    }

    public float GetDuration()
    {
        // Not inplemented yet
        return 0;
    }

    /*
    public bool IsPlaying()
    {
        return false;
    }
    */

    public string ToStringFormat(Vector3 v)
    {
        string fmt = "0.0000";
        return "( " + v.x.ToString(fmt) + ", " + v.y.ToString(fmt) + ", " + v.z.ToString(fmt) + " )";
    }

    public string ToStringFormat(Quaternion q)
    {
        string fmt = "0.0000";
        return "( " + q.x.ToString(fmt) + ", " + q.y.ToString(fmt) + ", " + q.z.ToString(fmt) + ", " + q.w.ToString(fmt) + " )";
    }

    public Vector3 GetEuler(Quaternion q1)
    {
        float test = q1.x * q1.y + q1.z * q1.w;
        if (test > 0.499) // singularity at north pole
        {
            return new Vector3(
                0,
                2 * Mathf.Atan2(q1.x, q1.w),
                Mathf.PI / 2
            ) * Mathf.Rad2Deg;
        }
        if (test < -0.499) // singularity at south pole
        {
            return new Vector3(
                0,
                -2 * Mathf.Atan2(q1.x, q1.w),
                -Mathf.PI / 2
            ) * Mathf.Rad2Deg;
        }
        float sqx = q1.x * q1.x;
        float sqy = q1.y * q1.y;
        float sqz = q1.z * q1.z;

        return new Vector3(
            Mathf.Atan2(2.0f * q1.x * q1.w - 2 * q1.y * q1.z, 1.0f - 2.0f * sqx - 2.0f * sqz),
            Mathf.Atan2(2.0f * q1.y * q1.w - 2 * q1.x * q1.z, 1.0f - 2.0f * sqy - 2.0f * sqz),
            Mathf.Sin(2.0f * test)
        ) * Mathf.Rad2Deg;
    }

    public bool IsPlaying()
    {
        return isPlaying;
    }

    // Update is called once per frame
    void Update()
    {
        if (m1BaseDecode == null)
        {
            Debug.LogError("Mach1: cannot find AudioListener!");
            attachM1BaseDecode();
            return;
        }

        if (IsReady())
        {
            if ((autoPlay || needToPlay) && !isPlaying)
            {
                needToPlay = false;
                isPlaying = true;
            }

            Vector3 vec = gameObject.transform.position - m1BaseDecode.transform.position;

            float angle = -Mathf.Atan2(vec.x, -vec.z) / Mathf.PI;
            angle = 0.5f + angle / 2;

            m1Encode.setIsotropicEncode(true);
            m1Encode.setRotation(angle);
            m1Encode.setPitch( Mathf.Clamp(vec.y / 10,-1,1));
            m1Encode.setDiverge(Mathf.Clamp(vec.x / 10, -1, 1));
            m1Encode.generatePointResults();

            gains = m1Encode.getGains();

            string fmt = "";
            for(int i=0; i < gains[0].Length; i++ )
            {
                fmt += " , " + gains[0][i];
            }
            Debug.Log("gains: " + fmt);

            //Debug.Log("vec.x / 10: " + vec.x / 10 +  " , " +  vec.y / 10);
        }
    }
}