//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//


using UnityEngine;
using UnityEngine.Audio;
using System.Collections;
using System.IO;

public class M1Base : MonoBehaviour
{
    public AudioMixerGroup m1SpatialAudioMixerGroup;

    public AudioClip[] audioClipMain;
    public AudioClip[] audioClipBlend;

    [Space(10)]
    public string externalAudioPath = "file:///";
    public bool isFromStreamingAssets = true;
    public string[] externalAudioFilenameMain;
    public string[] externalAudioFilenameBlend;

    [Space(10)]
    public bool autoPlay;
    public bool isLoop;
    private bool isPlaying;

    [Space(10)]
    public bool useFalloff = false;
    public AnimationCurve falloffCurve;
    public AnimationCurve blendModeFalloffCurve;

    private int loadedCountMain;
    private AudioSource[] audioSourceMain;

    private int loadedCountBlend;
    private AudioSource[] audioSourceBlend;

    private int MAX_SOUNDS_PER_CHANNEL;
    private Matrix4x4 mat;

    [Space(10)]
    public bool useBlendMode = false;
    public bool ignoreTopBottom = true;

    [Space(10)]
    public bool muteWhenInsideObject = false;
    public bool muteWhenOutsideObject = false;
    public bool useClosestPointRotationMuteInside = false;

    [Space(10)]
    public bool useYawForRotation = true;
    public bool usePitchForRotation = true;
    public bool useRollForRotation = true;

    [Space(10)]
    public bool drawHelpers = false;
    public bool debug = false;

    protected Mach1.Mach1Decode m1Decode = new Mach1.Mach1Decode();
    protected Mach1.Mach1DecodePositional m1Positional = new Mach1.Mach1DecodePositional();

    private Camera camera;

    static Mach1.Mach1Point3D ConvertToMach1Point3D(Vector3 vec)
    {
        return new Mach1.Mach1Point3D(vec.x, vec.y, vec.z);
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

    public M1Base()
    {
        m1Decode.setPlatformType(Mach1.Mach1PlatformType.Mach1PlatformUnity);
        m1Positional.setPlatformType(Mach1.Mach1PlatformType.Mach1PlatformUnity);
    }

    protected void InitComponents(int MAX_SOUNDS_PER_CHANNEL)
    {
        this.MAX_SOUNDS_PER_CHANNEL = MAX_SOUNDS_PER_CHANNEL;

        // Falloff
        falloffCurve = generateCurve(10);
        blendModeFalloffCurve = generateCurve(1);

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

    public virtual float[] SoundAlgorithm(float Yaw, float Pitch, float Roll)
    {
        float[] data = new float[18];
        m1Decode.decode(Yaw, Pitch, Roll, ref data);
        return data;
    }

    void Awake()
    {
    }

    void Start()
    {
        Reload();
    }

    public void Reload()
    {
        // Sounds
        audioSourceMain = new AudioSource[MAX_SOUNDS_PER_CHANNEL * 2];

        loadedCountMain = 0;

        for (int i = 0; i < externalAudioFilenameMain.Length; i++)
        {
            StartCoroutine(LoadAudio(Path.Combine(externalAudioPath, externalAudioFilenameMain[i]), false, i, isFromStreamingAssets));
        }

        if (useBlendMode)
        {
            // Sounds
            audioSourceBlend = new AudioSource[MAX_SOUNDS_PER_CHANNEL * 2];

            loadedCountBlend = 0;

            for (int i = 0; i < externalAudioFilenameBlend.Length; i++)
            {
				StartCoroutine(LoadAudio(Path.Combine(externalAudioPath, externalAudioFilenameBlend[i]), true, i, isFromStreamingAssets));
            }
        }

        isPlaying = false;
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
        source.priority = 0;
        source.spatialize = false;
        source.outputAudioMixerGroup = m1SpatialAudioMixerGroup;
        return source;
    }

    Quaternion quatInternal;
    Transform t;

    // Draw gizmo in editor (you may display this also in game windows if set "Gizmo" button)
    void OnDrawGizmos()
    {
        if (t == null) t = new GameObject().transform;

        if (drawHelpers)
        {
            Gizmos.color = Color.gray;
            Gizmos.DrawIcon(transform.position, "sound_icon.png", true);

            Gizmos.matrix = gameObject.transform.localToWorldMatrix;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));

             t.position = camera.transform.position;
            t.rotation = quatInternal;
            Gizmos.matrix = t.localToWorldMatrix;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(2, 1, 0.5f));

            Gizmos.matrix = mat;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));
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
		    if (!room && audioSourceMain != null && audioSourceMain.Length > n * 2 + 1)
            {
                audioSourceMain[n * 2] = AddAudio(clip, isLoop, true, 1.0f);
                audioSourceMain[n * 2].panStereo = -1;

                audioSourceMain[n * 2 + 1] = AddAudio(clip, isLoop, true, 1.0f);
                audioSourceMain[n * 2 + 1].panStereo = 1;
                loadedCountMain++;
            }
		    else if(audioSourceBlend != null && audioSourceBlend.Length > n * 2 + 1)
            {
                audioSourceBlend[n * 2] = AddAudio(clip, isLoop, true, 1.0f);
                audioSourceBlend[n * 2].panStereo = -1;

                audioSourceBlend[n * 2 + 1] = AddAudio(clip, isLoop, true, 1.0f);
                audioSourceBlend[n * 2 + 1].panStereo = 1;
                loadedCountBlend++;

            }
        }

        yield break;
    }

    public bool IsReady()
    {
        return loadedCountMain == MAX_SOUNDS_PER_CHANNEL && (useBlendMode ? loadedCountBlend == MAX_SOUNDS_PER_CHANNEL : true);
    }

    public void PlayAudio()
    {
        if (IsReady())
        {
            //audioSourceWalls[0].Play();
            //audioSourceWalls[1].Play();

            for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
            {
                audioSourceMain[i].Play();
            }

            if (useBlendMode)
            {
                for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
                {
                    audioSourceBlend[i].Play();
                }
            }
        }
        else
        {
            Debug.LogError("Audio was not loaded");
        }
    }

    public void StopAudio()
    {
        if (IsReady())
        {
			if (audioSourceMain != null) {
				for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++) {
					audioSourceMain [i].Stop ();
				}
			}

			if (useBlendMode && audioSourceBlend != null)
            {
                for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
                {
                    audioSourceBlend[i].Stop();
                }
            }
        }
    }

    public void Seek(float timeInSeconds)
    {
        if (audioSourceBlend != null)
            foreach (AudioSource source in audioSourceBlend)
                if (source != null)
                    source.time = timeInSeconds;

        if (audioSourceMain != null)
            foreach (AudioSource source in audioSourceMain)
                if (source != null)
                    source.time = timeInSeconds;
    }

    public float GetPosition()
    {
		if (audioSourceBlend != null && audioSourceBlend.Length > 0) return audioSourceBlend[0].time;
		else if (audioSourceMain != null && audioSourceMain.Length > 0) return audioSourceMain[0].time;
        return 0;
    }

    public float GetDuration()
    {
		if (audioSourceBlend != null && audioSourceBlend.Length > 0) return audioSourceBlend[0].clip.length;
		else if (audioSourceMain != null && audioSourceMain.Length > 0) return audioSourceMain[0].clip.length;
        return 0;
    }

    public bool IsPlaying()
    {
		return (audioSourceBlend != null && audioSourceBlend.Length > 0 && audioSourceBlend[0].isPlaying) || (audioSourceMain != null && audioSourceMain[0].isPlaying);
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

            // Find closest point
            Vector3 point = gameObject.transform.position;

            Vector3 outsideClosestPoint;
            //Vector3 insidePoint0, insidePoint1;


            if (Camera.main)
            {
                camera = Camera.main;
            }
            else
            {
                for (int i = 0; i < Camera.allCamerasCount; i++)
                {
                    if (Camera.allCameras[i].enabled)
                    {
                        camera = Camera.allCameras[i];
                        break;
                    }
                }
            }

            if (camera == null)
            {
                Debug.LogError("Mach1: cannot found camera!");
                return;
            }

            //            /*
            //---------------------------------------------------------------------------------
            //---------------------------------------------------------------------------------
            //---------------------------------------------------------------------------------
            //---------------------------------------------------------------------------------

            float volumeWalls = 1.0f;
            float volumeRoom = 0.0f;


            Vector3 cameraPosition = camera.transform.position;
            if (ignoreTopBottom)
            {
                cameraPosition.y = gameObject.transform.position.y;
            }

            bool isOutside = (ClosestPointOnBox(camera.transform.position, gameObject.transform.position, gameObject.transform.right, gameObject.transform.up, gameObject.transform.forward, gameObject.transform.localScale / 2, out outsideClosestPoint) > 0);
            bool hasSoundOutside = isOutside && !muteWhenOutsideObject;
            bool hasSoundInside = !isOutside && !muteWhenInsideObject;

            float dist = 0;

            if (hasSoundOutside && useClosestPointRotationMuteInside) // useClosestPointRotation
            {
                point = outsideClosestPoint;

                dist = Vector3.Distance(camera.transform.position, point);

                if (useFalloff)
                {
                    volumeWalls = volumeWalls * falloffCurve.Evaluate(dist);
                }

                if (drawHelpers)
                {
                    Debug.DrawLine(gameObject.transform.position, point, Color.red);
                }
            }
            else if (hasSoundInside && useBlendMode) // && DoClipping(0, float.MaxValue, cameraPosition, (cameraPosition - gameObject.transform.position).normalized, gameObject.transform.position, gameObject.transform.right, gameObject.transform.up, gameObject.transform.forward, gameObject.transform.localScale / 2, true, out insidePoint0, out insidePoint1) == 2)
            {
                Vector3 p0 = 2 * gameObject.transform.InverseTransformPoint(cameraPosition);

                Vector3 p1 = p0;
                if (Mathf.Abs(p0.x) > Mathf.Abs(p0.y) && Mathf.Abs(p0.x) > Mathf.Abs(p0.z))
                {
                    p1.x = p0.x > 0 ? 1 : -1;
                }
                if (Mathf.Abs(p0.y) > Mathf.Abs(p0.x) && Mathf.Abs(p0.y) > Mathf.Abs(p0.z))
                {
                    p1.y = p0.y > 0 ? 1 : -1;
                }
                if (Mathf.Abs(p0.z) > Mathf.Abs(p0.x) && Mathf.Abs(p0.z) > Mathf.Abs(p0.y))
                {
                    p1.z = p0.z > 0 ? 1 : -1;
                }
                p1 = gameObject.transform.TransformPoint(p1 / 2);

                dist = 1 - Mathf.Max(Mathf.Abs(p0.x), Mathf.Max(Mathf.Abs(p0.y), Mathf.Abs(p0.z)));

                if (useFalloff)
                {
                    volumeWalls = volumeWalls * blendModeFalloffCurve.Evaluate(dist);
                }

                volumeRoom = 1 - volumeWalls;

                if (drawHelpers)
                {
                    //Debug.Log("d: " + dist);
                    Debug.DrawLine(cameraPosition, p1, Color.cyan);
                    //Debug.Log("volumeWalls: " + volumeWalls);
                }
            }
            else if (hasSoundOutside || hasSoundInside) // useCenterPointRotation
            {
                dist = Vector3.Distance(camera.transform.position, point);

                if (useFalloff)
                {
                    if (hasSoundOutside)
                    {
                        volumeWalls = volumeWalls * falloffCurve.Evaluate(dist);
                    }
                    if (useBlendMode)
                    {
                        volumeWalls = volumeWalls * blendModeFalloffCurve.Evaluate(dist);
                    }
                }
            }
            else
            {
                volumeWalls = 0;
                volumeRoom = 0;
            }

     //   Vector3 dir = camera.transform.position - point;
            Vector3 dir = (point - camera.transform.position).normalized;  // !!!!


            //Quaternion.FromToRotation(new Vector3(165, 590, 100), new Vector3(-160, -10, 100));
            /*
            Debug.Log(">   1 " + ToStringFormat(Quaternion.LookRotation((new Vector3(165, 590, 100) - new Vector3(-160, -10, 100)).normalized, new Vector3(0, 0, 1)).eulerAngles * Mathf.Deg2Rad));
            Debug.Log(">   2 " + ToStringFormat(Quaternion.LookRotation((-new Vector3(165, 590, 100) + new Vector3(-160, -10, 100)).normalized, new Vector3(0, 0, 1)).eulerAngles * Mathf.Deg2Rad));
            Debug.Log(">   3 " + ToStringFormat(GetEuler(Quaternion.LookRotation((new Vector3(165, 590, 100) - new Vector3(-160, -10, 100)).normalized, new Vector3(0, 0, 1))) * Mathf.Deg2Rad));
            */


            //Debug.Log(">> ok " + ToStringFormat(GetEuler(Quaternion.LookRotation((new Vector3(-160, -10, 100) - new Vector3(165, 590, 100)).normalized, new Vector3(0, 0, 1))) * Mathf.Deg2Rad));
            

            //Debug.Log(">> ok2 " + ToStringFormat( (Quaternion.LookRotation((new Vector3(-160, -10, 100) - new Vector3(165, 590, 100)).normalized, new Vector3(0, 0, 1)).eulerAngles) * Mathf.Deg2Rad));
            // point - camera
            //...

           // Quaternion.LookRotation((point - camera.transform.position).normalized, Vector3.up) *  //

            // Compute matrix for draw gizmo
            Quaternion quatGizmo = Quaternion.LookRotation(dir, Vector3.up) * Quaternion.Inverse(gameObject.transform.rotation);
            quatGizmo.eulerAngles = new Vector3(usePitchForRotation ? quatGizmo.eulerAngles.x : 0, useYawForRotation ? quatGizmo.eulerAngles.y : 0, useRollForRotation ? quatGizmo.eulerAngles.z : 0);
            mat = Matrix4x4.TRS(camera.transform.position, quatGizmo, new Vector3(1, 1, 1));

            // Compute rotation for sound
            Quaternion quat = Quaternion.Inverse(Quaternion.LookRotation(dir, Vector3.up)) * gameObject.transform.rotation;
            quat.eulerAngles = new Vector3(usePitchForRotation ? quat.eulerAngles.x : 0, useYawForRotation ? quat.eulerAngles.y : 0, useRollForRotation ? quat.eulerAngles.z : 0);

            // test only
            quatInternal = quat;


            quat *= camera.transform.rotation;

             //gameObject.transform.rotation = quat;

            // Compute volumes
            //Vector3 eulerAngles = quat.eulerAngles;
            //eulerAngles.x = eulerAngles.x > 180 ? 360 - eulerAngles.x : -eulerAngles.x;
            //eulerAngles.y += 180;

            //eulerAngles = Quaternion.Euler(eulerAngles).eulerAngles;

            // Debug.Log(Camera.current.name + " camera eulerAngles:" + eulerAngles);

            Vector3 eulerAngles = GetEuler(quat);
            //            eulerAngles.x *= -1;
          //  eulerAngles.y += 180;
          //  if (eulerAngles.z < 0) eulerAngles.z = 360 + eulerAngles.z;

            float[] volumes = SoundAlgorithm(eulerAngles.x, eulerAngles.y, eulerAngles.z);
            for (int i = 0; i < audioSourceMain.Length; i++)
            {
                audioSourceMain[i].volume = volumeWalls * volumes[i];
            }
            if (useBlendMode)
            {
                for (int i = 0; i < audioSourceBlend.Length; i++)
                {
                    audioSourceBlend[i].volume = volumeRoom * volumes[i];
                }
            }

            if (debug)
            {
                if (audioSourceMain.Length > 0)
                {
                    string str = " ";
                    for (int i = 0; i < audioSourceMain.Length; i++)
                    {
                        str += volumeWalls * volumes[i] + (i < audioSourceMain.Length - 1 ? " , " : "");
                    }
                    Debug.Log("audioSourceWalls: " + str);
                }
                if (useBlendMode)
                {
                    string str = " ";
                    for (int i = 0; i < audioSourceBlend.Length; i++)
                    {
                        str += volumeRoom * volumes[i] + (i < audioSourceBlend.Length - 1 ? " , " : "");
                    }
                    Debug.Log("audioSourceWalls: " + str);
                }

                Debug.Log("eulerAngles: " + eulerAngles.x + " , " + eulerAngles.y + " , " + eulerAngles.z);
            }

            //Debug.Log("eulerAngles _ : " + eulerAngles.x + " , " + eulerAngles.y + " , " + eulerAngles.z);

            //*/
            //---------------------------------------------------------------------------------
            //---------------------------------------------------------------------------------
            //---------------------------------------------------------------------------------


             

            // In order to use values set in Unity's object inspector, we have to put them into an
            // M1 Positional library instance. Here's an example:
            /*
            m1Positional.setUseBlendMode(useBlendMode);
            m1Positional.setIgnoreTopBottom(ignoreTopBottom);
            m1Positional.setMuteWhenOutsideObject(muteWhenOutsideObject);
            m1Positional.setMuteWhenInsideObject(muteWhenInsideObject);
            m1Positional.setUseFalloff(useFalloff);
            m1Positional.setUseClosestPointRotationMuteInside(useClosestPointRotationMuteInside);
            m1Positional.setUseYawForRotation(useYawForRotation);
            m1Positional.setUsePitchForRotation(usePitchForRotation);
            m1Positional.setUseRollForRotation(useRollForRotation);


            m1Positional.setCameraPosition(ConvertToMach1Point3D(camera.transform.position));
            m1Positional.setCameraRotation(ConvertToMach1Point3D(camera.transform.rotation.eulerAngles));
            m1Positional.setDecoderAlgoPosition(ConvertToMach1Point3D(gameObject.transform.position));
            m1Positional.setDecoderAlgoRotation(ConvertToMach1Point3D(gameObject.transform.rotation.eulerAngles));
            m1Positional.setDecoderAlgoScale(ConvertToMach1Point3D(gameObject.transform.lossyScale));
            m1Positional.evaluatePostionResults();

            if (useFalloff)
            {
                m1Positional.setFalloffCurve(falloffCurve.Evaluate(m1Positional.getDist()));
                m1Positional.setFalloffCurveBlendMode(blendModeFalloffCurve.Evaluate(m1Positional.getDist()));
            }
          
            float[] volumesWalls = new float[18];
            m1Positional.getVolumesWalls(ref volumesWalls);
            for (int i = 0; i < audioSourceMain.Length; i++)
            {
                audioSourceMain[i].volume = volumesWalls[i];
            }

            if (useBlendMode)
            {
                float[] volumesRoom = new float[18];
                m1Positional.getVolumesRoom(ref volumesRoom);
                for (int i = 0; i < audioSourceBlend.Length; i++)
                {
                    audioSourceBlend[i].volume = volumesRoom[i];
                }
            }
            */

            //  Debug.Log("volumeWalls: " + volumeWalls  + " , " + "volumeRoom" + volumeRoom);
            // Debug.Log("d: " + dist + ", d2: " + m1Positional.getDist());

            if (drawHelpers)
            {
                // Draw forward vector from camera
                Vector3 targetForward = camera.transform.rotation * (Vector3.forward * 3);
                Debug.DrawLine(camera.transform.position, camera.transform.position + targetForward, Color.blue);

                // Draw direction from camera to object
                Debug.DrawLine(camera.transform.position, point, Color.green);
            }
        }

    }

}
