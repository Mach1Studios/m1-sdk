using UnityEngine;
using System.Collections;

public class MediaPlayerEvent : MonoBehaviour {


	public MediaPlayerCtrl m_srcVideo;

	// Use this for initialization
	void Start () {
		m_srcVideo.OnReady += OnReady;
		m_srcVideo.OnVideoFirstFrameReady += OnFirstFrameReady;
		m_srcVideo.OnVideoError += OnError;
		m_srcVideo.OnEnd += OnEnd;
		m_srcVideo.OnResize += OnResize;

	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnReady() {

		Debug.Log ("OnReady");
	}

	void OnFirstFrameReady() {
		Debug.Log ("OnFirstFrameReady");
	}

	void OnEnd() {
		Debug.Log ("OnEnd");
	}

	void OnResize()
	{
		Debug.Log ("OnResize");
	}

	void OnError(MediaPlayerCtrl.MEDIAPLAYER_ERROR errorCode, MediaPlayerCtrl.MEDIAPLAYER_ERROR errorCodeExtra){
		Debug.Log ("OnError");
	}
}
