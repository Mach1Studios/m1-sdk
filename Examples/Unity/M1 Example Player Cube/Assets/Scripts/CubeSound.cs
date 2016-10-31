//Mach1
//2016
//
//Set each audioSource item calls script

using UnityEngine;
using System.Collections;
using System.IO;

public class CubeSound : MonoBehaviour
{
    public string audioPath = "file://";
    public string[] audioFilename;

    public bool useFalloff = true;
    public AnimationCurve curveFalloff; 

    private int loadedCount;
    private AudioSource[] audioSource;

    private const int MAX_SOUNDS_PER_CHANNEL = 8;

    CubeSound()
    {
        // Falloff
        Keyframe[] keyframes = new Keyframe[3];
        for (int i = 0; i < keyframes.Length; i++)
        {
            keyframes[i] = new Keyframe(i * 10, 1 - 1.0f * i / (keyframes.Length - 1));
        }

        curveFalloff = new AnimationCurve(keyframes);
        for (int i = 0; i < keyframes.Length; i++)
        {
            curveFalloff.SmoothTangents(i, 0);
        }

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
            StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename[i]), i));
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
        Gizmos.color = Color.gray;
        Gizmos.DrawIcon(transform.position, "sound_icon.png", true);

        Gizmos.matrix = gameObject.transform.localToWorldMatrix;
        Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));

        Gizmos.matrix = mat; // gameObject.transform.localToWorldMatrix; // mat;// 
        Gizmos.DrawWireCube(new Vector3(0, 0, 0), new Vector3(1, 1, 1));
    }

    // Load audio
    IEnumerator LoadAudio(string url, int n)
    {
        WWW www = new WWW(url);
        yield return www;

        if (www.error == null)
        {
            AudioClip clip = www.GetAudioClip(false, false);

            audioSource[n * 2] = AddAudio(clip, false, true, 1.0f);
            audioSource[n * 2].panStereo = -1;

            audioSource[n * 2 + 1] = AddAudio(clip, false, true, 1.0f);
            audioSource[n * 2 + 1].panStereo = 1;
            loadedCount++;
        }
        else
        {
            Debug.Log("WWW Error: " + www.error);
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


    Quaternion quat;
    Matrix4x4 mat;

    // Update is called once per frame
    void Update()
    {
        if (IsReady())
        {
            Quaternion quatCamera = Camera.main.transform.rotation;
            //quatCamera.eulerAngles = new Vector3(0, quatCamera.eulerAngles.y, quatCamera.eulerAngles.z);

            Vector3 dir = Camera.main.transform.position - gameObject.transform.position;
            //dir.y = 0;
            quat = Quaternion.Inverse(Quaternion.LookRotation(dir, Vector3.up)) * quatCamera * gameObject.transform.rotation;

            mat = Matrix4x4.TRS(Camera.main.transform.position,  Quaternion.LookRotation(dir, Vector3.up) * Quaternion.Inverse(gameObject.transform.rotation) , new Vector3(1, 1, 1));
            
            //Quaternion.RotateTowards(gameObject.transform.rotation, Camera.main.transform.rotation, 360);
            //quat = Camera.main.transform.rotation * quat;

            Vector3 eulerAngles = quat.eulerAngles;
            eulerAngles.x = eulerAngles.x > 180 ? 360 - eulerAngles.x : -eulerAngles.x;
			eulerAngles.y += 180;

            //Debug.Log("eulerAngles:" + eulerAngles);

            float volumeFalloff = useFalloff ? curveFalloff.Evaluate(Vector3.Distance(Camera.main.transform.position, gameObject.transform.position)) : 1;

            float[] volumes = M1DSPAlgorithms.eightChannelsAlgorithm(eulerAngles.x, eulerAngles.y, eulerAngles.z);
            for (int i = 0; i < volumes.Length; i++)
            {
                audioSource[i].volume = volumeFalloff * volumes[i];
            }
         

            // Draw forward vector from camera
            Vector3 targetForward = quatCamera * (Vector3.forward * 3);
            Debug.DrawLine(Camera.main.transform.position, Camera.main.transform.position + targetForward, Color.blue);

            // Draw direction from camera to object
            Debug.DrawLine(Camera.main.transform.position, gameObject.transform.position, Color.red);
        }


        /*
         LineRenderer.SetPosition(0, transform.position);
         LineRenderer.SetPosition(1, target.position); 
        */
 
    }

}
