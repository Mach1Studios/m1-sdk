//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

using UnityEngine;
using UnityEngine.Audio;
using System.Collections;
using System.IO;
using UnityEditor;
using System.Text;
using System;
using UnityEngine.Networking;

[CustomEditor(typeof(M1BaseDecode), true)]
[CanEditMultipleObjects]
public class M1BaseDecodeEditor : Editor
{
    Vector2 layoutPosition = new Vector2(0, 0);
    string[] sourceTypeList = { "Encode Object", "Audio Clip (Assets)", "External Audio (Streaming Assets)" };

    public override void OnInspectorGUI()
    {
        EditorGUILayout.LabelField("Source Type", EditorStyles.boldLabel);

        ((M1BaseDecode)target).sourceType = (M1BaseDecode.M1BaseDecodeSourceType)GUILayout.SelectionGrid((int)(((M1BaseDecode)target).sourceType), sourceTypeList, 1, EditorStyles.radioButton);

        DrawDefaultInspector();
    }
}

[AddComponentMenu("")]
public class M1BaseDecode : MonoBehaviour
{
    [HideInInspector]
    public M1BaseDecodeSourceType sourceType;
    public enum M1BaseDecodeSourceType : int
    {
        M1BaseDecodeSourceEncodeObject = 0, M1BaseDecodeSourceAudioClip = 1, M1BaseDecodeSourceExternalAudio = 2
    };

    M1DSPPlayer dspPlayerEncode;
    M1DSPPlayer dspPlayerMain;
    M1DSPPlayer dspPlayerBlend;

    [Header("Source Settings")]
    public M1BaseEncode[] m1EncodeObjects;

    public AudioClip[] audioClipMain;

    public string externalAudioPath = "file:///";
    public string[] externalAudioFilenameMain;

    public string outputFilename = "output.wav";
    public bool useWriter = false;

    [Header("Load/Play Settings")]
    public bool autoPlay = false;
    public bool isLoop = false;
    public bool loadAudioOnStart = true;
    private bool isPlaying = false;

    [Header("Attenuation Settings")]
    public bool useAttenuation = false;
    public AnimationCurve attenuationCurve;
   
    private int MAX_SOUNDS_PER_CHANNEL;
    private Matrix4x4 mat;
    private Matrix4x4 matInternal;

    [Header("Point / Plane Setting")]
    public bool usePlaneCalculation = false;

    [Header("Advanced Settings")]
    public bool muteWhenInsideObject = false;
    public bool muteWhenOutsideObject = false;

    public bool useYawForRotation = true;
    public bool usePitchForRotation = true;
    public bool useRollForRotation = true;

    public bool drawHelpers = false;
    public bool debug = false;

    [Header("Experimental BlendMode")]
    public bool useBlendMode = false;
    public AudioClip[] audioClipBlend;
    public string[] externalAudioFilenameBlend;
    public AnimationCurve attenuationCurveBlendMode;
    public bool ignoreTopBottom = true;

    private float[] coeffs;
    private float[] coeffsInterior;

    private AudioListener audiolistener;
    private bool needToPlay;

    protected Mach1.Mach1DecodePositional m1Positional = new Mach1.Mach1DecodePositional();

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

    public M1BaseDecode()
    {
        coeffs = new float[18];
        coeffsInterior = new float[18];

        m1Positional.setPlatformType(Mach1.Mach1PlatformType.Mach1PlatformUnity);
    }

    protected void InitComponents(int MAX_SOUNDS_PER_CHANNEL)
    {
        this.MAX_SOUNDS_PER_CHANNEL = MAX_SOUNDS_PER_CHANNEL;

        // Falloff
        attenuationCurve = generateCurve(10);
        attenuationCurveBlendMode = generateCurve(1);

        // Init filenames
        externalAudioFilenameMain = new string[MAX_SOUNDS_PER_CHANNEL];
        externalAudioFilenameBlend = new string[MAX_SOUNDS_PER_CHANNEL];
        for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
        {
            externalAudioFilenameMain[i] = (i + 1) + ".wav";
            externalAudioFilenameBlend[i] = (i + 1) + ".wav";
        }

        // audioClip
        audioClipMain = new AudioClip[MAX_SOUNDS_PER_CHANNEL];
        audioClipBlend = new AudioClip[MAX_SOUNDS_PER_CHANNEL];
    }

    void Awake()
    {
    }

    void Start()
    {
        if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceEncodeObject)
        {
            dspPlayerEncode = new M1DSPPlayer();
            dspPlayerEncode.countPlayers = m1EncodeObjects.Length;
            dspPlayerEncode.useEncode = true;
            dspPlayerEncode.useWriter = useWriter;
            dspPlayerEncode.Start();
        }
        else
        {
            dspPlayerMain = new M1DSPPlayer();
            dspPlayerMain.countPlayers = 1;
            dspPlayerMain.useWriter = useWriter;
            dspPlayerMain.Start();

            dspPlayerBlend = new M1DSPPlayer();
            dspPlayerBlend.countPlayers = 1;
            dspPlayerBlend.Start();
        }

        if (loadAudioOnStart)
        {
            LoadAudioData();
        }

        attachAudioListener();
    }

    public void OnDestroy()
    {
        if (useWriter && M1DSPWriterNode.writerData.Count > 0)
        {
            int numsamples = M1DSPWriterNode.writerData.Count * M1DSPWriterNode.writerData[0].Length / 8;
            ushort numchannels = 8;
            ushort samplelength = 2; // in bytes
            uint samplerate = 48000;

            FileStream fs = new FileStream(outputFilename, FileMode.Create);
            BinaryWriter wr = new BinaryWriter(fs);
            // 8 bytes
            wr.Write(Encoding.ASCII.GetBytes("RIFF"));  // RIFF
            wr.Write((int)(36 + numsamples * numchannels * samplelength)); // Filesize - 8 (Int32 + RIFF)

            // 28 bytes
            wr.Write(Encoding.ASCII.GetBytes("WAVEfmt "));  // WAVEfmt (8)
            wr.Write(16);   // size of waveformat data (4) int
            wr.Write((ushort)1);    // wFormatTag (2) ushort
            wr.Write(numchannels);  // nChannels (2) ushort
            wr.Write(samplerate);   // sample rate (4) uint
            wr.Write(samplerate * samplelength * numchannels);  // nAvgBytesPerSec (4) uint
            wr.Write((ushort)(samplelength * numchannels));   // nBlockAlign (2) ushort
            wr.Write((ushort)(8 * samplelength));   // wBitsPerSample (2) ushort

            // 8
            wr.Write(Encoding.ASCII.GetBytes("data"));  // data (4)
            wr.Write((int)(numsamples * samplelength));    // length of data in bytes (4) int

            for (int i = 0; i < M1DSPWriterNode.writerData.Count; i++)
            {
                for (int j = 0; j < M1DSPWriterNode.writerData[i].Length; j++)
                {
                    wr.Write((short)(short.MaxValue * M1DSPWriterNode.writerData[i][j]));
                }
            }

            wr.Flush();
            wr.Close();
            fs.Close();

            Debug.Log("finish: " + M1DSPWriterNode.writerData.Count);
        }
    }

    public void LoadAudioData()
    {
        // Sounds
        if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceAudioClip || sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceExternalAudio)
        { 
            for (int i = 0; i < Mathf.Max(externalAudioFilenameMain.Length, audioClipMain.Length); i++)
            {
                StartCoroutine(LoadAudio(Path.Combine(externalAudioPath, i < externalAudioFilenameMain.Length ? externalAudioFilenameMain[i] : ""), false, i, sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceAudioClip));
            }

            if (useBlendMode)
            {
                // Sounds
                for (int i = 0; i < Mathf.Max(externalAudioFilenameBlend.Length, audioClipBlend.Length); i++)
                {
                    StartCoroutine(LoadAudio(Path.Combine(externalAudioPath, i < externalAudioFilenameBlend.Length ? externalAudioFilenameBlend[i] : ""), true, i, sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceAudioClip));
                }
            }
        }
        else
        {
            // TODO
        }
        isPlaying = false;
    }

    public void UnloadAudioData()
    {
        if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceAudioClip)
        {
            for (int i = 0; i < audioClipMain.Length; i++)
            {
                audioClipMain[i].UnloadAudioData();
            }

            for (int i = 0; i < audioClipBlend.Length; i++)
            {
                audioClipBlend[i].UnloadAudioData();
            }
        }
        else if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceExternalAudio)
        {
            for (int i = 0; i < audioClipMain.Length; i++)
            {
                AudioClip.Destroy(audioClipMain[i]);
            }
            for (int i = 0; i < audioClipBlend.Length; i++)
            {
                AudioClip.Destroy(audioClipBlend[i]);
            }
        }


        isPlaying = false;
    }

    public void attachAudioListener() 
    {
        audiolistener = GameObject.FindObjectOfType<AudioListener>();
    }

     // Draw gizmo in editor (you may display this also in game windows if set "Gizmo" button)
    void OnDrawGizmos()
    {
        if (drawHelpers)
        {
            //Gizmos.DrawIcon(transform.position, "sound_icon.png", true);

            Gizmos.color = Color.gray;
            Gizmos.matrix = gameObject.transform.localToWorldMatrix;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));

            /*
            Gizmos.color = Color.magenta;
            Gizmos.matrix = mat;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));
            */

            Gizmos.color = Color.yellow;
            Gizmos.matrix = matInternal;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));

            float radius = 0.1f;

            Vector3[] edges = new Vector3[8] {
                new Vector3(-0.5f, 0.5f, 0.5f),
                new Vector3(0.5f, 0.5f, 0.5f),
                new Vector3(-0.5f, 0.5f, -0.5f),
                new Vector3(0.5f, 0.5f, -0.5f),
                new Vector3(-0.5f, -0.5f, 0.5f),
                new Vector3(0.5f, -0.5f, 0.5f),
                new Vector3(-0.5f, -0.5f, -0.5f),
                new Vector3(0.5f, -0.5f, -0.5f),
            };

            for (int i = 0; i < 8; i++)
            {
                Gizmos.color = Color.red;
                Gizmos.matrix = gameObject.transform.localToWorldMatrix * (Matrix4x4.Translate(new Vector3(-radius, 0, 0)) * Matrix4x4.Translate(edges[i]));
                Gizmos.DrawSphere(new Vector3(0, 0, 0), radius * coeffs[2 * i]);

                Gizmos.color = Color.blue;
                Gizmos.matrix = gameObject.transform.localToWorldMatrix * (Matrix4x4.Translate(new Vector3(radius, 0, 0)) * Matrix4x4.Translate(edges[i]));
                Gizmos.DrawSphere(new Vector3(0, 0, 0), radius * coeffs[2 * i + 1]);

                Gizmos.DrawIcon((gameObject.transform.localToWorldMatrix * Matrix4x4.Translate(edges[i])).MultiplyPoint(new Vector4(0, -2 * radius, 0)), "sound_icon_" + i + ".png", true);
            }

        }
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
    IEnumerator LoadAudio(string url, bool room, int n, bool isFromAssets)
    {
        AudioClip clip = null;
        

        if (isFromAssets)
        {   
            if (!room)
            {
                clip = audioClipMain[n];// Resources.Load< AudioClip>(url);
            }
            else
            {
                clip = audioClipBlend[n];
            }

            if(clip != null)
            { 
                clip.LoadAudioData();
            }
        }
        else
        {
            url = url.Replace("$CURDIR", "file:///" + Directory.GetCurrentDirectory());
            url = url.Replace("$STREAMINGASSETS", GetStreamingAssetsPath());

            //Debug.Log ("load audio : " + url);

            using (UnityWebRequest www = UnityWebRequestMultimedia.GetAudioClip(url, AudioType.UNKNOWN))
            {
                yield return www.SendWebRequest();

                if (www.isNetworkError)
                {
                    Debug.Log("WWW Error: " + www.error + " (" + url + ")");
                }
                else
                {
                    AudioClip myClip = DownloadHandlerAudioClip.GetContent(www);
                }
            }
        }

        if (clip != null)
        {
            if (!room)
            {
                dspPlayerMain.audioClips[0][n] = clip;
            }
            else
            {
                dspPlayerBlend.audioClips[0][n] = clip;
            }
        }

        yield break;
    }

    public bool IsReady()
    {
        if(isPlaying == false)
        {
            if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceEncodeObject)
            {
                bool isReady = true;
                for (int i = 0; i < m1EncodeObjects.Length; i++)
                {
                    if (!m1EncodeObjects[i].IsReady())
                    {
                        isReady = false;
                    }
                }

                if (isReady)
                {
                    for (int i = 0; i < dspPlayerEncode.countPlayers; i++)
                    {
                        dspPlayerEncode.audioClips[i] = new AudioClip[m1EncodeObjects[i].audioClips.Length];
                        for (int j = 0; j < m1EncodeObjects[i].audioClips.Length; j++)
                        {
                            dspPlayerEncode.audioClips[i][j] = m1EncodeObjects[i].audioClips[j];
                        }
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                bool isLoadedMain = true;
                for (int i = 0; i < dspPlayerMain.audioClips[0].Length; i++)
                {
                    if (dspPlayerMain.audioClips[0][i].loadState != AudioDataLoadState.Loaded)
                    {
                        isLoadedMain = false;
                        break;
                    }
                }

                bool isLoadedBlend = true;
                if (useBlendMode)
                {
                    for (int i = 0; i < dspPlayerMain.audioClips[0].Length; i++)
                    {
                        if (dspPlayerMain.audioClips[0][i].loadState != AudioDataLoadState.Loaded)
                        {
                            isLoadedBlend = false;
                            break;
                        }
                    }
                }
                return isLoadedMain && (useBlendMode ? isLoadedBlend : true);
            }
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
        if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceEncodeObject)
        {
            for (int i = 0; i < m1EncodeObjects.Length; i++)
            {
                m1EncodeObjects[i].StopAudio();
            }
        }
        else
        {
            if (isPlaying)
            {
                dspPlayerMain.Stop();

                if (useBlendMode)
                {
                    dspPlayerBlend.Stop();
                }
            }
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

    public bool IsPlaying()
    {
        if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceEncodeObject)
        {
            return dspPlayerEncode.isPlaying;
        }
        else
        { 
            return (dspPlayerMain.isPlaying || dspPlayerBlend.isPlaying);
        }
    }

    public static float ClosestPointOnBox(Vector3 point, Vector3 center, Vector3 axis0, Vector3 axis1, Vector3 axis2, Vector3 extents, out Vector3 closestPoint)
    {
        Vector3 vector = point - center;
        float num = 0f;

        float num0 = Vector3.Dot(vector, axis0);
        if (num0 < -extents.x)
        {
            num += Mathf.Pow(num0 + extents.x, 2);
            num0 = -extents.x;
        }
        else if (num0 > extents.x)
        {
            num += Mathf.Pow(num0 - extents.x, 2);
            num0 = extents.x;
        }

        float num1 = Vector3.Dot(vector, axis1);
        if (num1 < -extents.y)
        {
            num += Mathf.Pow(num1 + extents.y, 2);
            num1 = -extents.y;
        }
        else if (num1 > extents.y)
        {
            num += Mathf.Pow(num1 - extents.y, 2);
            num1 = extents.y;
        }

        float num2 = Vector3.Dot(vector, axis2);
        if (num2 < -extents.z)
        {
            num += Mathf.Pow(num2 + extents.z, 2);
            num2 = -extents.z;
        }
        else if (num2 > extents.z)
        {
            num += Mathf.Pow(num2 - extents.z, 2);
            num2 = extents.z;
        }
        closestPoint = center + num0 * axis0 + num1 * axis1 + num2 * axis2;

        return Mathf.Sqrt(num);
    }


    private static bool Clip(float denom, float numer, ref float t0, ref float t1)
    {
        if ((double)denom > 0.0)
        {
            if ((double)numer > (double)denom * (double)t1)
                return false;
            if ((double)numer > (double)denom * (double)t0)
                t0 = numer / denom;
            return true;
        }
        if ((double)denom >= 0.0)
            return (double)numer <= 0.0;
        if ((double)numer > (double)denom * (double)t0)
            return false;
        if ((double)numer > (double)denom * (double)t1)
            t1 = numer / denom;
        return true;
    }

    private static int DoClipping(float t0, float t1, Vector3 origin, Vector3 direction, Vector3 center, Vector3 axis0, Vector3 axis1, Vector3 axis2, Vector3 extents, bool solid, out Vector3 point0, out Vector3 point1)
    {
        Vector3 vector = origin - center;
        Vector3 vector2 = new Vector3(Vector3.Dot(vector, axis0), Vector3.Dot(vector, axis1), Vector3.Dot(vector, axis2));
        Vector3 vector3 = new Vector3(Vector3.Dot(direction, axis0), Vector3.Dot(direction, axis1), Vector3.Dot(direction, axis2));

        float num = t0;
        float num2 = t1;

        int quantity = 0;

        bool flag = Clip(vector3.x, -vector2.x - extents.x, ref t0, ref t1) && Clip(-vector3.x, vector2.x - extents.x, ref t0, ref t1) && Clip(vector3.y, -vector2.y - extents.y, ref t0, ref t1) && Clip(-vector3.y, vector2.y - extents.y, ref t0, ref t1) && Clip(vector3.z, -vector2.z - extents.z, ref t0, ref t1) && Clip(-vector3.z, vector2.z - extents.z, ref t0, ref t1);
        if (flag && (solid || t0 != num || t1 != num2))
        {
            if (t1 > t0)
            {
                quantity = 2;
                point0 = origin + t0 * direction;
                point1 = origin + t1 * direction;
            }
            else
            {
                quantity = 1;
                point0 = origin + t0 * direction;
                point1 = Vector3.zero;
            }
        }
        else
        {
            quantity = 0;
            point0 = Vector3.zero;
            point1 = Vector3.zero;
        }

        return quantity;
    }


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

    void OnDisable()
    {
        if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceEncodeObject)
        {
            dspPlayerEncode.OnDisable();
        }
        else
        { 
            dspPlayerMain.OnDisable();
            dspPlayerBlend.OnDisable();
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (audiolistener == null)
        {
            Debug.LogError("Mach1: cannot find AudioListener!");
            attachAudioListener();
            return;
        }


        if (IsReady())
        {
            if ((autoPlay || needToPlay) && !isPlaying)
            {
                if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceEncodeObject)
                {
                    dspPlayerEncode.Play();
                }
                else
                {
                    dspPlayerMain.Play();

                    if (useBlendMode)
                    {
                        dspPlayerBlend.Play();
                    }
                }

                needToPlay = false;
                isPlaying = true;
            }


            // In order to use values set in Unity's object inspector, we have to put them into an
            // M1 Positional library instance. Here's an example:
            // /*
            m1Positional.setUseBlendMode(useBlendMode);
            m1Positional.setIgnoreTopBottom(ignoreTopBottom);
            m1Positional.setMuteWhenOutsideObject(muteWhenOutsideObject);
            m1Positional.setMuteWhenInsideObject(muteWhenInsideObject);
            m1Positional.setUseAttenuation(useAttenuation);
            m1Positional.setUsePlaneCalculation(usePlaneCalculation);
            m1Positional.setUseYawForRotation(useYawForRotation);
            m1Positional.setUsePitchForRotation(usePitchForRotation);
            m1Positional.setUseRollForRotation(useRollForRotation);
            m1Positional.setListenerPosition(ConvertToMach1Point3D(audiolistener.transform.position));
            m1Positional.setListenerRotationQuat(ConvertToMach1Point4D(audiolistener.transform.rotation));
            m1Positional.setDecoderAlgoPosition(ConvertToMach1Point3D(gameObject.transform.position));
            m1Positional.setDecoderAlgoRotationQuat(ConvertToMach1Point4D(gameObject.transform.rotation));
            m1Positional.setDecoderAlgoScale(ConvertToMach1Point3D(gameObject.transform.lossyScale));
            m1Positional.evaluatePositionResults();

            if (useAttenuation)
            {
                m1Positional.setAttenuationCurve(attenuationCurve.Evaluate(m1Positional.getDist()));
                m1Positional.setAttenuationCurveBlendMode(attenuationCurveBlendMode.Evaluate(m1Positional.getDist()));
            }

            m1Positional.getCoefficients(ref coeffs);
            if (sourceType == M1BaseDecodeSourceType.M1BaseDecodeSourceEncodeObject)
            {
                for (int i = 0; i < 16; i++)
                {
                    dspPlayerEncode.coeffs[i] = coeffs[i];
                }
                for (int i = 0; i < m1EncodeObjects.Length; i++)
                {
                    dspPlayerEncode.gains[i] = m1EncodeObjects[i].gains;
                }
                dspPlayerEncode.Update();
            }
            else
            {
                for (int i = 0; i < 16; i++)
                {
                    dspPlayerMain.coeffs[i] = coeffs[i];
                }

                if (useBlendMode)
                {
                    m1Positional.getCoefficientsInterior(ref coeffsInterior);
                    for (int i = 0; i < 16; i++)
                    {
                        dspPlayerBlend.coeffs[i] = coeffsInterior[i];
                    }
                }
                dspPlayerMain.Update();
                dspPlayerBlend.Update();
            }

            if (debug)
            {
                // Compute rotation for sound
                Mach1.Mach1Point3D angles = m1Positional.getCoefficientsRotation();
                matInternal = Matrix4x4.TRS(audiolistener.transform.position, Quaternion.Euler(angles.x, angles.y, angles.z), new Vector3(1, 1, 1)) * Matrix4x4.Rotate(Quaternion.Inverse(audiolistener.transform.rotation));

                Debug.Log("M1Obj Euler Rotation Angles: " + m1Positional.getCoefficientsRotation().x + " , " + m1Positional.getCoefficientsRotation().y + " , " + m1Positional.getCoefficientsRotation().z);
                Debug.Log("M1Obj Distance: " + m1Positional.getDist());

                if (sourceType != M1BaseDecodeSourceType.M1BaseDecodeSourceEncodeObject)
                {
                    string str = "Returned Coefficients: ";
                    for (int i = 0; i < dspPlayerMain.coeffs.Length; i++)
                    {
                        str += string.Format("{0:0.000}, ", dspPlayerMain.coeffs[i]);
                    }
                    if (useBlendMode)
                    {
                        str += " , " + "Returned Coefficients Internal (BlendMode): ";
                        for (int i = 0; i < dspPlayerBlend.coeffs.Length; i++)
                        {
                            str += string.Format("{0:0.000}, ", dspPlayerBlend.coeffs[i]);
                        }
                    }
                    Debug.Log(str);
                }
            }

            // Mach1.Mach1Point3D angles = m1Positional.getCoefficientsRotation();
            //Debug.Log("volumeWalls: " + coeffs + " , " + "volumeRoom" + coeffsInterior);
            // Debug.Log("d: " + dist + ", d2: " + m1Positional.getDist());

            if (drawHelpers)
            {
                // Draw forward vector from audio listener
                Vector3 targetForward = audiolistener.transform.rotation * (Vector3.forward * 3);
                Debug.DrawLine(audiolistener.transform.position, audiolistener.transform.position + targetForward, Color.blue);

                // Draw direction from audio listener to object
                Debug.DrawLine(audiolistener.transform.position, gameObject.transform.position, Color.green);
            }
        }
    }
}