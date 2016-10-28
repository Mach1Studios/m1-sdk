//Mach1
//2016
//
//Set each audioSource item calls script

using UnityEngine;
using System.Collections;
using System.IO;

public class CubeSound : MonoBehaviour
{
    public string audioPath = "file:///C:/Projects/Dylan/Observatory%20Ambience%201/Observatory%20Ambience%201/";
    public string audioFilename01 = "Observatory_AMB_1_T000.wav";
    public string audioFilename02 = "Observatory_AMB_1_T090.wav";
    public string audioFilename03 = "Observatory_AMB_1_T180.wav";
    public string audioFilename04 = "Observatory_AMB_1_T270.wav";
    public string audioFilename05 = "Observatory_AMB_1_B000.wav";
    public string audioFilename06 = "Observatory_AMB_1_B090.wav";
    public string audioFilename07 = "Observatory_AMB_1_B180.wav";
    public string audioFilename08 = "Observatory_AMB_1_B270.wav";

    private int loadedCount;

    private AudioSource[] audioSource;

    private const int MAX_SOUNDS_PER_CHANNEL = 8;

    void Awake()
    {

    }

    void Start()
    {
        audioSource = new AudioSource[MAX_SOUNDS_PER_CHANNEL * 2];

        loadedCount = 0;

        StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename01), 0));
        StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename02), 1));
        StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename03), 2));
        StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename04), 3));
        StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename05), 4));
        StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename06), 5));
        StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename07), 6));
        StartCoroutine(LoadAudio(Path.Combine(audioPath, audioFilename08), 7));
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

    // Update is called once per frame
    void Update()
    {
        if (IsReady())
        {
            Vector3 eulerAngles = Camera.main.transform.rotation.eulerAngles;

            eulerAngles.x = eulerAngles.x > 180 ? 360 - eulerAngles.x : -eulerAngles.x;

            Debug.Log("eulerAngles:" + eulerAngles);

            float[] volumes = M1DSPAlgorithms.eightChannelsAlgorithm(eulerAngles.x, eulerAngles.y, eulerAngles.z);
            for (int i = 0; i < volumes.Length; i++)
            {
                audioSource[i].volume = volumes[i];
            }
        }

        //Debug.DrawLine(new Vector3(0, 0, 0), new Vector3(2, 1, 10), Color.red);
        /*
         LineRenderer.SetPosition(0, transform.position);
LineRenderer.SetPosition(1, target.position); 
*/







        /*
        
        // get the angle of the camera

        // X is the up/down angle, Y is the left/right angle of camera
        float rotX = Camera.main.transform.eulerAngles.x;
        float rotY = Camera.main.transform.eulerAngles.y;

        // if the min boundry dips below 0 or the max boundry goes above 360 then we have to subtract or add 360 to the cam angle
        float tempCamAngleY = FlipCheck(rotY, audioAngle.y, minAngle.y, maxAngle.y);
        float tempCamAngleX = FlipCheck(rotX, audioAngle.x, minAngle.x, maxAngle.x);


        // if the camera angle is within all the min/max bounds then set the volume
        if (tempCamAngleY > audioAngle.y + minAngle.y && tempCamAngleY < audioAngle.y + maxAngle.y && tempCamAngleX > audioAngle.x + minAngle.x && tempCamAngleX < audioAngle.x + maxAngle.x)
        {
            //
            float yVol = 0;
            float xVol = 0;
            // calculate y volume, if attuation is greater than 0 than add a falloff
            if (tempCamAngleY < audioAngle.y)
            {
                yVol = 1;
                if (minFalloff.y > 0)
                    yVol -= (Mathf.Abs((audioAngle.y - tempCamAngleY) / (minAngle.y)));
            }
            else
            {
                yVol = 1;
                if (maxFalloff.y > 0)
                    yVol -= (Mathf.Abs((tempCamAngleY - audioAngle.y) / (maxAngle.y)));
            }

            // calculate the x volume
            if (tempCamAngleX < audioAngle.x)
            {
                xVol = 1;
                if (minFalloff.x > 0)
                    xVol -= (Mathf.Abs((audioAngle.x - tempCamAngleX) / (minAngle.x)));
            }
            else
            {
                xVol = 1;
                if (maxFalloff.x > 0)
                    xVol -= (Mathf.Abs((tempCamAngleX - audioAngle.x) / (maxAngle.x)));
            }

            // multiply the two volumes to find the final output volume
            float finalvol = yVol * xVol;
            audioSource.volume = finalvol;
            //float debug = finalvol;
            //Debug.Log ("Audio: " + debug);
        }
        else
        {
            audioSource.volume = 0.0f;
        }
        */
    }

}
