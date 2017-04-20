

using UnityEngine;
using System.Runtime.InteropServices;
using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
#if !UNITY_WEBPLAYER && !UNITY_WEBGL && !UNITY_WP8 && !UNITY_WP8_1
using FFmpeg.AutoGen;
using System.Threading;
#endif

#if UNITY_EDITOR || UNITY_STANDALONE
public unsafe class MediaPlayerCtrl : MonoBehaviour {
#else
public class MediaPlayerCtrl : MonoBehaviour
{
#endif




	public string m_strFileName;
	public GameObject[] m_TargetMaterial = null;
	private Texture2D m_VideoTexture = null;
	private Texture2D m_VideoTextureDummy = null;

	private Texture2D m_VideoTextureY;
	private Texture2D m_VideoTextureU;
	private Texture2D m_VideoTextureV;

	private Texture2D m_VideoTextureDummyY;
	private Texture2D m_VideoTextureDummyU;
	private Texture2D m_VideoTextureDummyV;

	private MEDIAPLAYER_STATE m_CurrentState;
	private int m_iCurrentSeekPosition;
	private float m_fVolume = 1.0f;
	private int m_iWidth;
	private int m_iHeight;
	private float m_fSpeed = 1.0f;

	public bool m_bFullScreen = false;//Please use only in FullScreen prefab.
	public bool m_bSupportRockchip = true; //Using a device support Rochchip or Low-end devices
	//(Reason 1 : Not directly play in StreamingAssets)
	//(Reason 2 : Video buffer is RGB565 only supported)

	public bool m_bPC_FastMode = false;

	public delegate void VideoEnd();
	public delegate void VideoReady();
	public delegate void VideoError(MEDIAPLAYER_ERROR errorCode, MEDIAPLAYER_ERROR errorCodeExtra);
	public delegate void VideoFirstFrameReady();
	public delegate void VideoResize ();


	public VideoResize OnResize;
	public VideoReady OnReady;
	public VideoEnd OnEnd;
	public VideoError OnVideoError;
	public VideoFirstFrameReady OnVideoFirstFrameReady;

	private IntPtr m_texPtr;
	private IntPtr m_texPtrY;
	private IntPtr m_texPtrU;
	private IntPtr m_texPtrV;

	public Shader m_shaderYUV;


	#if UNITY_IPHONE || UNITY_TVOS

	private int m_iPauseFrame;
	#endif





	#if UNITY_ANDROID && !UNITY_EDITOR && UNITY_5
	[DllImport ("BlueDoveMediaRender")]
	private static extern void InitNDK();

	#if UNITY_5_2  || UNITY_5_3_OR_NEWER
	[DllImport ("BlueDoveMediaRender")]
	private static extern IntPtr EasyMovieTextureRender();
	#endif
	#endif


	#if (UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN)

	[DllImport ("EasyMovieTexture")]
	private static extern int SetTexture();

	[DllImport ("EasyMovieTexture")]
	private static extern void ReleaseTexture(int iID);

	[DllImport ("EasyMovieTexture")]
	private static extern void SetTextureFromUnity(int iID,System.IntPtr texture,System.IntPtr textureY,System.IntPtr textureU,System.IntPtr textureV, int w, int h,byte[] data);


	[DllImport("EasyMovieTexture")]
	private static extern IntPtr GetRenderEventFunc();



	#endif

	#if (UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX )
	[DllImport ("EasyMovieTextureRender")]
	private static extern int SetTexture();

	[DllImport ("EasyMovieTextureRender")]
	private static extern void ReleaseTexture(int iID);

	[DllImport ("EasyMovieTextureRender")]
	private static extern void SetTextureFromUnity(int iID,System.IntPtr texture,System.IntPtr textureY,System.IntPtr textureU,System.IntPtr textureV, int w, int h,byte[] data);

	[DllImport("EasyMovieTextureRender")]
	private static extern IntPtr GetRenderEventFunc();


	#endif





	private int m_iAndroidMgrID;
	private bool m_bIsFirstFrameReady;


	public enum MEDIAPLAYER_ERROR
	{
		MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK = 200,
		MEDIA_ERROR_IO = -1004,
		MEDIA_ERROR_MALFORMED = -1007,
		MEDIA_ERROR_TIMED_OUT = -110,
		MEDIA_ERROR_UNSUPPORTED = -1010,
		MEDIA_ERROR_SERVER_DIED = 100,
		MEDIA_ERROR_UNKNOWN = 1
	}

	public enum MEDIAPLAYER_STATE
	{
		NOT_READY = 0,
		READY = 1,
		END = 2,
		PLAYING = 3,
		PAUSED = 4,
		STOPPED = 5,
		ERROR = 6
	}

	public enum MEDIA_SCALE
	{
		SCALE_X_TO_Y = 0,
		SCALE_X_TO_Z = 1,
		SCALE_Y_TO_X = 2,
		SCALE_Y_TO_Z = 3,
		SCALE_Z_TO_X = 4,
		SCALE_Z_TO_Y = 5,
		SCALE_X_TO_Y_2 = 6,
	}

	bool m_bFirst = false;

	public MEDIA_SCALE m_ScaleValue;
	public GameObject[] m_objResize = null;
	public bool m_bLoop = false;
	public bool m_bAutoPlay = true;
	private bool m_bStop = false;

	private bool m_bInit = false;



	#if !UNITY_WEBPLAYER && !UNITY_WEBGL && !UNITY_WP8 && !UNITY_WP8_1

	static MediaPlayerCtrl()
	{
	#if UNITY_EDITOR
		String currentPath = Environment.GetEnvironmentVariable ("PATH", EnvironmentVariableTarget.Process);

		String dllPath = Environment.CurrentDirectory + Path.DirectorySeparatorChar + "Assets" + Path.DirectorySeparatorChar + "Plugins";

		if (currentPath.Contains (dllPath) == false) {
			Environment.SetEnvironmentVariable ("PATH", currentPath + Path.PathSeparator + dllPath, EnvironmentVariableTarget.Process);
		}

		dllPath = Environment.CurrentDirectory + Path.DirectorySeparatorChar + "Assets" + Path.DirectorySeparatorChar + "Plugins"  + Path.DirectorySeparatorChar + "x64";

		if (currentPath.Contains (dllPath) == false) {
			Environment.SetEnvironmentVariable ("PATH", currentPath + Path.PathSeparator + dllPath, EnvironmentVariableTarget.Process);
		}

		dllPath = Environment.CurrentDirectory + Path.DirectorySeparatorChar + "Assets" + Path.DirectorySeparatorChar + "Plugins"  + Path.DirectorySeparatorChar + "x86";

		if (currentPath.Contains (dllPath) == false) {
			Environment.SetEnvironmentVariable ("PATH", currentPath + Path.PathSeparator + dllPath, EnvironmentVariableTarget.Process);
		}

	#endif
	}

	void Awake()
	{


	#if UNITY_STANDALONE
		String currentPath = Environment.GetEnvironmentVariable("PATH", EnvironmentVariableTarget.Process);

		String dllPath = Application.dataPath + Path.DirectorySeparatorChar + "Plugins";

		if(currentPath.Contains(dllPath) == false)
		{
			Environment.SetEnvironmentVariable("PATH", currentPath + Path.PathSeparator + dllPath, EnvironmentVariableTarget.Process);
		}
	#endif


		if (SystemInfo.deviceModel.Contains("rockchip"))
		{
			m_bSupportRockchip = true;
		}
		else
		{
			m_bSupportRockchip = false;
		}


	#if UNITY_IPHONE  || UNITY_TVOS || UNITY_EDITOR || UNITY_STANDALONE

		if(m_TargetMaterial!=null)
		{

			for( int iIndex = 0; iIndex < m_TargetMaterial.Length; iIndex++)
			{
				if( m_TargetMaterial[iIndex] != null)
				{
					if(m_TargetMaterial[iIndex].GetComponent<MeshFilter>() != null)
					{
						Vector2 [] vec2UVs= m_TargetMaterial[iIndex].GetComponent<MeshFilter>().mesh.uv;

						for(int i = 0; i < vec2UVs.Length; i++)
						{
							vec2UVs[i] = new Vector2(vec2UVs[i].x, 1.0f -vec2UVs[i].y);
						}

						m_TargetMaterial[iIndex].GetComponent<MeshFilter>().mesh.uv = vec2UVs;
					}

					if(m_TargetMaterial[iIndex].GetComponent<RawImage>() != null)
					{
						m_TargetMaterial[iIndex].GetComponent<RawImage>().uvRect = new Rect(0,1,1,-1);
					}
				}

			}


		}



	#endif

	#if UNITY_STANDALONE || UNITY_EDITOR
		//RegisterDebugCallback(new DebugCallback(DebugMethod));
		//threadVideo = new Thread(ThreadUpdate);
		//threadVideo.Start();
	#endif

	#if UNITY_ANDROID && !UNITY_EDITOR

	#if UNITY_5
	if( SystemInfo.graphicsMultiThreaded == true)
	InitNDK();
	#endif
		m_iAndroidMgrID = Call_InitNDK();
	#endif

		Call_SetUnityActivity();






	}
	// Use this for initialization
	void Start()
	{








	#if UNITY_ANDROID
	if (Application.dataPath.Contains(".obb")) {

	Call_SetSplitOBB(true,Application.dataPath);
	}
	else
	{
	Call_SetSplitOBB(false, null);
	}
	#endif

		m_bInit = true;

	}


	void OnApplicationQuit()
	{

		//if (System.IO.Directory.Exists(Application.persistentDataPath + "/Data") == true)
		//    System.IO.Directory.Delete(Application.persistentDataPath + "/Data", true);
	}

	bool m_bCheckFBO = false;

	void OnDisable()
	{
		if (GetCurrentState() == MEDIAPLAYER_STATE.PLAYING)
		{
			Pause();
		}
	}

	void OnEnable()
	{
		if (GetCurrentState() == MEDIAPLAYER_STATE.PAUSED)
		{
			Play();
		}
	}


	void Update()
	{


		if (string.IsNullOrEmpty(m_strFileName))
		{
			return;
		}

		if (checkNewActions)
		{			
			checkNewActions = false;
			CheckThreading ();
		}

		if (m_bFirst == false)
		{





			string strName = m_strFileName.Trim();

	#if UNITY_IPHONE  || UNITY_TVOS
	/*if (strName.StartsWith("http",StringComparison.OrdinalIgnoreCase))
	{
	StartCoroutine( DownloadStreamingVideoAndLoad(strName) );
	}
	else*/
	{
	Call_Load(strName,0);
	}

	#elif UNITY_ANDROID 

	if(m_bSupportRockchip)
	{
	Call_SetRockchip(m_bSupportRockchip);

	if(strName.Contains("://"))
	{
	Call_Load(strName,0);
	}
	else
	{
	//Call_Load(strName,0);
	StartCoroutine( CopyStreamingAssetVideoAndLoad(strName));
	}

	}
	else
	{

	Call_Load(strName,0);
	}
	#elif UNITY_STANDALONE
			Call_Load(strName,0);

	#endif

			Call_SetLooping(m_bLoop);
			m_bFirst = true;


		}


		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.PAUSED)
		{
			if (m_bCheckFBO == false) {


				if (Call_GetVideoWidth () <= 0 || Call_GetVideoHeight () <= 0) {
					return;
				}

				m_iWidth = Call_GetVideoWidth ();
				m_iHeight = Call_GetVideoHeight ();

				Resize ();

				if (m_VideoTexture != null) {

					//Destroy(m_VideoTexture);

					if (m_VideoTextureDummy != null) {
						Destroy (m_VideoTextureDummy);
						m_VideoTextureDummy = null;
					}
						

					m_VideoTextureDummy = m_VideoTexture;
					m_VideoTexture = null;


				}

				if (m_VideoTextureY != null) {


					if (m_VideoTextureDummyY != null) {
						Destroy (m_VideoTextureDummyY);
						m_VideoTextureDummyY = null;
					}


					m_VideoTextureDummyY = m_VideoTextureY;
					m_VideoTextureY = null;


				}


				if (m_VideoTextureU != null) {


					if (m_VideoTextureDummyU != null) {
						Destroy (m_VideoTextureDummyU);
						m_VideoTextureDummyU = null;
					}


					m_VideoTextureDummyU = m_VideoTextureU;
					m_VideoTextureU = null;


				}

				if (m_VideoTextureV != null) {


					if (m_VideoTextureDummyV != null) {
						Destroy (m_VideoTextureDummyV);
						m_VideoTextureDummyV = null;
					}


					m_VideoTextureDummyV = m_VideoTextureV;
					m_VideoTextureV = null;


				}


	#if UNITY_ANDROID || UNITY_EDITOR || UNITY_STANDALONE

#if UNITY_EDITOR || UNITY_STANDALONE
				if(m_bPC_FastMode == true)
				{

					m_VideoTextureY = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.Alpha8, false);
					m_VideoTextureU = new Texture2D (Call_GetVideoWidth () / 2, Call_GetVideoHeight () / 2, TextureFormat.Alpha8, false);
					m_VideoTextureV = new Texture2D (Call_GetVideoWidth () / 2, Call_GetVideoHeight () / 2, TextureFormat.Alpha8, false);

					m_VideoTextureY.filterMode = FilterMode.Bilinear;
					m_VideoTextureY.wrapMode = TextureWrapMode.Clamp;

					m_VideoTextureU.filterMode = FilterMode.Bilinear;
					m_VideoTextureU.wrapMode = TextureWrapMode.Clamp;

					m_VideoTextureV.filterMode = FilterMode.Bilinear;
					m_VideoTextureV.wrapMode = TextureWrapMode.Clamp;
				

				}
				else
				{
					if (m_bSupportRockchip) {
						m_VideoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGB565, false);

					} else {
						m_VideoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGBA32, false);
					}
					m_VideoTexture.filterMode = FilterMode.Bilinear;
					m_VideoTexture.wrapMode = TextureWrapMode.Clamp;
				}







				if(m_bPC_FastMode == true)
				{
					m_texPtrY = m_VideoTextureY.GetNativeTexturePtr ();
					m_texPtrU = m_VideoTextureU.GetNativeTexturePtr ();
					m_texPtrV = m_VideoTextureV.GetNativeTexturePtr ();
				}
				else
				{
					m_texPtr = m_VideoTexture.GetNativeTexturePtr ();
				}

#else
				if (m_bSupportRockchip) {
					m_VideoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGB565, false);
					
				} else {
					m_VideoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGBA32, false);
				}
				m_VideoTexture.filterMode = FilterMode.Bilinear;
				m_VideoTexture.wrapMode = TextureWrapMode.Clamp;

				m_texPtr = m_VideoTexture.GetNativeTexturePtr ();
#endif


	#if UNITY_5_2 || UNITY_5_3_OR_NEWER
				Call_SetUnityTexture((int)m_texPtr);
	#else
	Call_SetUnityTexture (m_VideoTexture.GetNativeTextureID ());
	#endif
	#endif
				Call_SetWindowSize ();
				m_bCheckFBO = true;
				if (OnResize != null)
					OnResize ();
			} else {
				if (Call_GetVideoWidth () != m_iWidth || Call_GetVideoHeight () != m_iHeight) {
					m_iWidth = Call_GetVideoWidth ();
					m_iHeight = Call_GetVideoHeight ();


					if (OnResize != null)
						OnResize ();

					ResizeTexture ();
				}
			}


			Call_UpdateVideoTexture();



			m_iCurrentSeekPosition = Call_GetSeekPosition();




		}



		if (m_CurrentState != Call_GetStatus())
		{

			m_CurrentState = Call_GetStatus();



			if (m_CurrentState == MEDIAPLAYER_STATE.READY)
			{

				if (OnReady != null)
					OnReady();

				if (m_bAutoPlay)
					Call_Play(0);

				if (m_bReadyPlay) {
					Call_Play(0);
					m_bReadyPlay = false;
				}

				SetVolume(m_fVolume);



			}
			else if (m_CurrentState == MEDIAPLAYER_STATE.END)
			{
				if (OnEnd != null)
					OnEnd();

				if (m_bLoop == true)
				{
					Call_Play(0);
				}
			}
			else if (m_CurrentState == MEDIAPLAYER_STATE.ERROR)
			{
				OnError((MEDIAPLAYER_ERROR)Call_GetError(), (MEDIAPLAYER_ERROR)Call_GetErrorExtra());
			}

		}

		GL.InvalidateState ();


	}

	public void DeleteVideoTexture()
	{
		if (m_VideoTextureDummy != null)
		{
			Destroy(m_VideoTextureDummy);
			m_VideoTextureDummy = null;
		}

		if (m_VideoTexture != null)
		{
			Destroy(m_VideoTexture);
			m_VideoTexture = null;
		}


		if (m_VideoTextureDummyY != null)
		{
			Destroy(m_VideoTextureDummyY);
			m_VideoTextureDummyY = null;
		}

		if (m_VideoTextureY != null)
		{
			Destroy(m_VideoTextureY);
			m_VideoTextureY = null;
		}

		if (m_VideoTextureDummyU != null)
		{
			Destroy(m_VideoTextureDummyU);
			m_VideoTextureDummyU = null;
		}

		if (m_VideoTextureU != null)
		{
			Destroy(m_VideoTextureU);
			m_VideoTextureU = null;
		}

		if (m_VideoTextureDummyV != null)
		{
			Destroy(m_VideoTextureDummyV);
			m_VideoTextureDummyV = null;
		}

		if (m_VideoTextureV != null)
		{
			Destroy(m_VideoTextureV);
			m_VideoTextureV = null;
		}
	}

	public void ResizeTexture()
	{
		Debug.Log ("ResizeTexture " + m_iWidth + " " + m_iHeight);

		if (m_iWidth == 0 || m_iHeight == 0)
			return;

		if (m_VideoTexture != null) {

			//Destroy(m_VideoTexture);

			if (m_VideoTextureDummy != null) {
				Destroy (m_VideoTextureDummy);
				m_VideoTextureDummy = null;
			}

			m_VideoTextureDummy = m_VideoTexture;
			m_VideoTexture = null;


		}

		if (m_VideoTextureY != null) {


			if (m_VideoTextureDummyY != null) {
				Destroy (m_VideoTextureDummyY);
				m_VideoTextureDummyY = null;
			}


			m_VideoTextureDummyY = m_VideoTextureY;
			m_VideoTextureY = null;


		}


		if (m_VideoTextureU != null) {


			if (m_VideoTextureDummyU != null) {
				Destroy (m_VideoTextureDummyU);
				m_VideoTextureDummyU = null;
			}


			m_VideoTextureDummyU = m_VideoTextureU;
			m_VideoTextureU = null;


		}

		if (m_VideoTextureV != null) {


			if (m_VideoTextureDummyV != null) {
				Destroy (m_VideoTextureDummyV);
				m_VideoTextureDummyV = null;
			}


			m_VideoTextureDummyV = m_VideoTextureV;
			m_VideoTextureV = null;


		}

	#if UNITY_IPHONE && !UNITY_EDITOR
	_videoTexture =null;
	bFirstIOS = true;
	#endif

	#if UNITY_ANDROID || UNITY_EDITOR || UNITY_STANDALONE

			#if UNITY_EDITOR || UNITY_STANDALONE
		if(m_bPC_FastMode == true)
		{
			m_VideoTextureY = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.Alpha8, false);
			m_VideoTextureU = new Texture2D (Call_GetVideoWidth () / 2, Call_GetVideoHeight () / 2, TextureFormat.Alpha8, false);
			m_VideoTextureV = new Texture2D (Call_GetVideoWidth () / 2, Call_GetVideoHeight () / 2, TextureFormat.Alpha8, false);
		}
		else
		{
			if (m_bSupportRockchip) {
				m_VideoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGB565, false);

			} else {
				m_VideoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGBA32, false);
			}
		}


		m_VideoTexture.filterMode = FilterMode.Bilinear;
		m_VideoTexture.wrapMode = TextureWrapMode.Clamp;

		if(m_bPC_FastMode == true)
		{
			m_texPtrY = m_VideoTextureY.GetNativeTexturePtr ();
			m_texPtrU = m_VideoTextureU.GetNativeTexturePtr ();
			m_texPtrV = m_VideoTextureV.GetNativeTexturePtr ();
		}
		else
		{
			m_texPtr = m_VideoTexture.GetNativeTexturePtr ();
		}

#else
		if (m_bSupportRockchip) {
			m_VideoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGB565, false);
			
		} else {
			m_VideoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGBA32, false);
		}

		m_VideoTexture.filterMode = FilterMode.Bilinear;
		m_VideoTexture.wrapMode = TextureWrapMode.Clamp;

		m_texPtr = m_VideoTexture.GetNativeTexturePtr ();
#endif

	#if UNITY_5_2 || UNITY_5_3_OR_NEWER
		Call_SetUnityTexture((int)m_texPtr);
	#else
	Call_SetUnityTexture (m_VideoTexture.GetNativeTextureID ());
	#endif
	#endif

		Call_SetWindowSize ();
	}

	public void Resize()
	{
		if (m_CurrentState != MEDIAPLAYER_STATE.PLAYING)
			return;

		if (Call_GetVideoWidth() <= 0 || Call_GetVideoHeight() <= 0)
		{
			return;
		}

		if (m_objResize != null)
		{
			int iScreenWidth = Screen.width;
			int iScreenHeight = Screen.height;

			float fRatioScreen = (float)iScreenHeight / (float)iScreenWidth;
			int iWidth = Call_GetVideoWidth();
			int iHeight = Call_GetVideoHeight();

			float fRatio = (float)iHeight / (float)iWidth;
			float fRatioResult = fRatioScreen / fRatio;

			for (int i = 0; i < m_objResize.Length; i++)
			{
				if (m_objResize[i] == null)
					continue;

				if (m_bFullScreen)
				{



					m_objResize[i].transform.localScale = new Vector3(20.0f / fRatioScreen, 20.0f / fRatioScreen, 1.0f);
					if (fRatio < 1.0f)
					{
						if (fRatioScreen < 1.0f)
						{
							if (fRatio > fRatioScreen)
							{
								m_objResize[i].transform.localScale *= fRatioResult;
							}
						}

						m_ScaleValue = MEDIA_SCALE.SCALE_X_TO_Y;
					}
					else
					{
						if (fRatioScreen > 1.0f) {
							if (fRatio >= fRatioScreen) {

								m_objResize [i].transform.localScale *= fRatioResult;
							}
						} else {
							m_objResize [i].transform.localScale *= fRatioResult;

						}

						m_ScaleValue = MEDIA_SCALE.SCALE_X_TO_Y;
					}
				}



				if (m_ScaleValue == MEDIA_SCALE.SCALE_X_TO_Y)
				{
					m_objResize[i].transform.localScale
					= new Vector3(m_objResize[i].transform.localScale.x
						, m_objResize[i].transform.localScale.x * fRatio
						, m_objResize[i].transform.localScale.z);
				}
				else if (m_ScaleValue == MEDIA_SCALE.SCALE_X_TO_Y_2)
				{
					m_objResize[i].transform.localScale
					= new Vector3(m_objResize[i].transform.localScale.x
						, m_objResize[i].transform.localScale.x * fRatio / 2.0f
						, m_objResize[i].transform.localScale.z);
				}
				else if (m_ScaleValue == MEDIA_SCALE.SCALE_X_TO_Z)
				{
					m_objResize[i].transform.localScale
					= new Vector3(m_objResize[i].transform.localScale.x
						, m_objResize[i].transform.localScale.y
						, m_objResize[i].transform.localScale.x * fRatio);
				}
				else if (m_ScaleValue == MEDIA_SCALE.SCALE_Y_TO_X)
				{
					m_objResize[i].transform.localScale
					= new Vector3(m_objResize[i].transform.localScale.y / fRatio
						, m_objResize[i].transform.localScale.y
						, m_objResize[i].transform.localScale.z);
				}
				else if (m_ScaleValue == MEDIA_SCALE.SCALE_Y_TO_Z)
				{
					m_objResize[i].transform.localScale
					= new Vector3(m_objResize[i].transform.localScale.x
						, m_objResize[i].transform.localScale.y
						, m_objResize[i].transform.localScale.y / fRatio);
				}
				else if (m_ScaleValue == MEDIA_SCALE.SCALE_Z_TO_X)
				{
					m_objResize[i].transform.localScale
					= new Vector3(m_objResize[i].transform.localScale.z * fRatio
						, m_objResize[i].transform.localScale.y
						, m_objResize[i].transform.localScale.z);
				}
				else if (m_ScaleValue == MEDIA_SCALE.SCALE_Z_TO_Y)
				{
					m_objResize[i].transform.localScale
					= new Vector3(m_objResize[i].transform.localScale.x
						, m_objResize[i].transform.localScale.z * fRatio
						, m_objResize[i].transform.localScale.z);
				}
				else
				{
					m_objResize[i].transform.localScale
					= new Vector3(m_objResize[i].transform.localScale.x, m_objResize[i].transform.localScale.y, m_objResize[i].transform.localScale.z);
				}
			}

		}
	}



	//The error code is the following sites related documents.
	//http://developer.android.com/reference/android/media/MediaPlayer.OnErrorListener.html 
	void OnError(MEDIAPLAYER_ERROR iCode, MEDIAPLAYER_ERROR iCodeExtra)
	{
		string strError = "";

		switch (iCode)
		{
		case MEDIAPLAYER_ERROR.MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK:
			strError = "MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK";
			break;
		case MEDIAPLAYER_ERROR.MEDIA_ERROR_SERVER_DIED:
			strError = "MEDIA_ERROR_SERVER_DIED";
			break;
		case MEDIAPLAYER_ERROR.MEDIA_ERROR_UNKNOWN:
			strError = "MEDIA_ERROR_UNKNOWN";
			break;
		default:
			strError = "Unknown error " + iCode;
			break;
		}

		strError += " ";

		switch (iCodeExtra)
		{
		case MEDIAPLAYER_ERROR.MEDIA_ERROR_IO:
			strError += "MEDIA_ERROR_IO";
			break;
		case MEDIAPLAYER_ERROR.MEDIA_ERROR_MALFORMED:
			strError += "MEDIA_ERROR_MALFORMED";
			break;
		case MEDIAPLAYER_ERROR.MEDIA_ERROR_TIMED_OUT:
			strError += "MEDIA_ERROR_TIMED_OUT";
			break;
		case MEDIAPLAYER_ERROR.MEDIA_ERROR_UNSUPPORTED:
			strError += "MEDIA_ERROR_UNSUPPORTED";
			break;
		default:
			strError = "Unknown error " + iCode;
			break;
		}



		Debug.LogError(strError);

		if (OnVideoError != null)
		{
			OnVideoError(iCode, iCodeExtra);
		}
	}


	void OnDestroy()
	{


	#if UNITY_ANDROID

	//Call_Reset();

	#endif
		Call_UnLoad();

		DeleteVideoTexture ();


	

		Call_Destroy();


	}

	bool m_bPause = false;


    void OnApplicationPause(bool bPause)
    {
        Debug.Log("ApplicationPause : " + bPause);
        if (bPause == true)
        {

            if (m_CurrentState == MEDIAPLAYER_STATE.PAUSED)
            {
                m_bPause = true;
            }

#if (UNITY_IPHONE  || UNITY_TVOS) && !UNITY_EDITOR
    if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING)
    {
           m_iPauseFrame = m_iCurrentSeekPosition;
           // Stop();
	Pause();
    }
    else
    {
    m_iPauseFrame = 0;
    }
            
#else
            Call_Pause();
#endif


        }
        else
        {
#if ( UNITY_IPHONE  || UNITY_TVOS )&& !UNITY_EDITOR
    if (m_iPauseFrame != 0)
    {
            //m_bStop = false;
            //Call_Play(m_iPauseFrame);
	//Call_Play();
	Call_RePlay();
    }
#else
            Call_RePlay();
#endif

            if (m_bPause == true)
            {
                Call_Pause();
                m_bPause = false;
            }

        }

    }



	public MEDIAPLAYER_STATE GetCurrentState()
	{
		return m_CurrentState;
	}

	public Texture2D GetVideoTexture()
	{
		return m_VideoTexture;
	}


	bool m_bReadyPlay = false;
	public void Play()
	{
		if (m_bStop == true)
		{
			SeekTo(0);
			Call_Play(0);
			m_bStop = false;
		}

		if (m_CurrentState == MEDIAPLAYER_STATE.PAUSED) 
		{
			Call_RePlay ();
		} 
		else if (m_CurrentState == MEDIAPLAYER_STATE.READY || m_CurrentState == MEDIAPLAYER_STATE.STOPPED || m_CurrentState == MEDIAPLAYER_STATE.END) 
		{
			Call_Play (0);
		} 
		else if (m_CurrentState == MEDIAPLAYER_STATE.NOT_READY) 
		{
			m_bReadyPlay = true;
		}

	}

	public void Stop()
	{
		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING)
			Call_Pause();


		m_bStop = true;
		m_CurrentState = MEDIAPLAYER_STATE.STOPPED;
		m_iCurrentSeekPosition = 0;
	}

	public void Pause()
	{
        if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING)
        {
            Call_Pause();
            m_CurrentState = MEDIAPLAYER_STATE.PAUSED;
        }

		
	}

	public void Load(string strFileName)
	{
		if (GetCurrentState() != MEDIAPLAYER_STATE.NOT_READY)
			UnLoad();


       
		m_bReadyPlay = false;

		m_bIsFirstFrameReady = false;

		m_bFirst = false;
		m_bCheckFBO = false;

		m_strFileName = strFileName;

		if (m_bInit == false)
			return;


		m_CurrentState = MEDIAPLAYER_STATE.NOT_READY;
	}



	public void SetVolume(float fVolume)
	{
		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.END || m_CurrentState == MEDIAPLAYER_STATE.READY || m_CurrentState == MEDIAPLAYER_STATE.STOPPED)
		{
			m_fVolume = fVolume;
			Call_SetVolume(fVolume);
		}
	}

	//return milisecond  
	public int GetSeekPosition()
	{

		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.END)
			return m_iCurrentSeekPosition;
		else
			return 0;
	}

	public void SeekTo(int iSeek)
	{
		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.READY || m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.END || m_CurrentState == MEDIAPLAYER_STATE.STOPPED)
			Call_SetSeekPosition(iSeek);

	}


	/// <summary>
	/// Sets the speed.
	/// Experimental API( PC&iOS support, support from Android version 6.0 or later)
	/// </summary>
	/// <param name="fSpeed">video playback speed.</param>
	public void SetSpeed(float fSpeed)
	{

		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.READY || m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.END || m_CurrentState == MEDIAPLAYER_STATE.STOPPED) {
			m_fSpeed = fSpeed;
			Call_SetSpeed (fSpeed);
		}
	}


	//Gets the duration of the file.
	//Returns
	//the duration in milliseconds, if no duration is available (for example, if streaming live content), -1 is returned.
	public int GetDuration()
	{
		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.END || m_CurrentState == MEDIAPLAYER_STATE.READY || m_CurrentState == MEDIAPLAYER_STATE.STOPPED)
			return Call_GetDuration();
		else
			return 0;
	}

	public float GetSeekBarValue()
	{
		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.END || m_CurrentState == MEDIAPLAYER_STATE.READY || m_CurrentState == MEDIAPLAYER_STATE.STOPPED)
		{
			if (GetDuration () == 0) {
				return 0;
			}
			return (float)GetSeekPosition() / (float)GetDuration() ;
		}
		else
			return 0;
	}

	public void SetSeekBarValue(float fValue)
	{
		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.END || m_CurrentState == MEDIAPLAYER_STATE.READY || m_CurrentState == MEDIAPLAYER_STATE.STOPPED)
		{
			if (GetDuration () == 0) {
				return;
			}

			SeekTo( (int)((float)GetDuration() * fValue) );
		}
		else
			return;
	}


	//Only Android support.
	//Get update status in buffering a media stream received through progressive HTTP download. 
	//The received buffering percentage indicates how much of the content has been buffered or played. 
	//For example a buffering update of 80 percent when half the content has already been played indicates that the next 30 percent of the content to play has been buffered.
	//the percentage (0-100) of the content that has been buffered or played thus far 
	public int GetCurrentSeekPercent()
	{
		if (m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.END || m_CurrentState == MEDIAPLAYER_STATE.READY)
			return Call_GetCurrentSeekPercent();
		else
			return 0;
	}

	public int GetVideoWidth()
	{
		return Call_GetVideoWidth();
	}

	public int GetVideoHeight()
	{
		return Call_GetVideoHeight();
	}

	public void UnLoad()
	{
		m_bCheckFBO = false;
#if UNITY_ANDROID

        //Call_Reset();
#endif


		Call_UnLoad();

		m_CurrentState = MEDIAPLAYER_STATE.NOT_READY;

	}



	#if !UNITY_EDITOR && !UNITY_STANDALONE && !UNITY_WEBGL
	#if UNITY_ANDROID

	private AndroidJavaObject javaObj = null;

	private AndroidJavaObject GetJavaObject()
	{
	if (javaObj == null)
	{
	javaObj = new AndroidJavaObject("com.EasyMovieTexture.EasyMovieTexture");
	}

	return javaObj;
	}




	private void Call_Destroy()
	{
	#if UNITY_5
	if( SystemInfo.graphicsMultiThreaded == true)
	{
	#if UNITY_5_2 || UNITY_5_3_OR_NEWER
	GL.IssuePluginEvent(EasyMovieTextureRender(), 5 + m_iAndroidMgrID * 10 + 7000);
	#else
	GL.IssuePluginEvent(5 + m_iAndroidMgrID * 10 + 7000);
	#endif

	}
	else
	{
	GetJavaObject().Call("Destroy");
	}
	#else
	GetJavaObject().Call("Destroy");
	#endif


	}


	private void Call_UnLoad()
	{
	#if UNITY_5
	if( SystemInfo.graphicsMultiThreaded == true)
	{


	#if UNITY_5_2  || UNITY_5_3_OR_NEWER
	GL.IssuePluginEvent(EasyMovieTextureRender(), 4 + m_iAndroidMgrID * 10 + 7000);
	#else
	GL.IssuePluginEvent(4 + m_iAndroidMgrID * 10 + 7000);
	#endif
	}
	else
	{
	GetJavaObject().Call("UnLoad");
	}
	#else
	GetJavaObject().Call("UnLoad");
	#endif


	}

	private bool Call_Load(string strFileName, int iSeek)
	{
	#if UNITY_5
	if( SystemInfo.graphicsMultiThreaded == true)
	{
	GetJavaObject().Call("NDK_SetFileName", strFileName);

	#if UNITY_5_2  || UNITY_5_3_OR_NEWER
	GL.IssuePluginEvent(EasyMovieTextureRender(), 1 + m_iAndroidMgrID * 10 + 7000);
	#else
	GL.IssuePluginEvent(1+ m_iAndroidMgrID * 10 + 7000);
	#endif


	Call_SetNotReady();

	return true;
	}
	else
	{
	GetJavaObject().Call("NDK_SetFileName", strFileName);
	if (GetJavaObject().Call<bool>("Load"))
	{
	return true;
	}
	else
	{
	OnError(MEDIAPLAYER_ERROR.MEDIA_ERROR_UNKNOWN, MEDIAPLAYER_ERROR.MEDIA_ERROR_UNKNOWN);
	return false;
	}
	}
	#else
	GetJavaObject().Call("NDK_SetFileName", strFileName);
	if (GetJavaObject().Call<bool>("Load"))
	{
	return true;
	}
	else
	{
	OnError(MEDIAPLAYER_ERROR.MEDIA_ERROR_UNKNOWN, MEDIAPLAYER_ERROR.MEDIA_ERROR_UNKNOWN);
	return false;
	}
	#endif


	}

	private void Call_UpdateVideoTexture()
	{

	if( Call_IsUpdateFrame() == false)
	return;

	if(m_VideoTextureDummy != null)
	{
	Destroy(m_VideoTextureDummy);
	m_VideoTextureDummy = null;
	}


	for( int i = 0; i < m_TargetMaterial.Length; i++)
	{
	if(m_TargetMaterial[i])
	{
	if(m_TargetMaterial[i].GetComponent<MeshRenderer>()!= null)
	{
	if(m_TargetMaterial[i].GetComponent<MeshRenderer>().material.mainTexture != m_VideoTexture)
	{
	m_TargetMaterial[i].GetComponent<MeshRenderer>().material.mainTexture = m_VideoTexture;
	}
	}

	if(m_TargetMaterial[i].GetComponent<RawImage>()!= null)
	{
	if(m_TargetMaterial[i].GetComponent<RawImage>().texture != m_VideoTexture)
	{
	m_TargetMaterial[i].GetComponent<RawImage>().texture = m_VideoTexture;
	}
	}



	}
	}


	#if UNITY_5
	if( SystemInfo.graphicsMultiThreaded == true)
	{
	#if UNITY_5_2  || UNITY_5_3_OR_NEWER
	GL.IssuePluginEvent(EasyMovieTextureRender(), 3 + m_iAndroidMgrID * 10 + 7000);
	#else
	GL.IssuePluginEvent(3+ m_iAndroidMgrID * 10 + 7000);
	#endif


	}
	else
	{
	GetJavaObject().Call("UpdateVideoTexture");
	}
	#else
	GetJavaObject().Call("UpdateVideoTexture");
	#endif

	if (!m_bIsFirstFrameReady)
	{
	m_bIsFirstFrameReady = true;
	if (OnVideoFirstFrameReady != null)
	{
	OnVideoFirstFrameReady();
	//OnVideoFirstFrameReady = null;
	}
	}
	}

	private void Call_SetVolume(float fVolume)
	{
	GetJavaObject().Call("SetVolume",fVolume);
	}

	private void Call_SetSeekPosition(int iSeek)
	{
	GetJavaObject().Call("SetSeekPosition",iSeek);
	}

	private int Call_GetSeekPosition()
	{
	return GetJavaObject().Call<int>("GetSeekPosition");
	}

	private void Call_Play(int iSeek)
	{
	GetJavaObject().Call("Play",iSeek);
	}

	private void Call_Reset()
	{
	GetJavaObject().Call("Reset");
	}

	private void Call_Stop()
	{
	GetJavaObject().Call("Stop");
	}

	private void Call_RePlay()
	{
	GetJavaObject().Call("RePlay");
	}

	private void Call_Pause()
	{
	GetJavaObject().Call("Pause");
	}

	private int Call_InitNDK()
	{
	return GetJavaObject().Call<int>("InitNative",GetJavaObject());
	}

	private int Call_GetVideoWidth()
	{
	return GetJavaObject().Call<int>("GetVideoWidth");
	}

	private int Call_GetVideoHeight()
	{
	return GetJavaObject().Call<int>("GetVideoHeight");
	}

	private bool Call_IsUpdateFrame()
	{
	return GetJavaObject().Call<bool>("IsUpdateFrame");
	}

	private void Call_SetUnityTexture(int iTextureID)
	{
	GetJavaObject().Call("SetUnityTexture",iTextureID);
	}

	private void Call_SetWindowSize()
	{

	#if UNITY_5
	if( SystemInfo.graphicsMultiThreaded == true)
	{


	#if UNITY_5_2  || UNITY_5_3_OR_NEWER
	GL.IssuePluginEvent(EasyMovieTextureRender(), 2 + m_iAndroidMgrID * 10 + 7000);
	#else
	GL.IssuePluginEvent(2+ m_iAndroidMgrID * 10 + 7000);
	#endif
	}
	else
	{
	GetJavaObject().Call("SetWindowSize");
	}
	#else
	GetJavaObject().Call("SetWindowSize");
	#endif
	}

	private void Call_SetLooping(bool bLoop)
	{
	GetJavaObject().Call("SetLooping",bLoop);
	}

	private void Call_SetRockchip(bool bValue)
	{
	GetJavaObject().Call("SetRockchip",bValue);
	}


	private int Call_GetDuration()
	{
	return GetJavaObject().Call<int>("GetDuration");
	}

	private int Call_GetCurrentSeekPercent()
	{
	return GetJavaObject().Call<int>("GetCurrentSeekPercent");
	}

	private int Call_GetError()
	{
	return GetJavaObject().Call<int>("GetError");
	}

	private void Call_SetSplitOBB(bool bValue, string strOBBName)
	{
	GetJavaObject().Call("SetSplitOBB",bValue,strOBBName);
	}

	private int Call_GetErrorExtra()
	{
	return GetJavaObject().Call<int>("GetErrorExtra");
	}


	private void Call_SetUnityActivity()
	{
	AndroidJavaClass jc = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
	AndroidJavaObject jo = jc.GetStatic<AndroidJavaObject>("currentActivity");
	GetJavaObject().Call("SetUnityActivity", jo);

	#if UNITY_5
	if( SystemInfo.graphicsMultiThreaded == true)
	{
	#if UNITY_5_2  || UNITY_5_3_OR_NEWER
	GL.IssuePluginEvent(EasyMovieTextureRender(), 0 + m_iAndroidMgrID * 10 + 7000);
	#else
	GL.IssuePluginEvent(0+ m_iAndroidMgrID * 10 + 7000);
	#endif
	}
	else
	{
	Call_InitJniManager();
	}
	#else
	Call_InitJniManager();
	#endif

	}

	private void Call_SetNotReady()
	{
	GetJavaObject().Call("SetNotReady");
	}

	private void Call_InitJniManager()
	{
	GetJavaObject().Call("InitJniManager");
	}

	private void Call_SetSpeed(float fSpeed)
	{

	using (AndroidJavaClass buildVersion = new AndroidJavaClass("android.os.Build$VERSION"))
	{
	int sdkVersion = buildVersion.GetStatic<int>("SDK_INT");

	if(sdkVersion >= 23 ) //Android 6.0
	{

	GetJavaObject().Call("SetSpeed",fSpeed);

	}


	}




	}

	private MEDIAPLAYER_STATE Call_GetStatus()
	{
	return (MEDIAPLAYER_STATE)GetJavaObject().Call<int>("GetStatus");
	}



	#elif UNITY_IPHONE  || UNITY_TVOS
	[DllImport("__Internal")]
	private static extern int VideoPlayerPluginCreateInstance();
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginDestroyInstance(int iID);
	[DllImport("__Internal")]
	private static extern bool VideoPlayerPluginCanOutputToTexture(string videoURL);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginSetLoop(int iID, bool bLoop);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginSetVolume(int iID, float fVolume);
	[DllImport("__Internal")]
	private static extern bool VideoPlayerPluginPlayerReady(int iID);
	[DllImport("__Internal")]
	private static extern float VideoPlayerPluginDurationSeconds(int iID);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginExtents(int iID,ref int width, ref int height);
	[DllImport("__Internal")]
	private static extern IntPtr VideoPlayerPluginCurFrameTexture(int iID);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginLoadVideo(int iID,string videoURL);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginPlayVideo(int iID);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginPauseVideo(int iID);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginResumeVideo(int iID);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginRewindVideo(int iID);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginSeekToVideo(int iID,float time);
	[DllImport("__Internal")]
	private static extern float VideoPlayerPluginCurTimeSeconds(int iID);
	[DllImport("__Internal")]
	private static extern bool VideoPlayerPluginIsPlaying(int iID);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginStopVideo(int iID);
	[DllImport("__Internal")]
	private static extern bool VideoPlayerPluginFinish(int iID);
	[DllImport("__Internal")]
	private static extern bool VideoPlayerPluginError(int iID);
	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginSetTexture(int iID,int iTextureID);

	[DllImport("__Internal")]
	private static extern void VideoPlayerPluginSetSpeed(int iID,float fSpeed);





	int m_iID = -1;
	/// <summary>
	/// ???????????????true???????    /// </summary>
	public bool ready
	{
	get
	{
	return VideoPlayerPluginPlayerReady(m_iID);
	}
	}

	/// <summary>
	/// 
	/// </summary>
	public float duration
	{
	get
	{
	return VideoPlayerPluginDurationSeconds(m_iID);
	}
	}


	public float currentTime
	{
	get
	{
	return VideoPlayerPluginCurTimeSeconds(m_iID);
	}
	}


	public bool isPlaying
	{
	get
	{
	return VideoPlayerPluginIsPlaying(m_iID);
	}
	}
	public Vector2 videoSize
	{
	get
	{
	int width = 0, height = 0;
	VideoPlayerPluginExtents(m_iID,ref width, ref height);
	return new Vector2(width, height);
	}
	}

	private Texture2D _videoTexture;

	public Texture2D videoTexture
	{
	get
	{
	IntPtr nativeTex = ready ? VideoPlayerPluginCurFrameTexture(m_iID) : (IntPtr)0;


	if (nativeTex != (IntPtr)0)
	{
	if (_videoTexture == null)
	{
	#if UNITY_5
	if(SystemInfo.graphicsDeviceType == UnityEngine.Rendering.GraphicsDeviceType.Metal)
	_videoTexture = new Texture2D (Call_GetVideoWidth (), Call_GetVideoHeight (), TextureFormat.RGBA32, false);
	else
	_videoTexture = Texture2D.CreateExternalTexture((int)videoSize.x, (int)videoSize.y, TextureFormat.RGBA32,
	false, false, (IntPtr)nativeTex);
	#else
	_videoTexture = Texture2D.CreateExternalTexture((int)videoSize.x, (int)videoSize.y, TextureFormat.RGBA32,
	false, false, (IntPtr)nativeTex);
	#endif


	_videoTexture.filterMode = FilterMode.Bilinear;
	_videoTexture.wrapMode = TextureWrapMode.Clamp;
	}

	_videoTexture.UpdateExternalTexture((IntPtr)nativeTex);
	}
	/* else
	{
	if(_videoTexture != null)
	{
	Destroy(_videoTexture);
	}

	_videoTexture = null;
	}*/

	return _videoTexture;
	}
	}

	private void Call_Destroy()
	{

	VideoPlayerPluginDestroyInstance(m_iID);

	if(_videoTexture != null)
	Destroy(_videoTexture);

	_videoTexture = null;
	m_iID = -1;
	}

	private void Call_UnLoad()
	{

	VideoPlayerPluginStopVideo(m_iID);
	VideoPlayerPluginDestroyInstance(m_iID);
	}

	private bool Call_Load(string strFileName, int iSeek)
	{
	if( m_iID == -1)
	{
	Call_SetUnityActivity();
	}

	bFirstIOS = true;

	/*if(_videoTexture != null)
	{
	Destroy(_videoTexture);
	}*/

	_videoTexture = null;

	if (VideoPlayerPluginCanOutputToTexture(strFileName))
	{
	VideoPlayerPluginLoadVideo(m_iID,strFileName);	
	}




	return true;
	}


	bool bFirstIOS = false;
	private void Call_UpdateVideoTexture()
	{




	if( m_CurrentState == MEDIAPLAYER_STATE.PLAYING || m_CurrentState == MEDIAPLAYER_STATE.PAUSED)
	{

	if( videoTexture == null)
	return;


	if(bFirstIOS == true)
	{
	bFirstIOS = false;
	return;
	}




	if(bFirstIOS == false)
	{
	for( int i = 0; i < m_TargetMaterial.Length; i++)
	{
	if(m_TargetMaterial[i])
	{
	if(m_TargetMaterial[i].GetComponent<MeshRenderer>() != null)
	m_TargetMaterial[i].GetComponent<MeshRenderer>().material.mainTexture = _videoTexture;
	}

	if(m_TargetMaterial[i])
	{
	if(m_TargetMaterial[i].GetComponent<RawImage>() != null)
	m_TargetMaterial[i].GetComponent<RawImage>().texture= _videoTexture;
	}
	}

	if(m_VideoTextureDummy != null)
	{
	Destroy(m_VideoTextureDummy);
	m_VideoTextureDummy = null;
	}
	}


	m_VideoTexture = _videoTexture;
	}

	if (!m_bIsFirstFrameReady)
	{
	m_bIsFirstFrameReady = true;
	if (OnVideoFirstFrameReady != null)
	{
	OnVideoFirstFrameReady();
	//OnVideoFirstFrameReady = null;
	}
	}






	}

	private void Call_SetVolume(float fVolume)
	{
	VideoPlayerPluginSetVolume(m_iID, fVolume);
	}

	private void Call_SetSeekPosition(int iSeek)
	{
	float fSeek = (float)iSeek / 1000.0f;
	VideoPlayerPluginSeekToVideo(m_iID,fSeek);
	}

	private int Call_GetSeekPosition()
	{

	float fSeek = VideoPlayerPluginCurTimeSeconds(m_iID);
	return (int)(fSeek * 1000.0f);
	}

	private void Call_Play(int iSeek)
	{
	float fSeek = (float)iSeek / 1000.0f;

	if( isPlaying == true )
	{
	VideoPlayerPluginSeekToVideo(m_iID,fSeek);
	}
	else
	{
	if( m_CurrentState != MEDIAPLAYER_STATE.READY)
	VideoPlayerPluginSeekToVideo(m_iID,fSeek);

	VideoPlayerPluginPlayVideo(m_iID);
	}


	if( m_CurrentState == MEDIAPLAYER_STATE.STOPPED)
	m_CurrentState = MEDIAPLAYER_STATE.PLAYING;


	}

	private void Call_Reset()
	{

	}

	private void Call_Stop()
	{
	VideoPlayerPluginStopVideo(m_iID);
	}

	private void Call_RePlay()
	{
	VideoPlayerPluginResumeVideo(m_iID);
	m_CurrentState = MEDIAPLAYER_STATE.PLAYING;
	}

	private void Call_Pause()
	{
	m_CurrentState = MEDIAPLAYER_STATE.PAUSED;
	VideoPlayerPluginPauseVideo(m_iID);

	}

	private int Call_GetVideoWidth()
	{
	return (int)videoSize.x;
	}

	private int Call_GetVideoHeight()
	{
	return (int)videoSize.y;
	}

	private void Call_SetUnityTexture(int iTextureID)
	{
	VideoPlayerPluginSetTexture(m_iID,iTextureID);
	}

	private void Call_SetWindowSize()
	{

	}

	private void Call_SetLooping(bool bLoop)
	{
	VideoPlayerPluginSetLoop(m_iID, bLoop);
	}


	public void Call_SetUnityActivity()
	{
	m_iID = VideoPlayerPluginCreateInstance();
	}

	private int Call_GetError()
	{
	return 0;
	}

	private int Call_GetErrorExtra()
	{
	return 0;
	}

	private int Call_GetDuration()
	{
	return (int)(duration * 1000);
	}

	private int Call_GetCurrentSeekPercent()
	{
	return -1;
	}

	private void Call_SetSpeed(float fSpeed)
	{
	VideoPlayerPluginSetSpeed(m_iID,fSpeed);
	}

	private MEDIAPLAYER_STATE Call_GetStatus()
	{

	if( VideoPlayerPluginFinish(m_iID) )
	{
	return MEDIAPLAYER_STATE.END;
	}

	if( VideoPlayerPluginError(m_iID) )
	{
	return MEDIAPLAYER_STATE.ERROR;
	}

	if( m_CurrentState == MEDIAPLAYER_STATE.PAUSED || m_CurrentState == MEDIAPLAYER_STATE.STOPPED)
	{
	return m_CurrentState;
	}
	else if(isPlaying)
	{
	return MEDIAPLAYER_STATE.PLAYING;
	}
	else if(ready)
	{
	return MEDIAPLAYER_STATE.READY;
	}


	return m_CurrentState;
	}
	#endif
	#else


	AVFrame* pConvertedFrame = null;
	sbyte* pConvertedFrameBuffer = null;
	SwsContext* pConvertContext = null;
	AVCodecContext* pCodecContext = null;
	AVCodecContext* pAudioCodecContext = null;
	AVFrame* pDecodedFrame = null;
	AVFrame* pDecodedAudioFrame = null;
	AVFormatContext* pFormatContext = null;
	AVPacket* pPacket = null;
	AVStream* pStream = null;
	AVStream* pStreamAudio = null;

	int iStreamAudioIndex;
	int iStreamIndex;

	AudioClip audioClip;
	AudioSource audioSource;
	int iSoundCount = 0;
	int iInitCount = 0;


	double pts;

	bool bVideoFirstFrameReady = false;


	int m_iID;

	private void Call_Destroy()
	{

        if (loader != null) {
            while (loader.IsAlive == true) {
        
                loader.Abort ();
			}

			loader = null;
		}

		if( threadVideo != null)
		{
			while(threadVideo.IsAlive == true)
			{
				threadVideo.Abort();
			}

			threadVideo = null;
		}

        ffmpeg.avformat_network_deinit ();

		ReleaseTexture (m_iID);
	}

	private void Call_UnLoad()
	{
		m_CurrentState = MEDIAPLAYER_STATE.NOT_READY;
       

        if (loader != null) {
			while (loader.IsAlive == true) {
              
                loader.Abort ();
                
			}

			loader = null;
		}
        
	if( threadVideo != null)
		{
			while(threadVideo.IsAlive == true)
			{
				threadVideo.Abort();
   
            }



			threadVideo = null;
		}



        if (listAudio != null)
		{
			listAudio.Clear();
			listAudio = null;
		}


		if (listVideo != null)
		{
			listVideo.Clear();
			listVideo = null;
		}

		if (listAudioPts != null)
		{
			listAudioPts.Clear();
			listAudioPts = null;
		}

		if (listAudioPtsTime != null)
		{

			listAudioPtsTime.Clear();
			listAudioPtsTime = null;
		}

		if (listVideoPts != null)
		{
			listVideoPts.Clear();
			listVideoPts = null;
		}




		fCurrentSeekTime = 0.0f;
		fLastFrameTime = 0.0f;

		if (pPacket != null) 
		{
			ffmpeg.av_free_packet (pPacket);
			Marshal.FreeCoTaskMem((IntPtr)pPacket);
			pPacket = null;
		}


		if(pConvertedFrame != null)
		{
			ffmpeg.av_free(pConvertedFrame);
			pConvertedFrame = null;
		}

		if(pConvertedFrameBuffer != null)
		{
			ffmpeg.av_free(pConvertedFrameBuffer);
			pConvertedFrameBuffer = null;
		}

		if(pConvertContext != null)
		{
			ffmpeg.sws_freeContext(pConvertContext);
			pConvertContext = null;
		}

		if(pDecodedFrame != null)
		{
			ffmpeg.av_free(pDecodedFrame);
			pDecodedFrame = null;
		}

		if (pDecodedAudioFrame != null)
			ffmpeg.av_free (pDecodedAudioFrame);

		pDecodedAudioFrame = null;


		if(pCodecContext != null)
			ffmpeg.avcodec_close(pCodecContext);

		pCodecContext = null;

		if (pAudioCodecContext != null)
			ffmpeg.avcodec_close (pAudioCodecContext);

		pAudioCodecContext = null;


		if (pFormatContext != null) {
			AVFormatContext* ppFormatContext = pFormatContext;
			ffmpeg.avformat_close_input (&ppFormatContext);
		}

		pFormatContext = null;

		if (audioSource != null) {
			audioSource.Stop();
		}

		if ( audioClip != null)
		{
			Destroy (audioClip);
			audioClip = null;
		}



	}

	Thread loader = null;

	private unsafe bool Call_Load(string strFileName, int iSeek)
	{

		fCurrentSeekTime = 0.0f;
		fLastFrameTime = 0.0f;
		iSoundCount = 0;
		iInitCount= 0;
		bSeekTo = true;
		bEnd = false;

		if (audioSource != null) {
			audioSource.Stop();
			audioSource.time = 0.0f;
		}

		if ( audioClip != null)
		{
			Destroy (audioClip);
			audioClip = null;
		}

		pFormatContext = ffmpeg.avformat_alloc_context();

		if( strFileName.Contains("://") == false)
		{
			strFileName = Application.streamingAssetsPath + "/" + strFileName;
			Debug.Log(strFileName);
		}
		else if( strFileName.Contains("file://") == true)
		{
			strFileName = strFileName.Replace("file://", "");
		}

 
        loader = new Thread ( () => {


			AVFormatContext* ppFomatContext = null;
			if (ffmpeg.avformat_open_input(&ppFomatContext, strFileName, null, null) != 0)
			{
				pFormatContext = null;
				m_CurrentState = MEDIAPLAYER_STATE.ERROR;
                Debug.Log("Could not open file");
                throw new ApplicationException(@"Could not open file");
			}

            //ffmpeg.av_dict_free(&opts);

            pFormatContext = ppFomatContext;



			if (ffmpeg.avformat_find_stream_info(pFormatContext, null) != 0)
			{
				m_CurrentState = MEDIAPLAYER_STATE.ERROR;
                Debug.Log("Could not find stream info");
                throw new ApplicationException(@"Could not find stream info");
			}
    
            AddActionForUnityMainThread( () => {
				LoadVideoPart2 ();
			});

            


        });

		loader.Priority = System.Threading.ThreadPriority.AboveNormal;
		loader.IsBackground = true;
		loader.Start ();

		return true;
	}


	void LoadVideoPart2 ()
	{

		pStream = null;
		pStreamAudio = null;

		bool bFindVideo = false;


		for (var i = 0; i < (pFormatContext)->nb_streams; i++)
		{
			if ((pFormatContext)->streams[i]->codec->codec_type == AVMediaType.AVMEDIA_TYPE_VIDEO)
			{
				if(bFindVideo == false)
				{
					bFindVideo = true;
					pStream = (pFormatContext)->streams[i];
					iStreamIndex = i;

					Debug.Log("Video" +  iStreamIndex);
				}

			}
			else if((pFormatContext)->streams[i]->codec->codec_type == AVMediaType.AVMEDIA_TYPE_AUDIO)
			{
				pStreamAudio = (pFormatContext)->streams[i];
				iStreamAudioIndex = i;

			}
		}


		if (pStream == null)
		{
			m_CurrentState = MEDIAPLAYER_STATE.ERROR;
            Debug.Log("Could not found video stream");
            throw new ApplicationException(@"Could not found video stream");
		}

		if (pStreamAudio == null)
		{
			m_CurrentState = MEDIAPLAYER_STATE.ERROR;
			//throw new ApplicationException(@"Could not found audio stream");
		}

		var codecContext = *pStream->codec;

		m_iWidth = codecContext.width;
		m_iHeight = codecContext.height;
		var sourcePixFmt = codecContext.pix_fmt;
		var codecId = codecContext.codec_id;
		var convertToPixFmt = AVPixelFormat.AV_PIX_FMT_RGBA;

	#if UNITY_5
		if( SystemInfo.graphicsDeviceType == UnityEngine.Rendering.GraphicsDeviceType.Direct3D9)
		{
			convertToPixFmt = AVPixelFormat.AV_PIX_FMT_BGRA;
		}
	#endif


		pConvertContext = ffmpeg.sws_getContext(m_iWidth, m_iHeight, sourcePixFmt,
			m_iWidth, m_iHeight, convertToPixFmt,
			ffmpeg.SWS_FAST_BILINEAR, null, null, null);
		if (pConvertContext == null)
		{
			m_CurrentState = MEDIAPLAYER_STATE.ERROR;
            Debug.Log("Could not initialize the conversion context");
            throw new ApplicationException(@"Could not initialize the conversion context");
		}

		//pConvertedFrame = ffmpeg.avcodec_alloc_frame();
		pConvertedFrame = ffmpeg.av_frame_alloc();
		var convertedFrameBufferSize = ffmpeg.avpicture_get_size(convertToPixFmt, m_iWidth, m_iHeight);
		pConvertedFrameBuffer = (sbyte*)ffmpeg.av_malloc((ulong)convertedFrameBufferSize);
		AVPicture* tempPicture = (AVPicture*)pConvertedFrame;
		ffmpeg.avpicture_fill(tempPicture , pConvertedFrameBuffer, convertToPixFmt, m_iWidth, m_iHeight);

		var pCodec = ffmpeg.avcodec_find_decoder(codecId);
		if (pCodec == null)
		{
			m_CurrentState = MEDIAPLAYER_STATE.ERROR;
            Debug.Log("Unsupported codec");
            throw new ApplicationException(@"Unsupported codec");
		}


		pCodecContext = pStream->codec;
		pCodecContext->hwaccel = ff_find_hwaccel(pCodecContext->codec_id ,sourcePixFmt);

		//if ((pCodec->capabilities & ffmpeg.AV_CODEC_CAP_TRUNCATED) == ffmpeg.AV_CODEC_CAP_TRUNCATED)
		{
			//pCodecContext->flags = ffmpeg.AV_CODEC_FLAG_TRUNCATED;
		}

		if (ffmpeg.avcodec_open2(pCodecContext, pCodec, null) < 0)
		{
			m_CurrentState = MEDIAPLAYER_STATE.ERROR;
            Debug.Log("Could not open codec");
			throw new ApplicationException(@"Could not open codec");
		}


		if(pStreamAudio != null)
		{
			var codecAudioContext = *pStreamAudio->codec;

			var pAudioCodec = ffmpeg.avcodec_find_decoder(codecAudioContext.codec_id);
			if (pAudioCodec == null) {
				m_CurrentState = MEDIAPLAYER_STATE.ERROR;
                Debug.Log("Unsupported codec");
                throw new ApplicationException(@"Unsupported codec");
			}

			pAudioCodecContext = pStreamAudio->codec;
			///> Initialize Codec Context as Decoder
			if (ffmpeg.avcodec_open2(pAudioCodecContext, pAudioCodec, null) < 0) {
				m_CurrentState = MEDIAPLAYER_STATE.ERROR;
                Debug.Log("Could not open codec");
                throw new ApplicationException(@"Could not open codec");
			}



		}

		pDecodedFrame = ffmpeg.av_frame_alloc();
		pDecodedAudioFrame = ffmpeg.av_frame_alloc();


		listAudio = new List<float[]>();
		listVideo = new Queue<byte[]>();
		listAudioPts = new List<double>();
		listAudioPtsTime = new List<double>();
		listVideoPts = new Queue<float>();



		if( m_strFileName.StartsWith("rtsp",StringComparison.OrdinalIgnoreCase) == true)
		{
		}
		else
		{
			action = Interrupt1;
			//pFormatContext->interrupt_callback.callback = Marshal.GetFunctionPointerForDelegate(action);
		}


		bVideoFirstFrameReady = false;

		threadVideo = new Thread(ThreadUpdate);
		threadVideo.IsBackground = true;
		threadVideo.Start();

		if (m_bAutoPlay == true) 
		{
			if( OnReady != null)
				OnReady();
			m_CurrentState = MEDIAPLAYER_STATE.PLAYING;

		}
		else
		{

			m_CurrentState = MEDIAPLAYER_STATE.READY;

			if( OnReady != null)
				OnReady();

		}

        loader = null;

    }

	AVHWAccel *ff_find_hwaccel( AVCodecID codec_id,  AVPixelFormat pix_fmt)
	{
		AVHWAccel *hwaccel=null;

		while((hwaccel= ffmpeg.av_hwaccel_next(hwaccel))!=null){
			if (   hwaccel->id      == codec_id
				&& hwaccel->pix_fmt == pix_fmt)
				return hwaccel;
		}
		return null;
	}

	public delegate void VideoInterrupt();
	VideoInterrupt action = null;
	bool bInterrupt = false;


	public void Interrupt1()
	{
		pFormatContext->interrupt_callback.callback = (IntPtr)null;
		bInterrupt = true;

		//Debug.Log ("Interrupt1");


		//Call_Pause ();


	}


	static double av_q2d(AVRational a){
		return a.num / (double) a.den;
	}


	float fLastFrameTime = 0.0f;
	float fCurrentSeekTime = 0.0f;

	float[] fAudioData;
	bool bEnd = false;


	Thread threadVideo;

	List<float[]> listAudio;
	Queue<byte[]> listVideo;
	List<double> listAudioPts;
	List<double> listAudioPtsTime;
	Queue<float> listVideoPts;


	private static void DebugMethod(string message)
	{
		Debug.Log("EasyMovieTexture: " + message);
	}


	private void ThreadUpdate()
	{



		/*lock (listVideo)
        {
            if (listVideo != null)
            {
                listVideo.Clear();
            }
        }

        lock (listVideoPts)
        {
            if (listVideoPts != null)
            {
                listVideoPts.Clear();
            }
        }*/


		while (true)
		{
            if (m_CurrentState == MEDIAPLAYER_STATE.ERROR)
                break;

			if (listVideo != null)
			{
				while (listVideo.Count >30 || bEnd == true )
				{
					Thread.Sleep(5);

             
				}
			}




			// if( m_CurrentState == MEDIAPLAYER_STATE.PLAYING)






			UpdateVideo();
			Thread.Sleep(1);



			//Thread.Sleep(5);
		}
	}


	private void UpdateVideo()
	{

		var gotPicture = 0;
		var gotSound = 0;


		if( m_CurrentState != MEDIAPLAYER_STATE.PAUSED)
		{

			if (pPacket != null) 
			{

				if (pPacket->stream_index == iStreamIndex) {



					var size = ffmpeg.avcodec_decode_video2 (pCodecContext, pDecodedFrame, &gotPicture, pPacket);
					if (size < 0) {
                        //throw new ApplicationException (string.Format (@"Error while decoding frame "));

                        Debug.Log("Error while decoding frame");
                        return;
                   
                    }






					if ((ulong)pPacket->dts != ffmpeg.AV_NOPTS_VALUE) {
						pts = ffmpeg.av_frame_get_best_effort_timestamp (pDecodedFrame);
					} else {
						pts = 0;
					}

					pts *= av_q2d (pStream->time_base);

					if (gotPicture == 1) {
						if (pts > 0) {
							if (listVideo.Count > 5)
							{
								if (!m_bIsFirstFrameReady) {

									//Debug.Log("FistReady"  + listVideo.Count);
									m_bIsFirstFrameReady = true;


									bVideoFirstFrameReady = true;

								}
							}

						}

						//Debug.Log ("Video " + pts);

						//fLastFrameTime = (float)pts;


						sbyte** src = &pDecodedFrame->data0;
						sbyte** dst = &pConvertedFrame->data0;
						int* srcStride = pDecodedFrame->linesize;
						int* dstStride = pConvertedFrame->linesize;

						int iLineSize = srcStride[0] ;
						int iLineSize1 = srcStride[1] ;
						int iLineSize2 = srcStride[2] ;

						if (m_bPC_FastMode == true) {
							


							int iVideoSize = m_iWidth * m_iHeight + (2 * m_iWidth / 2 * m_iHeight / 2);
							byte[] buffer = new byte[iVideoSize];



							if (iLineSize != m_iWidth) {

								sbyte* convertedFrameAddress;
								IntPtr imageBufferPtr;
								for (int i = 0; i < m_iHeight; i++) {
									convertedFrameAddress = pDecodedFrame->data0 + iLineSize * i;
									imageBufferPtr = new IntPtr (convertedFrameAddress);
									Marshal.Copy(imageBufferPtr,buffer ,m_iWidth * i,m_iWidth );


								}


								for (int i = 0; i < m_iHeight / 2; i++) {
									convertedFrameAddress = pDecodedFrame->data1 + iLineSize1 * i;
									imageBufferPtr = new IntPtr (convertedFrameAddress);
									Marshal.Copy(imageBufferPtr,buffer,m_iWidth * m_iHeight + m_iWidth / 2 * i ,m_iWidth / 2);

									convertedFrameAddress = pDecodedFrame->data2 + iLineSize2 * i;
									imageBufferPtr = new IntPtr (convertedFrameAddress);
									Marshal.Copy(imageBufferPtr ,buffer,m_iWidth * m_iHeight+m_iWidth / 2 * m_iHeight / 2 + m_iWidth / 2 * i,m_iWidth / 2 );


								}



							} else {
								sbyte* convertedFrameAddress = pDecodedFrame->data0;
								var imageBufferPtr = new IntPtr (convertedFrameAddress);

								Marshal.Copy(imageBufferPtr,buffer,0,m_iWidth * m_iHeight);

								convertedFrameAddress = pDecodedFrame->data1;
								imageBufferPtr = new IntPtr (convertedFrameAddress);
								Marshal.Copy(imageBufferPtr,buffer,m_iWidth * m_iHeight,m_iWidth / 2 * m_iHeight / 2);

								convertedFrameAddress = pDecodedFrame->data2;
								imageBufferPtr = new IntPtr (convertedFrameAddress);
								Marshal.Copy(imageBufferPtr,buffer,m_iWidth * m_iHeight+m_iWidth / 2 * m_iHeight / 2,m_iWidth / 2 * m_iHeight / 2);
							}


							lock (listVideo)
							{
								listVideo.Enqueue(buffer);
								lock (listVideoPts)
								{
									listVideoPts.Enqueue((float)pts);
                                  
                                        

                                }



							}

						} else {
							ffmpeg.sws_scale (pConvertContext, src, srcStride, 0, m_iHeight, dst, dstStride);

							sbyte* convertedFrameAddress = pConvertedFrame->data0;
							var imageBufferPtr = new IntPtr (convertedFrameAddress);

							byte[] buffer = new byte[4* m_iWidth *m_iHeight];
							Marshal.Copy(imageBufferPtr,buffer,0,4* m_iWidth *m_iHeight);

							lock (listVideo)
							{
								listVideo.Enqueue(buffer);
								lock (listVideoPts)
								{
									listVideoPts.Enqueue((float)pts);
								}



							}
						}








					}
				}
			} else {
				pPacket = (AVPacket*)Marshal.AllocCoTaskMem(sizeof(AVPacket));

				ffmpeg.av_init_packet(pPacket);
			}

			do{

				if (pPacket != null) 
				{
					ffmpeg.av_free_packet (pPacket);
				}

				int ret  = ffmpeg.av_read_frame( pFormatContext, pPacket);

				if( bInterrupt == true && listVideo.Count > 20 )
				{
					//Debug.Log ("11");
					bInterrupt = false;
					pFormatContext->interrupt_callback.callback = Marshal.GetFunctionPointerForDelegate(action);
				}

				if ( ret < 0)
				{

					if( ret == -541478725)
					{

						if(listVideo.Count < 3)
						{
							bEnd= true;

							//threadVideo.Abort();

							return;
						}
					}
					else
					{   
						//throw new ApplicationException(@"Could not read frame");
                        Debug.Log("Could not read frame");
                    }



				}




				// Debug.Log(pPacket->pts + " " + pCodecContext->pts_correction_last_pts );


				if( pStreamAudio != null)
				{
					if(pPacket->stream_index == iStreamAudioIndex)
					{
						int iAudioLen = ffmpeg.avcodec_decode_audio4(pAudioCodecContext, pDecodedAudioFrame, &gotSound, pPacket);
						if (iAudioLen >= 0) {
							if (gotSound == 1) {






								int iDataSize = ffmpeg.av_samples_get_buffer_size(null,pAudioCodecContext->channels,pDecodedAudioFrame->nb_samples,pAudioCodecContext->sample_fmt,1);
								int iDataSize2 = ffmpeg.av_samples_get_buffer_size(null,pAudioCodecContext->channels,pDecodedAudioFrame->nb_samples,AVSampleFormat.AV_SAMPLE_FMT_FLT,1);

								//if( pAudioCodecContext->sample_fmt != AVSampleFormat.AV_SAMPLE_FMT_FLT)
								{
									//for(int i = 0; i < pAudioCodecContext->channels; i++)
									{
										sbyte* outData =  (sbyte*)Marshal.AllocCoTaskMem(iDataSize2 * sizeof(sbyte));;


										SwrContext* pAudioCvtContext = null;

                                        if (pAudioCodecContext->channel_layout == 0)
                                        {
                                            pAudioCvtContext = ffmpeg.swr_alloc_set_opts(null, (long)3, AVSampleFormat.AV_SAMPLE_FMT_FLT, pAudioCodecContext->sample_rate
                                            , (long)3, pAudioCodecContext->sample_fmt, pAudioCodecContext->sample_rate, 0, (void*)0);
                                        }
                                        else
                                        {
                                            pAudioCvtContext = ffmpeg.swr_alloc_set_opts(null, (long)pAudioCodecContext->channel_layout, AVSampleFormat.AV_SAMPLE_FMT_FLT, pAudioCodecContext->sample_rate
                                            , (long)pAudioCodecContext->channel_layout, pAudioCodecContext->sample_fmt, pAudioCodecContext->sample_rate, 0, (void*)0);
                                        }
										

										int error = 0;

                                        
                                        if ((error = ffmpeg.swr_init(pAudioCvtContext)) < 0) {
											Debug.Log ("error " + error);

                                            
                                        }

                                       

                                        ffmpeg.swr_convert(pAudioCvtContext,&outData,iDataSize2,pDecodedAudioFrame->extended_data,pDecodedAudioFrame->nb_samples );






										sbyte* soundFrameAddress = outData;

										var soundBufferPtr = new IntPtr (soundFrameAddress);


										byte[] buffer = new byte[iDataSize2  ];
										Marshal.Copy(soundBufferPtr,buffer,0,iDataSize2);



										if ((ulong)pPacket->dts != ffmpeg.AV_NOPTS_VALUE) 
										{
											pts = ffmpeg.av_frame_get_best_effort_timestamp(pDecodedAudioFrame);
										}
										else
										{
											pts = 0;
										}

										pts *= av_q2d(pStreamAudio->time_base);


										if( bSeekTo == true)
										{
											double value = pts * (double)pDecodedAudioFrame->sample_rate / ((double)iDataSize2 / 4 / pDecodedAudioFrame->channels );

											//Debug.Log(value + " " + pts + " " + GetDuration() + " " + pDecodedAudioFrame->pkt_duration);

											iSoundCount = (int)value;

                                            fCurrentSeekTime = (float)pts;
                                            bSeekTo = false;
                                        }

										while( pts > 600.0f * (iInitCount + 1))
										{
											iSoundCount -=  (int)(600.0f * (double)pDecodedAudioFrame->sample_rate / ((double)iDataSize2 / 4 / pDecodedAudioFrame->channels));
											iInitCount++;;
										}

										//Debug.Log ("sound " + iSoundCount);
										//Debug.Log (pDecodedAudioFrame->pkt_dts + " " +pDecodedAudioFrame->pkt_duration + " "  + pDecodedAudioFrame->pkt_pos + " " + pDecodedAudioFrame->pkt_pts + " " + pts);


										fAudioData = new float[buffer.Length / 4];
										Buffer.BlockCopy(buffer, 0, fAudioData, 0, buffer.Length);




										lock( listAudio)
										{
											listAudio.Add(fAudioData);
											lock(listAudioPts)
											{ 
												lock(listAudioPtsTime)
												{
													//Debug.Log(pDecodedAudioFrame->pkt_pts % pDecodedAudioFrame->pkt_duration + " " + iDataSize2);
													//listAudioPts.Add(iSoundCount++ * iDataSize2 / 4 / pDecodedAudioFrame->channels );

													/*if( m_strFileName.Contains(".m3u8") )
														{
															listAudioPts.Add(pDecodedAudioFrame->pkt_pts / pDecodedAudioFrame->pkt_duration *iDataSize2 / 4 / pDecodedAudioFrame->channels   );
														}
														else*/
													{
														listAudioPts.Add(iSoundCount++ * iDataSize2 / 4 / pDecodedAudioFrame->channels );
													}

													listAudioPtsTime.Add(pts);
												}

											}
										}



										//Debug.Log ( iDataSize + " "+ pts + " " + pAudioCodecContext->sample_rate + " " + pDecodedAudioFrame->sample_rate + " "+ pDecodedAudioFrame->nb_samples + " " + pDecodedAudioFrame->pkt_pts + " " + pDecodedAudioFrame->pkt_pos);
										//Debug.Log ("sound decode time " + pts);
										//audioClip.SetData(fAudioData,(int)(pAudioCodecContext->sample_rate * pts )   );
										//audioClip.SetData(fAudioData,(int)(pDecodedAudioFrame->pkt_pts )   );


										ffmpeg.swr_free(&pAudioCvtContext);

										Marshal.FreeCoTaskMem((IntPtr)outData);
									}

								}
								/*else
								{
                                   
                                    sbyte* soundFrameAddress = pDecodedAudioFrame->extended_data[0];

									var soundBufferPtr = new IntPtr (soundFrameAddress);


									byte[] buffer = new byte[iDataSize];
									Marshal.Copy(soundBufferPtr,buffer,0,iDataSize);



									if ((ulong)pPacket->dts != ffmpeg.AV_NOPTS_VALUE) 
									{
										pts = ffmpeg.av_frame_get_best_effort_timestamp(pDecodedAudioFrame);
									}
									else
									{
										pts = 0;
									}

									pts *= av_q2d(pStreamAudio->time_base);



                                    //Debug.Log (pDecodedAudioFrame->pkt_dts + " " +pDecodedAudioFrame->pkt_duration + " "  + pDecodedAudioFrame->pkt_pos + " " + pDecodedAudioFrame->pkt_pts + " " + pts);

                                    if (bSeekTo == true)
                                    {
                                        double value = pts * (double)pDecodedAudioFrame->sample_rate / ((double)iDataSize2 / 4 / pDecodedAudioFrame->channels);

                                        //Debug.Log(value + " " + pts + " " + GetDuration() + " " + pDecodedAudioFrame->pkt_duration);

                                        iSoundCount = (int)value;

                                        fCurrentSeekTime = (float)pts;
                                        bSeekTo = false;
                                    }

                                    while (pts > 600.0f * (iInitCount + 1))
                                    {
                                        iSoundCount -= (int)(600.0f * (double)pDecodedAudioFrame->sample_rate / ((double)iDataSize2 / 4 / pDecodedAudioFrame->channels));
                                        iInitCount++; ;
                                    }


                                    fAudioData = new float[buffer.Length / 4];
                                    Buffer.BlockCopy(buffer, 0, fAudioData, 0, buffer.Length);

                                    lock ( listAudio)
									{
										listAudio.Add(fAudioData);
										lock(listAudioPts)
										{
											lock(listAudioPts)
											{
												listAudioPts.Add(pts);
												listAudioPtsTime.Add(pts);
											}

										}
									}


									//Debug.Log ( pDecodedAudioFrame->channels + " "+buffer.Length / 4 + " " + pDecodedAudioFrame->pkt_pts + " " + pDecodedAudioFrame->pkt_pos);
									//Debug.Log ("sound decode time " + pts);

								}*/

							}
						}
					}
				}



			}while(pPacket->stream_index != iStreamIndex);





		}




	}














	int  iSoundBufferCount = 0;

	/*void OnAudioSetPosition(int newPosition)
	{
	//Debug.Log (newPosition);

	//Debug.Log("Pts" +  listAudioPtsTime[0]);
	}*/

	void OnAudioRead(float[] data)
	{
		//Debug.Log (listAudio.Count);

		if (listAudio.Count < 3)
		{
			return;
		}
		//Debug.Log (data.Length + " " + listAudio[1].Length);

		if (iSoundBufferCount == 0) {
			iSoundBufferCount = listAudio[0].Length;
		}


		if (iSoundBufferCount < data.Length) {
			Array.Copy (listAudio [0],  listAudio [0].Length - iSoundBufferCount, data, 0, iSoundBufferCount);


			//Debug.Log (iSoundBufferCount + " " + data.Length + " " + listAudio [1].Length);
			if ((data.Length - iSoundBufferCount) > listAudio [1].Length) {
				Array.Copy (listAudio [1], 0, data, 0, listAudio [1].Length);
				Array.Copy (listAudio [2], 0, data, iSoundBufferCount + listAudio [1].Length, data.Length - iSoundBufferCount - listAudio [1].Length);
				iSoundBufferCount = listAudio[2].Length - (data.Length - iSoundBufferCount  - listAudio [1].Length);
				listAudio.RemoveAt (0);
				listAudioPts.RemoveAt (0);
				listAudioPtsTime.RemoveAt (0);

			} else {
				Array.Copy (listAudio [1], 0, data, iSoundBufferCount, data.Length - iSoundBufferCount);
				iSoundBufferCount = listAudio[1].Length - (data.Length - iSoundBufferCount);
			}


			listAudio.RemoveAt (0);
			listAudioPts.RemoveAt (0);
			listAudioPtsTime.RemoveAt (0);



		} else {
			Array.Copy (listAudio [0], listAudio [0].Length - iSoundBufferCount, data,0, data.Length);
			iSoundBufferCount -= data.Length;
		}

		if (iSoundBufferCount == 0) {
			listAudio.RemoveAt (0);
			listAudioPts.RemoveAt (0);
			listAudioPtsTime.RemoveAt (0);

			iSoundBufferCount = listAudio[0].Length;
		}

	}



	private int m_LastAudioPartSetted = -1;

	private void Call_UpdateVideoTexture()
	{
        

		if (bEnd == true && listVideo.Count == 0 )
		{

			m_CurrentState = MEDIAPLAYER_STATE.END;

			if (OnEnd != null)
			{
				OnEnd();

				//return;
			}


			if (m_bLoop == true)
			{
				Destroy (audioClip);
				audioClip = null;
				Load (m_strFileName);
				bEnd = false;
				return;
			}

			bEnd = false;



			return;
		}


		if (bInterrupt == true)
		{
			if (audioSource != null)
			{
				audioSource.Pause();
			}
		}
		else
		{
			if (audioSource != null && m_CurrentState == MEDIAPLAYER_STATE.PLAYING && m_bIsFirstFrameReady == true)
			{
				if( audioSource.isPlaying == false/* && audioSource.time > 0.02f*/)
					audioSource.Play();
			}
		}





        if (threadVideo == null && m_CurrentState != MEDIAPLAYER_STATE.END && m_CurrentState != MEDIAPLAYER_STATE.NOT_READY && m_CurrentState != MEDIAPLAYER_STATE.ERROR)
        {
            threadVideo = new Thread(ThreadUpdate);
            threadVideo.IsBackground = true;
            threadVideo.Start();
        }




        if ( m_CurrentState == MEDIAPLAYER_STATE.PLAYING && m_bIsFirstFrameReady == true && bInterrupt == false )
		{
			if( listVideo.Count > 0 )
				fCurrentSeekTime += Time.deltaTime * m_fSpeed;
		}





		//Debug.Log("last " + fLastFrameTime + " " + fCurrentSeekTime + " " + listVideo.Count);

		



        if (Mathf.Abs(fLastFrameTime - fCurrentSeekTime) < 0.1f)
        {


			for (int i = 0; i < listAudio.Count; i++)
			{

				if( listAudioPtsTime.Count > i)
				{
					if(audioSource == null && (int)((float)pAudioCodecContext->sample_rate * ((float)listAudioPtsTime[i] + ((float)Call_GetDuration() / 1000.0f))) > 0)
					{
						audioSource = gameObject.AddComponent<AudioSource>();
						audioSource.volume = m_fVolume;
					}
				}

				if(audioClip == null && audioSource != null )
				{

					/*if ((float)Call_GetDuration() <= 0)
                    {
						audioClip = AudioClip.Create("videoAudio",(int)((float)pAudioCodecContext->sample_rate * ((float)listAudioPtsTime[i] + 600.0f)),pAudioCodecContext->channels,pAudioCodecContext->sample_rate,true,OnAudioRead);
                    }
                    else*/
					{

	#if UNITY_5
						audioClip = AudioClip.Create("videoAudio",(int)((float)pAudioCodecContext->sample_rate * 600.0f),pAudioCodecContext->channels,pAudioCodecContext->sample_rate,false);


	#else
	audioClip = AudioClip.Create("videoAudio",(int)((float)pAudioCodecContext->sample_rate * 600.0f),pAudioCodecContext->channels,pAudioCodecContext->sample_rate,false,false);


	#endif
					}




					audioSource.clip = audioClip;
				}

				if (audioSource != null  && Call_GetDuration() >0)
				{


					if( listAudioPts.Count > i)
					{
						if (listAudioPts [i] >= 0) {

							{
								/*if( listAudioPts[i] > (int)((float)pAudioCodecContext->sample_rate * ((float)listAudioPtsTime[i] + ((float)Call_GetDuration() / 1000.0f))))
								{
									audioClip.SetData(listAudio[i],(int)(((double)pAudioCodecContext->sample_rate) * listAudioPtsTime[i]  )   );
								}
								else*/
								{
									//Debug.Log(audioSource.time  + " " + (float)listAudioPtsTime[i]  + " " +listAudioPts [i]   +" " +  fLastFrameTime);

									audioClip.SetData(listAudio[i],(int)(listAudioPts[i] %  (pAudioCodecContext->sample_rate * 600.0f))  );

								}
							}
						}
					}



				}



			}

			if (audioSource != null && audioSource.isPlaying  && Call_GetDuration() >0) {
				listAudio.Clear();
				listAudioPts.Clear();
				listAudioPtsTime.Clear();
			}



		}
		else
		{


			//do
			{
	#if (UNITY_5_2 || UNITY_5_3_OR_NEWER)

				if( listVideo.Count > 0)
				{

                    if (m_bPC_FastMode == true)
                    {
                        if (SystemInfo.graphicsDeviceType == UnityEngine.Rendering.GraphicsDeviceType.Direct3D11)
                        {
                            m_texPtrY = m_VideoTextureY.GetNativeTexturePtr();
                            m_texPtrU = m_VideoTextureU.GetNativeTexturePtr();
                            m_texPtrV = m_VideoTextureV.GetNativeTexturePtr();
                        }
                    
                            

                        SetTextureFromUnity (m_iID,(IntPtr)0,m_texPtrY,m_texPtrU,m_texPtrV, m_iWidth, m_iHeight, listVideo.Dequeue());
						GL.IssuePluginEvent (GetRenderEventFunc (), 7000 + m_iID);


					}
					else
					{

                        if (SystemInfo.graphicsDeviceType == UnityEngine.Rendering.GraphicsDeviceType.Direct3D11)
                        {
                            m_texPtr = m_VideoTexture.GetNativeTexturePtr();
                        }
                        

                        SetTextureFromUnity (m_iID,m_texPtr,(IntPtr)0,(IntPtr)0,(IntPtr)0, m_iWidth, m_iHeight, listVideo.Dequeue());
						GL.IssuePluginEvent (GetRenderEventFunc (), 7000 + m_iID);
					}



				}

				if( listVideoPts.Count > 0)
				{
					float fpts =  listVideoPts.Dequeue();


                    if (fpts > fCurrentSeekTime)
                    {
                        fCurrentSeekTime = fpts;
                    }
                    
                    if (fLastFrameTime == 0)
					{

					 
						{
							if(fpts<0)
							{
								fLastFrameTime = 0;

							}
							else
							{
								fCurrentSeekTime = fpts;
								fLastFrameTime = fpts;
							}


							if( audioSource != null && Call_GetDuration() >0 )
							{
								audioSource.time = fLastFrameTime%600.0f;

							}
						}
					}
					else
					{
						if(fpts<=0)
							fLastFrameTime = fCurrentSeekTime - 0.05f;
						else
							fLastFrameTime = fpts;

                  
					}

				}

				if( audioSource != null &&  Call_GetDuration() > 0)
				{
					if( audioSource.time - fLastFrameTime%600.0f > 0.1f)
					{
						//Debug.Log("sync1 " + audioSource.time + " " + fLastFrameTime);
						audioSource.time = fLastFrameTime%600.0f;
					}
					else if( audioSource.time - fLastFrameTime%600.0f < -0.1f)
					{
						//Debug.Log("sync2");
						audioSource.time = fLastFrameTime%600.0f;
					}


				}

#else
	//var imageBufferPtr = new IntPtr (convertedFrameAddress);
	if( listVideo.Count > 0)
	{
	m_VideoTexture.LoadRawTextureData(listVideo.Dequeue());
	m_VideoTexture.Apply();
	}

	if( listVideoPts.Count > 0)
	{
	float fpts =  listVideoPts.Dequeue();

	if(fLastFrameTime == 0)
	{
	if( fpts > fCurrentSeekTime)
	{
	fLastFrameTime = fCurrentSeekTime;
	}
	}
	else
	{
	fLastFrameTime = fpts;
	}

	}

	if( audioSource != null)
	{
	/*if( audioSource.time - fLastFrameTime > 0)
	{
	//Debug.Log("sync1");
	audioSource.time = fLastFrameTime;
	}
	else if( audioSource.time - fLastFrameTime < -0.2f)
	{
	//Debug.Log("sync2");
	audioSource.time = fLastFrameTime;
	}*/
	}
#endif


				Material material = new Material(m_shaderYUV);

                if (m_TargetMaterial != null) {
					for (int i = 0; i < m_TargetMaterial.Length; i++) {
						if (m_TargetMaterial [i] == null)
							continue;

                        


                        if (m_TargetMaterial [i].GetComponent<MeshRenderer> () != null) {

                            if (m_TargetMaterial [i].GetComponent<MeshRenderer> ().material.mainTexture != m_VideoTexture) {
								m_TargetMaterial [i].GetComponent<MeshRenderer> ().material.mainTexture = m_VideoTexture;
							}
						}

						if (m_bPC_FastMode == true) {

							if (m_TargetMaterial [i].GetComponent<MeshRenderer> () != null) {
                                m_TargetMaterial[i].GetComponent<MeshRenderer>().material = material;

                                if (m_TargetMaterial [i].GetComponent<MeshRenderer> ().material.GetTexture("_Y") != m_VideoTextureY) {
									m_TargetMaterial [i].GetComponent<MeshRenderer> ().material.SetTexture("_Y", m_VideoTextureY);
								}

								if (m_TargetMaterial [i].GetComponent<MeshRenderer> ().material.GetTexture("_U") != m_VideoTextureU) {
									m_TargetMaterial [i].GetComponent<MeshRenderer> ().material.SetTexture("_U", m_VideoTextureU);
								}

								if (m_TargetMaterial [i].GetComponent<MeshRenderer> ().material.GetTexture("_V") != m_VideoTextureV) {
									m_TargetMaterial [i].GetComponent<MeshRenderer> ().material.SetTexture("_V", m_VideoTextureV);
								}
							}

						
						}


						if (m_TargetMaterial [i].GetComponent<RawImage> () != null) {
							if (m_TargetMaterial [i].GetComponent<RawImage> ().texture != m_VideoTexture) {
								m_TargetMaterial [i].GetComponent<RawImage> ().texture = m_VideoTexture;
							}
						}

                        if (m_bPC_FastMode == true)
                        {

                            if (m_TargetMaterial[i].GetComponent<RawImage>() != null)
                            {
                                m_TargetMaterial[i].GetComponent<RawImage>().material = material;

                                if (m_TargetMaterial[i].GetComponent<RawImage>().material.GetTexture("_Y") != m_VideoTextureY)
                                {
                                    m_TargetMaterial[i].GetComponent<RawImage>().material.SetTexture("_Y", m_VideoTextureY);
                                }

                                if (m_TargetMaterial[i].GetComponent<RawImage>().material.GetTexture("_U") != m_VideoTextureU)
                                {
                                    m_TargetMaterial[i].GetComponent<RawImage>().material.SetTexture("_U", m_VideoTextureU);
                                }

                                if (m_TargetMaterial[i].GetComponent<RawImage>().material.GetTexture("_V") != m_VideoTextureV)
                                {
                                    m_TargetMaterial[i].GetComponent<RawImage>().material.SetTexture("_V", m_VideoTextureV);
                                }
                            }


                        }

                    }
				}


				if (bVideoFirstFrameReady == true) {
					if (OnVideoFirstFrameReady != null) {
						OnVideoFirstFrameReady ();
						bVideoFirstFrameReady = false;
					}

					if (listAudio.Count > 0)
					{
						if (audioSource == null /*&& (int)((float)pAudioCodecContext->sample_rate * ((float)listAudioPtsTime[i] + ((float)Call_GetDuration() / 1000.0f))) > 0*/)
						{
							audioSource = gameObject.AddComponent<AudioSource>();
						}

						if (audioClip == null && audioSource != null)
						{

							/*if ((float)Call_GetDuration() <= 0)
                    {
                        audioClip = AudioClip.Create("videoAudio",(int)((float)pAudioCodecContext->sample_rate * ((float)listAudioPtsTime[i] + 600.0f)),pAudioCodecContext->channels,pAudioCodecContext->sample_rate,true,OnAudioRead);
                    }
                    else*/
							{
								/*if (Call_GetDuration () > 1500000) {
                            audioClip = AudioClip.Create("videoAudio",(int)((float)pAudioCodecContext->sample_rate * ((float)listAudioPtsTime[i] + 10.0f)),pAudioCodecContext->channels,pAudioCodecContext->sample_rate,false);
                        }
                        else{
                            audioClip = AudioClip.Create("videoAudio",(int)((float)pAudioCodecContext->sample_rate * ((float)listAudioPtsTime[i] + 600.0f)),pAudioCodecContext->channels,pAudioCodecContext->sample_rate,false);
                        }*/
	#if UNITY_5
								audioClip = AudioClip.Create("videoAudio", (int)((float)pAudioCodecContext->sample_rate * 600.0f), pAudioCodecContext->channels, pAudioCodecContext->sample_rate, false);


	#else
	audioClip = AudioClip.Create("videoAudio",(int)((float)pAudioCodecContext->sample_rate * 600.0f),pAudioCodecContext->channels,pAudioCodecContext->sample_rate,false,false);


	#endif

							}




							audioSource.clip = audioClip;
							//audioSource.Play();

							//AudioSource.PlayClipAtPoint(audioClip,new Vector3(0,0,0));
						}
					}

					// Just to be sure I guess - don't know what migh happen if I seek in video etc...
					if (m_LastAudioPartSetted > listAudio.Count)
					{
						m_LastAudioPartSetted = -1;
					}

					// using m_LastAudioPartSetted - 1 again to be sure - what if the last part of audio gets updated/appended? IDK...
					for (int i = Math.Max(0, m_LastAudioPartSetted - 1); i < listAudio.Count; i++) {

						m_LastAudioPartSetted = i;

						if (audioSource != null && Call_GetDuration() >0 ) {


							if (listAudioPts.Count > i) {
								if (listAudioPts [i] >= 0) {

									{
										/*if (listAudioPts [i] > (int)((float)pAudioCodecContext->sample_rate * ((float)listAudioPtsTime [i] + ((float)Call_GetDuration () / 1000.0f)))) {
                                    audioClip.SetData (listAudio [i], (int)(((double)pAudioCodecContext->sample_rate) * listAudioPtsTime [i]));
                                } else*/ {
											//Debug.Log(audioSource.time  + " " + (float)listAudioPtsTime[i]  + " " +listAudioPts [i]   +" " +  fLastFrameTime);

											audioClip.SetData(listAudio[i],(int)(listAudioPts[i] %  (pAudioCodecContext->sample_rate * 600.0f))  );

										}
									}
								}
							}

							//if(audioSource.isPlaying == false)
							//audioSource.Play();
						}



					}



				} 





			}//while(fLastFrameTime < fCurrentSeekTime );






			//Debug.Log(listAudio.Count);






		}





	}

	private void Call_SetVolume(float fVolume)
	{
		if (audioSource != null)
			audioSource.volume = fVolume;
	}


	bool bSeekTo = false;


	private void Call_SetSeekPosition(int iSeek)
	{


		if( threadVideo != null)
		{
			while(threadVideo.IsAlive == true)
			{
				threadVideo.Abort();
			}

			threadVideo = null;
		}

		bSeekTo = true;
		iInitCount = 0;


		long seek_target = (long)iSeek * 1000;



		Debug.Log (seek_target);
		seek_target= ffmpeg.av_rescale_q(seek_target, ffmpeg.av_get_time_base_q() , pStream->time_base);

		Debug.Log (seek_target);


		//int seek_flags =  iSeek - (int)(fLastFrameTime * 1000.0f) < 0 ? ffmpeg.AVSEEK_FLAG_BACKWARD : ffmpeg.AVSEEK_FLAG_BACKWARD;

		if(ffmpeg.av_seek_frame(pFormatContext, iStreamIndex, 
			seek_target, ffmpeg.AVSEEK_FLAG_BACKWARD) < 0) {
            //error

          

		} else {
			/* handle packet queues... more later... */

		}


		fCurrentSeekTime = (float)iSeek / 1000.0f;
		//fLastFrameTime = 0;

		listVideo.Clear();
		listVideoPts.Clear();

        listAudio.Clear();
        listAudioPts.Clear();
        listAudioPtsTime.Clear();

        Destroy(audioClip);
        audioClip = null;
      







        ffmpeg.avcodec_flush_buffers (pCodecContext);




		//Debug.Log (fLastFrameTime + " " + fCurrentSeekTime);

	}

	private int Call_GetSeekPosition()
	{
		return (int)(fCurrentSeekTime * 1000.0f);
	}

	private void Call_Play(int iSeek)
	{

		if (m_CurrentState == MEDIAPLAYER_STATE.READY || m_CurrentState == MEDIAPLAYER_STATE.STOPPED || m_CurrentState == MEDIAPLAYER_STATE.END  || m_CurrentState == MEDIAPLAYER_STATE.PAUSED) {
			SeekTo (iSeek);
			if (audioSource != null) {
				if( audioSource.isPlaying == false)
					audioSource.Play();
				audioSource.time = (float) iSeek / 1000.0f;
			}
            bEnd = false;
			m_CurrentState = MEDIAPLAYER_STATE.PLAYING;
		}

	}

	private void Call_Reset()
	{

	}

	private void Call_Stop()
	{
		SeekTo (0);
		if (audioSource != null) {
			audioSource.Stop ();
			audioSource.time = 0.0f;
		}

		m_CurrentState = MEDIAPLAYER_STATE.STOPPED;
	}

	private void Call_RePlay()
	{
		if (audioSource != null) {
			if( audioSource.isPlaying == false)
				audioSource.Play();

		}

		m_CurrentState = MEDIAPLAYER_STATE.PLAYING;
	}

	private void Call_Pause()
	{
		if (audioSource != null) {
			audioSource.Pause();
		}



		m_CurrentState = MEDIAPLAYER_STATE.PAUSED;
	}

	private int Call_GetVideoWidth()
	{
		//Debug.Log (m_iWidth);
		return m_iWidth;
	}

	private int Call_GetVideoHeight()
	{
		//Debug.Log (m_iHeight);
		return m_iHeight;
	}

	private void Call_SetUnityTexture(int iTextureID)
	{

	}

	private void Call_SetWindowSize()
	{

	}

	private void Call_SetLooping(bool bLoop)
	{

	}

	private void Call_SetRockchip(bool bValue)
	{

	}

	public void Call_SetUnityActivity()
	{
		ffmpeg.av_register_all();
		ffmpeg.avcodec_register_all();
		ffmpeg.avformat_network_init();

		m_iID = SetTexture ();



	}

	private int Call_GetError()
	{
		return 0;
	}

	private int Call_GetErrorExtra()
	{
		return 0;
	}

	private int Call_GetDuration()
	{
		//Debug.Log ((int)(pFormatContext->duration / 1000));

		if (pFormatContext != null)
			return (int)(pFormatContext->duration / 1000);

		return 0;
	}

	private int Call_GetCurrentSeekPercent()
	{
		return -1;
	}

	private void Call_SetSplitOBB(bool bValue, string strOBBName)
	{
	}

	private void Call_SetSpeed(float fSpeed)
	{
		if (audioSource != null) {
			audioSource.pitch = fSpeed;
		}
	}

	private MEDIAPLAYER_STATE Call_GetStatus()
	{
		return m_CurrentState;
	}

	#endif // !UNITY_EDITOR



	public IEnumerator DownloadStreamingVideoAndLoad(string strURL)
	{
		strURL = strURL.Trim();

		Debug.Log("DownloadStreamingVideo : " + strURL);


		WWW www = new WWW(strURL);

		yield return www;

		if (string.IsNullOrEmpty(www.error))
		{

			if (System.IO.Directory.Exists(Application.persistentDataPath + "/Data") == false)
				System.IO.Directory.CreateDirectory(Application.persistentDataPath + "/Data");

			string write_path = Application.persistentDataPath + "/Data" + strURL.Substring(strURL.LastIndexOf("/"));

			/*	if(System.IO.File.Exists(write_path) == true)
			{
				Debug.Log("Delete : " + write_path);
				System.IO.File.Delete(write_path);
			}
		*/
			System.IO.File.WriteAllBytes(write_path, www.bytes);

			Load("file://" + write_path);
		}
		else
		{
			Debug.Log(www.error);

		}

		www.Dispose();
		www = null;
		Resources.UnloadUnusedAssets();
	}

	public IEnumerator DownloadStreamingVideoAndLoad2(string strURL)
	{
		strURL = strURL.Trim();

		string write_path = Application.persistentDataPath + "/Data" + strURL.Substring(strURL.LastIndexOf("/"));

		if (System.IO.File.Exists(write_path) == true)
		{
			Load("file://" + write_path);
		}
		else
		{
			WWW www = new WWW(strURL);

			yield return www;

			if (string.IsNullOrEmpty(www.error))
			{

				if (System.IO.Directory.Exists(Application.persistentDataPath + "/Data") == false)
					System.IO.Directory.CreateDirectory(Application.persistentDataPath + "/Data");


				System.IO.File.WriteAllBytes(write_path, www.bytes);

				Load("file://" + write_path);
			}
			else
			{
				Debug.Log(www.error);

			}

			www.Dispose();
			www = null;
			Resources.UnloadUnusedAssets();
		}


	}


	IEnumerator CopyStreamingAssetVideoAndLoad(string strURL)
	{
		strURL = strURL.Trim();

		string write_path = Application.persistentDataPath + "/" + strURL;

		if (System.IO.File.Exists(write_path) == false)
		{
			Debug.Log("CopyStreamingAssetVideoAndLoad : " + strURL);

			WWW www = new WWW(Application.streamingAssetsPath + "/" + strURL);

			yield return www;

			if (string.IsNullOrEmpty(www.error))
			{



				Debug.Log(write_path);
				System.IO.File.WriteAllBytes(write_path, www.bytes);

				Load("file://" + write_path);


			}
			else
			{
				Debug.Log(www.error);

			}

			www.Dispose();
			www = null;
		}
		else
		{
			Load("file://" + write_path);
		}

	}
	#endif



	#region UnityMainThread adapter

	private List<Action> unityMainThreadActionList = new List<Action>();
	private bool checkNewActions = false;
	private object thisLock = new object();

	/// <summary>
	/// Execute UnityMainThread operations from unityMainThreadActionList List
	/// </summary>
	void CheckThreading()
	{
		lock (thisLock)
		{
			if (unityMainThreadActionList.Count > 0)
			{
				foreach(var a in unityMainThreadActionList)	
				{
					a();
				}
				unityMainThreadActionList.Clear ();
			}
		}
	}

	/// <summary>
	/// Add a sequence of Code enclosed into the Action (a). It triggers checkNewActions to execute code into the Unity Thread Update
	/// </summary>
	/// <param name="a">The alpha component.</param>
	void AddActionForUnityMainThread(Action a)
	{
		lock (thisLock)
		{
			unityMainThreadActionList.Add(a);
		}
		checkNewActions = true;
	}

	#endregion


}
