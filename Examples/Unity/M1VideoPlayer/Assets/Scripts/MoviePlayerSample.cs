/************************************************************************************

Filename    :   MoviePlayerSample.cs
Content     :   An example of how to use the Moonlight video player
Created     :   July 12, 2014

Copyright   :   Copyright 2014 Oculus VR, LLC. All Rights reserved.

Use of this software is subject to the terms of the Oculus LLC license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

************************************************************************************/

using UnityEngine;
using System.Collections;					// required for Coroutines
using System.Runtime.InteropServices;		// required for DllImport
using System;								// requred for IntPtr


/************************************************************************************
Usage:

	Place a simple textured quad surface with the correct aspect ratio in your scene.

	Add the MoviePlayerSample.cs script to the surface object.

	Modify it for the correct media location (currently hardcoded)

	Modify it to start when you want, instead of on the very first Update().

Implementation:

	In the MoviePlayerSample Awake() call, GetNativeTextureID() is called on 
	renderer.material.mainTexture.
	
	When the Oculus plugin gets the initialization event on the render thread, 
	it creates a new Android SurfaceTexture and Surface object in preparation 
	for receiving media. 

	When the game wants to start the video playing, it calls the StartVideoPlayerOnTextureId()
	script call, which creates an Android MediaPlayer java object, issues a 
	native plugin call to tell the native code which texture id to put the video
	on and return the Android Surface object to pass to MediaPlayer, then sets
	up the media stream and starts it.
	
	Every frame, right after VrApi::WarpSwap() is called by the native plugin,
	the	SurfaceTexture object is checked for updates.  If there is one, the
	target texId is re-created at the correct dimensions and format if it is
	the first frame, then the video image is rendered to it and mipmapped.  
	The following frame, instead of Unity drawing the image that was placed 
	on the surface in the Unity editor, it will draw the current video frame.

	It is important to note that the texture is actually replaced -- the original
	version is gone, and the video will now show up everywhere that texture was
	used, not just on sthe GameObject that ran the script.

	NOTE: The resolution of the movie texture is hard coded to 960x540 in MediaSurface.cpp.
	
************************************************************************************/

public class MoviePlayerSample : MonoBehaviour
{
	public string moviePath = "/storage/emulated/0/Mach1/M1SpatialLoader/video.mp4";
	private int nativeTextureID;
	bool	startedVideo = false;
	
	#if (UNITY_ANDROID && !UNITY_EDITOR)
	bool	videoPaused = false;
	AndroidJavaObject	mediaPlayer = null;
	#endif
	
	/// <summary>
	/// Initialization of the movie surface
	/// </summary>
	void Awake()
	{
		Renderer r = GetComponent<Renderer>();
		if (r != null)
		{
			Material m = r.material;
			if (m == null || m.mainTexture == null)
			{
				Debug.LogError("Can't GetNativeTextureID() for movie surface");
			}
			
			// This aparently has to be done at Awake time, before
			// multithreaded rendering starts;
			nativeTextureID = m.mainTexture.GetNativeTextureID();
			Debug.Log("Movie Texture id: " + nativeTextureID);
		}
		else
		{
			Debug.LogError("No renderer component");
		}
	}
	
	/// <summary>
	/// Auto-starts video playback
	/// </summary>
	void DelayedStartVideo()
	{
		if (!startedVideo)
		{
			startedVideo = true;
			#if (UNITY_ANDROID && !UNITY_EDITOR)
			// This can only be done once multithreaded rendering is running
			mediaPlayer = StartVideoPlayerOnTextureId(nativeTextureID);
			#endif
		}
	}
	
	public void PlayVideo()
	{
		#if (UNITY_ANDROID && !UNITY_EDITOR)
		DelayedStartVideo();
		#endif
	}
	
	/// <summary>
	/// Pauses video playback when the app loses or gains focus
	/// </summary>
	void OnApplicationPause(bool wasPaused)
	{
		Debug.Log("OnApplicationPause: " + wasPaused);
		#if (UNITY_ANDROID && !UNITY_EDITOR)
		if (mediaPlayer != null)
		{
			videoPaused = wasPaused;
			mediaPlayer.Call(( videoPaused ) ? "pause" : "start");
		}
		#endif
	}
	
	#if (UNITY_ANDROID && !UNITY_EDITOR)
	// This function returns an Android Surface object that is
	// bound to a SurfaceTexture object on an independent OpenGL texture id.
	// Each frame, before the TimeWarp processing, the SurfaceTexture is checked
	// for updates, and if one is present, the contents of the SurfaceTexture
	// will be copied over to the provided surfaceTexId and mipmaps will be 
	// generated so normal Unity rendering can use it.
	[DllImport("OculusPlugin")]
	private static extern IntPtr OVR_Media_Surface( int surfaceTexId, int surfaceWidth, int surfaceHeight );
	#endif
	
	#if (UNITY_ANDROID && !UNITY_EDITOR)
	/// <summary>
	/// Set up the video player with the movie surface texture id
	/// </summary>
	AndroidJavaObject StartVideoPlayerOnTextureId(int textureId)
	{
		Debug.Log("SetUpVideoPlayer ");
		
		IntPtr  androidSurface = OVR_Media_Surface( textureId, 3840, 2160 );
		//IntPtr  androidSurface = OVR_Media_Surface( textureId, 2880, 1440 );
		
		AndroidJavaObject mediaPlayer = new AndroidJavaObject("android/media/MediaPlayer");
		
		// Can't use AndroidJavaObject.Call() with a jobject, must use low level interface
		//mediaPlayer.Call("setSurface", androidSurface);
		IntPtr setSurfaceMethodId = AndroidJNI.GetMethodID(mediaPlayer.GetRawClass(),"setSurface","(Landroid/view/Surface;)V");
		jvalue[] parms = new jvalue[1];
		parms[0] = new jvalue();
		parms[0].l = androidSurface;
		AndroidJNI.CallObjectMethod(mediaPlayer.GetRawObject(), setSurfaceMethodId, parms);
		
		mediaPlayer.Call("setDataSource", moviePath);
		mediaPlayer.Call("prepare");
		mediaPlayer.Call("setLooping", true);
		mediaPlayer.Call("start");
		
		return mediaPlayer;
	}
	#endif
}
