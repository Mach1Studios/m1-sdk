//Mach1
//2016
//
//Set each audioSource item calls script

using UnityEngine;
using System.Collections;
using System.IO;

public class M1SpatialDecodeRoom : MonoBehaviour
{
    public string audioPath = "file:///";
    public bool isFromResource = true;
    public string[] audioFilename;

    [Space(10)]
    public bool useFalloff = false;
    public AnimationCurve curveFalloff;
    public AnimationCurve curveRoomModeFalloff;

    private int loadedCount;
    private AudioSource[] audioSource;

    private const int MAX_SOUNDS_PER_CHANNEL = 8;
    private Matrix4x4 mat;

    [Space(10)]
    public bool roomMode = false;
    public bool ignoreTopBottom = true;

    [Space(10)]
    public bool useClosestPoint = false;

    public bool useYawForClosestPoint = true;
    public bool usePitchForClosestPoint = true;
    public bool useRollForClosestPoint = false;

    [Space(10)]
    public bool drawHelpers = true;

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

    M1SpatialDecodeRoom()
    {
        // Falloff
        curveFalloff = generateCurve(10);
        curveRoomModeFalloff = generateCurve(1);

        // Init filenames
        audioFilename = new string[MAX_SOUNDS_PER_CHANNEL];
        for (int i = 0; i < audioFilename.Length; i++)
        {
            audioFilename[i] = (i + 1) + ".wav";
        }
    }

    void Awake()
    {
    }

    void Start()
    {
        // Sounds
        audioSource = new AudioSource[MAX_SOUNDS_PER_CHANNEL * 2];

        loadedCount = 0;

        for (int i = 0; i < audioFilename.Length; i++)
        {
            StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename[i]), i, isFromResource));
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

            Gizmos.matrix = gameObject.transform.localToWorldMatrix;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));

            Gizmos.matrix = mat;
            Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));
        }
    }

    // Load audio
    IEnumerator LoadAudio(string url, int n, bool isFromResource)
    {
        AudioClip clip = null;

        if (isFromResource)
        {
            clip = Resources.Load<AudioClip>(url);
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
            audioSource[n * 2] = AddAudio(clip, false, true, 1.0f);
            audioSource[n * 2].panStereo = -1;

            audioSource[n * 2 + 1] = AddAudio(clip, false, true, 1.0f);
            audioSource[n * 2 + 1].panStereo = 1;
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
            //audioSource[0].Play();
            //audioSource[1].Play();

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


    // Update is called once per frame
    void Update()
    {
        if (IsReady())
        {
            float volume = 1.0f;

            // Find closest point
            Vector3 point = gameObject.transform.position;

            Vector3 outsideClosestPoint;
            Vector3 insidePoint0, insidePoint1;

            Vector3 cameraPosition = Camera.main.transform.position;
            if(ignoreTopBottom)
            { 
                cameraPosition.y = gameObject.transform.position.y;
            }

            if (useClosestPoint && ClosestPointOnBox(Camera.main.transform.position, gameObject.transform.position, gameObject.transform.right, gameObject.transform.up, gameObject.transform.forward, gameObject.transform.localScale / 2, out outsideClosestPoint) > 0)
            {
                point = outsideClosestPoint;

                if (useFalloff)
                {
                    volume = volume * curveFalloff.Evaluate(Vector3.Distance(Camera.main.transform.position, point));
                }

                if (drawHelpers)
                {
                    Debug.DrawLine(gameObject.transform.position, point, Color.red);
                }
            }
            else if (roomMode && DoClipping(0, float.MaxValue, cameraPosition, (cameraPosition - gameObject.transform.position).normalized, gameObject.transform.position, gameObject.transform.right, gameObject.transform.up, gameObject.transform.forward, gameObject.transform.localScale / 2, true, out insidePoint0, out insidePoint1) == 2)
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
                    volume = volume * curveRoomModeFalloff.Evaluate(dist);
                }

                if (drawHelpers)
                {
                    Debug.Log("d: " + dist);
                    Debug.DrawLine(cameraPosition, p1, Color.cyan);
                    Debug.Log("volume: " + volume);
                }
            }
            else
            {
                volume = 0;
            }

            Vector3 dir = Camera.main.transform.position - point;

            // Compute matrix for draw gizmo
            Quaternion quatGizmo = Quaternion.LookRotation(dir, Vector3.up) * Quaternion.Inverse(gameObject.transform.rotation);
            quatGizmo.eulerAngles = new Vector3(usePitchForClosestPoint ? quatGizmo.eulerAngles.x : 0, useYawForClosestPoint ? quatGizmo.eulerAngles.y : 0, useRollForClosestPoint ? quatGizmo.eulerAngles.z : 0);
            mat = Matrix4x4.TRS(Camera.main.transform.position, quatGizmo, new Vector3(1, 1, 1));

            // Compute rotation for sound
            Quaternion quat = Quaternion.Inverse(Quaternion.LookRotation(dir, Vector3.up)) * gameObject.transform.rotation;
            quat.eulerAngles = new Vector3(usePitchForClosestPoint ? quat.eulerAngles.x : 0, useYawForClosestPoint ? quat.eulerAngles.y : 0, useRollForClosestPoint ? quat.eulerAngles.z : 0);
            quat *= Camera.main.transform.rotation;

            // Compute volumes
            Vector3 eulerAngles = quat.eulerAngles;
            eulerAngles.x = eulerAngles.x > 180 ? 360 - eulerAngles.x : -eulerAngles.x;
            eulerAngles.y += 180;
            //Debug.Log("eulerAngles:" + eulerAngles);

            float[] volumes = M1DSPAlgorithms.eightChannelsAlgorithm(eulerAngles.x, eulerAngles.y, eulerAngles.z);
            for (int i = 0; i < volumes.Length; i++)
            {
                audioSource[i].volume = volume * volumes[i];
            }

            if (drawHelpers)
            {
                // Draw forward vector from camera
                Vector3 targetForward = Camera.main.transform.rotation * (Vector3.forward * 3);
                Debug.DrawLine(Camera.main.transform.position, Camera.main.transform.position + targetForward, Color.blue);

                // Draw direction from camera to object
                Debug.DrawLine(Camera.main.transform.position, point, Color.green);
            }
        }

    }

}
