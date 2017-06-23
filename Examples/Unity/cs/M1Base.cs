//Mach1
//2016
//
//Set each audioSourceWalls item calls script

using UnityEngine;
using UnityEngine.Audio;
using System.Collections;
using System.IO;

public class M1Base : MonoBehaviour
{
    public bool isFromAssets = true;
    public AudioMixerGroup audioMixerGroup;

    public AudioClip[] audioClipWalls;
    public AudioClip[] audioClipRoom;
    public string audioPath = "file:///";
    public string[] audioFilenameWalls;
    public string[] audioFilenameRoom;

    [Space(10)]
    public bool autoPlay;
    public bool isLoop;
    private bool isPlaying;

    [Space(10)]
    public bool useFalloff = false;
    public AnimationCurve curveFalloff;
    public AnimationCurve curveRoomModeFalloff;

    private int loadedCountWalls;
    private AudioSource[] audioSourceWalls;

    private int loadedCountRoom;
    private AudioSource[] audioSourceRoom;

    private int MAX_SOUNDS_PER_CHANNEL;
    private Matrix4x4 mat;

    [Space(10)]
    public bool useRoomMode = false;
    public bool ignoreTopBottom = true;

    [Space(10)]
    public bool useRotator = false;
    public bool useClosestPoint = true;

    public bool useYawForClosestPoint = true;
    public bool usePitchForClosestPoint = true;
    public bool useRollForClosestPoint = false;

    [Space(10)]
    public bool drawHelpers = true;

    protected M1DSPAlgorithms m1DSPAlgorithms = new M1DSPAlgorithms();

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

    protected void InitComponents(int MAX_SOUNDS_PER_CHANNEL)
    {
        this.MAX_SOUNDS_PER_CHANNEL = MAX_SOUNDS_PER_CHANNEL;

        // Falloff
        curveFalloff = generateCurve(10);
        curveRoomModeFalloff = generateCurve(1);

        // Init filenames
        audioFilenameWalls = new string[MAX_SOUNDS_PER_CHANNEL];
        audioFilenameRoom = new string[MAX_SOUNDS_PER_CHANNEL];
        for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
        {
            audioFilenameWalls[i] = (i + 1) + ".wav";
            audioFilenameRoom[i] = (i + 1) + ".wav";
        }

        // audioClip
        audioClipWalls = new AudioClip[MAX_SOUNDS_PER_CHANNEL];
        audioClipRoom = new AudioClip[MAX_SOUNDS_PER_CHANNEL];
    }

    public virtual float[] SoundAlgorithm(float Yaw, float Pitch, float Roll)
    {
        return new float[0];
    }

    void Awake()
    {
    }

    void Start()
    {
        // Sounds
        audioSourceWalls = new AudioSource[MAX_SOUNDS_PER_CHANNEL * 2];

        loadedCountWalls = 0;

        for (int i = 0; i < audioFilenameWalls.Length; i++)
        {
            StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilenameWalls[i]), false, i, isFromAssets));
        }

        if (useRoomMode)
        {
            // Sounds
            audioSourceRoom = new AudioSource[MAX_SOUNDS_PER_CHANNEL * 2];

            loadedCountRoom = 0;

            for (int i = 0; i < audioFilenameRoom.Length; i++)
            {
                StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilenameRoom[i]), true, i, isFromAssets));
            }

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
        source.priority = 0;
        source.spatialize = false;
        source.outputAudioMixerGroup = audioMixerGroup;
        return source;
    }

    // Draw gizmo in editor (you may display this also in game windows if set "Gizmo" button)
    void OnDrawGizmos()
    {
        if (drawHelpers)
        {
            Gizmos.color = Color.gray;
            Gizmos.DrawIcon(transform.position, "sound_icon.png", true);

            Gizmos.matrix = gameObject.transform.localToWorldMatrix;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));

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
     path = "file:" + Application.dataPath + "/Raw";
#else
     //Desktop (Mac OS or Windows)
     path = "file:"+ Application.dataPath + "/StreamingAssets";
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
                clip = audioClipWalls[n];// Resources.Load< AudioClip>(url);
            }
            else
            {
                clip = audioClipRoom[n];
            }
        }
        else
        {
            url = url.Replace("$CURDIR", "file:///" + Directory.GetCurrentDirectory());
            url = url.Replace("$STREAMINGASSETS", GetStreamingAssetsPath());

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
            if (!room)
            {
                audioSourceWalls[n * 2] = AddAudio(clip, isLoop, true, 1.0f);
                audioSourceWalls[n * 2].panStereo = -1;

                audioSourceWalls[n * 2 + 1] = AddAudio(clip, isLoop, true, 1.0f);
                audioSourceWalls[n * 2 + 1].panStereo = 1;
                loadedCountWalls++;
            }
            else
            {
                audioSourceRoom[n * 2] = AddAudio(clip, isLoop, true, 1.0f);
                audioSourceRoom[n * 2].panStereo = -1;

                audioSourceRoom[n * 2 + 1] = AddAudio(clip, isLoop, true, 1.0f);
                audioSourceRoom[n * 2 + 1].panStereo = 1;
                loadedCountRoom++;

            }
        }

        yield break;
    }

    public bool IsReady()
    {
        return loadedCountWalls == MAX_SOUNDS_PER_CHANNEL && (useRoomMode ? loadedCountRoom == MAX_SOUNDS_PER_CHANNEL : true);
    }

    public void PlayAudio()
    {
        if (IsReady())
        {
            //audioSourceWalls[0].Play();
            //audioSourceWalls[1].Play();

            for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
            {
                audioSourceWalls[i].Play();
            }

            if (useRoomMode)
            {
                for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
                {
                    audioSourceRoom[i].Play();
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
            for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
            {
                audioSourceWalls[i].Stop();
            }

            if (useRoomMode)
            {
                for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
                {
                    audioSourceRoom[i].Stop();
                }
            }
        }
    }

    public void Seek(float timeInSeconds)
    {
        if (audioSourceRoom != null)
            foreach (AudioSource source in audioSourceRoom)
                if (source != null)
                    source.time = timeInSeconds;

        if (audioSourceWalls != null)
            foreach (AudioSource source in audioSourceWalls)
                if (source != null)
                    source.time = timeInSeconds;
    }

    public float GetPositon()
    {
        if (audioSourceRoom != null && audioSourceRoom[0]) return audioSourceRoom[0].time;
        else if (audioSourceWalls != null && audioSourceWalls[0]) return audioSourceWalls[0].time;
        return 0;
    }

    public bool IsPlaying()
    {
        return (audioSourceRoom != null && audioSourceRoom[0].isPlaying) || (audioSourceWalls != null && audioSourceWalls[0].isPlaying);
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

            float volumeWalls = 1.0f;
            float volumeRoom = 0.0f;

            // Find closest point
            Vector3 point = gameObject.transform.position;

            Vector3 outsideClosestPoint;
            //Vector3 insidePoint0, insidePoint1;

            Vector3 cameraPosition = Camera.current.transform.position;
            if (ignoreTopBottom)
            {
                cameraPosition.y = gameObject.transform.position.y;
            }

            bool isOutside = (ClosestPointOnBox(Camera.current.transform.position, gameObject.transform.position, gameObject.transform.right, gameObject.transform.up, gameObject.transform.forward, gameObject.transform.localScale / 2, out outsideClosestPoint) > 0);
            if (useClosestPoint && isOutside)
            {
                point = outsideClosestPoint;

                float dist = Vector3.Distance(Camera.current.transform.position, point);

                if (useFalloff)
                {
                    volumeWalls = volumeWalls * curveFalloff.Evaluate(dist);
                }

                if (drawHelpers)
                {
                    Debug.DrawLine(gameObject.transform.position, point, Color.red);
                }
            }
            else if (useRoomMode && !isOutside)//   && DoClipping(0, float.MaxValue, cameraPosition, (cameraPosition - gameObject.transform.position).normalized, gameObject.transform.position, gameObject.transform.right, gameObject.transform.up, gameObject.transform.forward, gameObject.transform.localScale / 2, true, out insidePoint0, out insidePoint1) == 2)
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

                float dist = 1 - Mathf.Max(Mathf.Abs(p0.x), Mathf.Max(Mathf.Abs(p0.y), Mathf.Abs(p0.z)));

                if (useFalloff)
                {
                    volumeWalls = volumeWalls * curveRoomModeFalloff.Evaluate(dist);
                }

                volumeRoom = 1 - volumeWalls;

                if (drawHelpers)
                {
                    Debug.Log("d: " + dist);
                    Debug.DrawLine(cameraPosition, p1, Color.cyan);
                    Debug.Log("volumeWalls: " + volumeWalls);
                }
            }
            else if (useRotator)
            {
                float dist = Vector3.Distance(Camera.current.transform.position, point);

                if (useFalloff)
                {
                    volumeWalls = volumeWalls * curveRoomModeFalloff.Evaluate(dist);
                }
            }
            else
            {
                volumeWalls = 0;
                volumeRoom = 0;
            }


            Vector3 dir = Camera.current.transform.position - point;

            // Compute matrix for draw gizmo
            Quaternion quatGizmo = Quaternion.LookRotation(dir, Vector3.up) * Quaternion.Inverse(gameObject.transform.rotation);
            quatGizmo.eulerAngles = new Vector3(usePitchForClosestPoint ? quatGizmo.eulerAngles.x : 0, useYawForClosestPoint ? quatGizmo.eulerAngles.y : 0, useRollForClosestPoint ? quatGizmo.eulerAngles.z : 0);
            mat = Matrix4x4.TRS(Camera.current.transform.position, quatGizmo, new Vector3(1, 1, 1));

            // Compute rotation for sound
            Quaternion quat = Quaternion.Inverse(Quaternion.LookRotation(dir, Vector3.up)) * gameObject.transform.rotation;
            quat.eulerAngles = new Vector3(usePitchForClosestPoint ? quat.eulerAngles.x : 0, useYawForClosestPoint ? quat.eulerAngles.y : 0, useRollForClosestPoint ? quat.eulerAngles.z : 0);
            quat *= Camera.current.transform.rotation;

            // Compute volumes
            //Vector3 eulerAngles = quat.eulerAngles;
            //eulerAngles.x = eulerAngles.x > 180 ? 360 - eulerAngles.x : -eulerAngles.x;
            //eulerAngles.y += 180;

            //eulerAngles = Quaternion.Euler(eulerAngles).eulerAngles;

            // Debug.Log(Camera.current.name + " camera eulerAngles:" + eulerAngles);

            Vector3 eulerAngles = GetEuler(quat);
//            eulerAngles.x *= -1;
            eulerAngles.y += 180;
            if (eulerAngles.z < 0) eulerAngles.z = 360 + eulerAngles.z;

            float[] volumes = SoundAlgorithm(eulerAngles.x, eulerAngles.y, eulerAngles.z);
            for (int i = 0; i < audioSourceWalls.Length; i++)
            {
                audioSourceWalls[i].volume = volumeWalls * volumes[i];
            }
            if (useRoomMode)
            {
                for (int i = 0; i < audioSourceRoom.Length; i++)
                {
                    audioSourceRoom[i].volume = volumeRoom * volumes[i];
                }
            }

            if (drawHelpers)
            {
                // Draw forward vector from camera
                Vector3 targetForward = Camera.current.transform.rotation * (Vector3.forward * 3);
                Debug.DrawLine(Camera.current.transform.position, Camera.current.transform.position + targetForward, Color.blue);

                // Draw direction from camera to object
                Debug.DrawLine(Camera.current.transform.position, point, Color.green);
            }
        }

    }

}
