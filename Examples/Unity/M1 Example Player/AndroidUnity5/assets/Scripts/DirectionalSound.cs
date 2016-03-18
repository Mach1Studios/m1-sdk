using System;
using UnityEngine;
using System.Collections;

public class DirectionalSound : MonoBehaviour
{
    [Header("Name of file in \"Resources\" folder")]
    public string FileName;

    public bool IsAudioLoaded { get; private set; }

    //public string audioPath;
    public Vector2 audioOrigin;
    public Vector2 minAngle;
    public Vector2 maxAngle;
    public Vector2 minFalloff;
    public Vector2 maxFalloff;


    private AudioClip _audioClip;
    private AudioSource source;
    private Vector3 audioAngle;

    void Awake()
    {
        source = GetComponent<AudioSource>();
    }

    // Use this for initialization
    void Start()
    {

        // calculate the audio angle based on the audioSource position in worldspace relative to the origin (OVR camera)
        // NOTE: not working well, so im just going to make it an attribute
        //Vector3 relPos = transform.position - Camera.main.transform.position;
        //Quaternion qRot = Quaternion.LookRotation(relPos);
        //audioAngle = qRot.eulerAngles;

        audioAngle = audioOrigin;
        StartCoroutine(LoadAudioFromResourcesAsync(FileName, x => { IsAudioLoaded = true; }));
    }

    IEnumerator LoadAudioFromResourcesAsync(string fileName, AsyncCallback callback)
    {
        ResourceRequest request = Resources.LoadAsync<AudioClip>(fileName);
        yield return request;
        _audioClip = request.asset as AudioClip;
        callback.Invoke(null);
    }

    public void PlayAudio()
    {
        if (_audioClip != null)
        {
            source.clip = _audioClip;
            source.Play(0);
        }
        else
        {
            Debug.LogError("Audio was not loaded");
        }
    }

    public void PauseAudio()
    {
        if (_audioClip != null)
        {
            source.Pause();
        }
        else
        {
            Debug.LogError("Audio was not loaded");
        }
    }

    //public void PlayAudio()
    //{
    //    if (_audioClip != null)
    //    {
    //        source.PlayOneShot(_audioClip, 1F);
    //    }
    //    else
    //    {
    //        Debug.LogError("Audio was not loaded");
    //    }
    //}

    // Update is called once per frame
    void Update()
    {
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
            source.volume = finalvol;
            //float debug = finalvol;
            //Debug.Log ("Audio: " + debug);
        }
        else
        {
            source.volume = 0F;
        }
    }

    float FlipCheck(float camAngle, float curAudioAngle, float curBoundryAngleMin, float curBoundryAngleMax)
    {
        float result = camAngle;
        if (curAudioAngle + curBoundryAngleMin < 0)
        {
            if (camAngle < (curAudioAngle + 360) && camAngle > (curAudioAngle + 360) + curBoundryAngleMin)
            {
                result -= 360;
            }
        }
        if (curAudioAngle + curBoundryAngleMax > 360)
        {
            if (camAngle > (curAudioAngle - 360) && camAngle < (curAudioAngle - 360) + curBoundryAngleMax)
            {
                result += 360;
            }
        }
        return result;
    }
}
