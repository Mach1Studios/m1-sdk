/************************************************************************************

Copyright   :   Copyright 2014 Oculus VR, LLC. All Rights reserved.

Licensed under the Oculus VR Rift SDK License Version 3.2 (the "License");
you may not use the Oculus VR Rift SDK except in compliance with the License,
which is provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

You may obtain a copy of the License at

http://www.oculusvr.com/licenses/LICENSE-3.2

Unless required by applicable law or agreed to in writing, the Oculus VR SDK
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************************/

using System;
using System.Runtime.InteropServices;
using UnityEngine;
using Ovr;

/// <summary>
/// Manages an Oculus Rift head-mounted display (HMD).
/// </summary>
public class OVRDisplay
{
	/// <summary>
	/// Specifies the size and field-of-view for one eye texture.
	/// </summary>
	public struct EyeRenderDesc
	{
		/// <summary>
		/// The horizontal and vertical size of the texture.
		/// </summary>
		public Vector2 resolution;

		/// <summary>
		/// The angle of the horizontal and vertical field of view in degrees.
		/// </summary>
		public Vector2 fov;
	}

	/// <summary>
	/// Contains latency measurements for a single frame of rendering.
	/// </summary>
	public struct LatencyData
	{
		/// <summary>
		/// The time it took to render both eyes in seconds.
		/// </summary>
		public float render;

		/// <summary>
		/// The time it took to perform TimeWarp in seconds.
		/// </summary>
		public float timeWarp;

		/// <summary>
		/// The time between the end of TimeWarp and scan-out in seconds.
		/// </summary>
		public float postPresent;
		public float renderError;
		public float timeWarpError;
	}
	
	/// <summary>
	/// If true, a physical HMD is attached to the system.
	/// </summary>
	/// <value><c>true</c> if is present; otherwise, <c>false</c>.</value>
	public bool isPresent
	{
		get {
#if !UNITY_ANDROID || UNITY_EDITOR
			if (!OVRManager.instance.isVRPresent)
				return false;
			return (OVRManager.capiHmd.GetTrackingState(0f).StatusFlags & (uint)StatusBits.HmdConnected) != 0;
#else
			return OVR_IsHMDPresent();
#endif
		}
	}

	private int prevAntiAliasing;
	private int prevScreenWidth;
	private int prevScreenHeight;
	private bool needsConfigureTexture = true;
	private bool needsSetTexture = true;
	private bool needsSetDistortionCaps;
	private bool prevFullScreen;
	private OVRPose[] eyePoses = new OVRPose[(int)OVREye.Count];
	private EyeRenderDesc[] eyeDescs = new EyeRenderDesc[(int)OVREye.Count];
    private RenderTexture[] eyeTextures = new RenderTexture[eyeTextureCount];
	private int[] eyeTextureIds = new int[eyeTextureCount];
	private int currEyeTextureIdx = 0;
	private static int frameCount = 0;

#if !UNITY_ANDROID && !UNITY_EDITOR
	private bool needsSetViewport = true;
#endif

#if UNITY_ANDROID && !UNITY_EDITOR
	private const int eyeTextureCount = 3 * (int)OVREye.Count; // triple buffer
#else
	private const int eyeTextureCount = 1 * (int)OVREye.Count;
#endif

#if UNITY_ANDROID && !UNITY_EDITOR
	private int nextEyeTextureIdx = 0;
#endif

	/// <summary>
	/// Creates an instance of OVRDisplay. Called by OVRManager.
	/// </summary>
	public OVRDisplay()
	{
#if !UNITY_ANDROID || UNITY_EDITOR
        prevFullScreen = Screen.fullScreen;
#endif
		ConfigureEyeTextures();

		OVRManager.Created += () => { needsConfigureTexture = true; };
		OVRManager.NativeTextureScaleModified += (prev, current) => { needsConfigureTexture = true; };
		OVRManager.EyeTextureAntiAliasingModified += (prev, current) => { needsConfigureTexture = true; };
		OVRManager.EyeTextureDepthModified += (prev, current) => { needsConfigureTexture = true; };
		OVRManager.EyeTextureFormatModified += (prev, current) => { needsConfigureTexture = true; };

		OVRManager.VirtualTextureScaleModified += (prev, current) => { needsSetTexture = true; };
		OVRManager.MonoscopicModified += (prev, current) => { needsSetTexture = true; };

		OVRManager.HdrModified += (prev, current) => { needsSetDistortionCaps = true; };
	}

	/// <summary>
	/// Updates the internal state of the OVRDisplay. Called by OVRManager.
	/// </summary>
	public void Update()
	{
		UpdateDistortionCaps();
		UpdateViewport();
		UpdateTextures();
	}

	/// <summary>
	/// Marks the beginning of all rendering.
	/// </summary>
    public void BeginFrame()
	{
		bool updateFrameCount = !(OVRManager.instance.timeWarp && OVRManager.instance.freezeTimeWarp);
		if (updateFrameCount)
		{
			frameCount++;
		}

		OVRPluginEvent.IssueWithData(RenderEventType.BeginFrame, frameCount);
    }

	/// <summary>
	/// Marks the end of all rendering.
	/// </summary>
    public void EndFrame()
    {
		OVRPluginEvent.Issue(RenderEventType.EndFrame);
    }

	/// <summary>
	/// Gets the head pose at the current time or predicted at the given time.
	/// </summary>
	public OVRPose GetHeadPose(double predictionTime)
	{
#if !UNITY_ANDROID || UNITY_EDITOR
		if (!OVRManager.instance.isVRPresent)
		{
			return new OVRPose
			{
				position = Vector3.zero,
				orientation = Quaternion.identity,
			};
		}

		double abs_time_plus_pred = Hmd.GetTimeInSeconds() + predictionTime;

		TrackingState state = OVRManager.capiHmd.GetTrackingState(abs_time_plus_pred);

		return state.HeadPose.ThePose.ToPose(true);
#else
		float px = 0.0f, py = 0.0f, pz = 0.0f, ow = 0.0f, ox = 0.0f, oy = 0.0f, oz = 0.0f;

		double atTime = Time.time + predictionTime;
		OVR_GetCameraPositionOrientation(ref  px, ref  py, ref  pz,
									     ref  ox, ref  oy, ref  oz, ref  ow, atTime);

		return new OVRPose
		{
			position = new Vector3(px, py, -pz),
			orientation = new Quaternion(-ox, -oy, oz, ow),
		};
#endif
	}

#if UNITY_ANDROID && !UNITY_EDITOR
	private float w = 0.0f, x = 0.0f, y = 0.0f, z = 0.0f, fov = 90.0f;
#endif

	/// <summary>
	/// Gets the pose of the given eye, predicted for the time when the current frame will scan out.
	/// </summary>
	/// <description>NOTE: This is safe to call in an Update function, but not in LateUpdate or subsequent callbacks.</description>
	public OVRPose GetEyePose(OVREye eye)
	{
#if !UNITY_ANDROID || UNITY_EDITOR
        if (!OVRManager.instance.isVRPresent)
		{
			return new OVRPose
			{
				position = Vector3.zero,
				orientation = Quaternion.identity,
			};
		}

		bool updateEyePose = !(OVRManager.instance.timeWarp && OVRManager.instance.freezeTimeWarp);
		if (updateEyePose)
		{
			eyePoses[(int)eye] = OVR_GetRenderPose(frameCount, (int)eye).ToPose(true);
		}

		return eyePoses[(int)eye];
#else
		if (eye == OVREye.Left)
			OVR_GetSensorState(
					OVRManager.instance.monoscopic,
				   	ref w,
				   	ref x,
				   	ref y,
				   	ref z,
				   	ref fov,
				   	ref OVRManager.timeWarpViewNumber);

		Quaternion rot = new Quaternion(-x, -y, z, w);

		float eyeOffsetX = 0.5f * OVRManager.profile.ipd;
		eyeOffsetX = (eye == OVREye.Left) ? -eyeOffsetX : eyeOffsetX;

		float neckToEyeHeight = OVRManager.profile.eyeHeight - OVRManager.profile.neckHeight;
		Vector3 headNeckModel = new Vector3(0.0f, neckToEyeHeight, OVRManager.profile.eyeDepth);
		Vector3 pos = rot * (new Vector3(eyeOffsetX, 0.0f, 0.0f) + headNeckModel);
		
		// Subtract the HNM pivot to avoid translating the camera when level
		pos -= headNeckModel;

		return new OVRPose
		{
			position = pos,
			orientation = rot,
		};
#endif
	}

	/// <summary>
	/// Gets the given eye's projection matrix.
	/// </summary>
	/// <param name="eyeId">Specifies the eye.</param>
	/// <param name="nearClip">The distance to the near clipping plane.</param>
	/// <param name="farClip">The distance to the far clipping plane.</param>
	public Matrix4x4 GetProjection(int eyeId, float nearClip, float farClip)
	{
#if !UNITY_ANDROID || UNITY_EDITOR
        if (!OVRManager.instance.isVRPresent)
			return new Matrix4x4();

		FovPort fov = OVRManager.capiHmd.GetDesc().DefaultEyeFov[eyeId];

		uint projectionModFlags = (uint)Hmd.ProjectionModifier.RightHanded;

		return Hmd.GetProjection(fov, nearClip, farClip, projectionModFlags).ToMatrix4x4();
#else
		return new Matrix4x4();
#endif
	}

	/// <summary>
	/// Occurs when the head pose is reset.
	/// </summary>
	public event System.Action RecenteredPose;

	/// <summary>
	/// Recenters the head pose.
	/// </summary>
	public void RecenterPose()
	{
#if !UNITY_ANDROID || UNITY_EDITOR
        if (!OVRManager.instance.isVRPresent)
			return;

		OVRManager.capiHmd.RecenterPose();
#else
		OVR_ResetSensorOrientation();
#endif

		if (RecenteredPose != null)
		{
			RecenteredPose();
		}
	}

	/// <summary>
	/// Gets the current acceleration of the head.
	/// </summary>
	public Vector3 acceleration
	{
		get {
#if !UNITY_ANDROID || UNITY_EDITOR
	        if (!OVRManager.instance.isVRPresent)
				return Vector3.zero;

			return OVRManager.capiHmd.GetTrackingState(0f).HeadPose.LinearAcceleration.ToVector3(true);
#else
			float x = 0.0f, y = 0.0f, z = 0.0f;
			OVR_GetAcceleration(ref x, ref y, ref z);
			return new Vector3(x, y, z);
#endif
		}
	}
	
	/// <summary>
	/// Gets the current angular velocity of the head.
	/// </summary>
	public Vector3 angularVelocity
	{
		get {
#if !UNITY_ANDROID || UNITY_EDITOR
	        if (!OVRManager.instance.isVRPresent)
				return Vector3.zero;

			return OVRManager.capiHmd.GetTrackingState(0f).HeadPose.AngularVelocity.ToVector3(true);
#else
			float x = 0.0f, y = 0.0f, z = 0.0f;
			OVR_GetAngularVelocity(ref x, ref y, ref z);
			return new Vector3(x, y, z);
#endif
		}
	}

	/// <summary>
	/// Gets the resolution and field of view for the given eye.
	/// </summary>
	public EyeRenderDesc GetEyeRenderDesc(OVREye eye)
	{
		return eyeDescs[(int)eye];
	}

	/// <summary>
	/// Gets the currently active render texture for the given eye.
	/// </summary>
	public RenderTexture GetEyeTexture(OVREye eye)
	{
		return eyeTextures[currEyeTextureIdx + (int)eye];
	}

	/// <summary>
	/// Gets the currently active render texture's native ID for the given eye.
	/// </summary>
	public int GetEyeTextureId(OVREye eye)
	{
		return eyeTextureIds[currEyeTextureIdx + (int)eye];
	}

	/// <summary>
	/// True if the direct mode display driver is active.
	/// </summary>
	public bool isDirectMode
	{
		get
		{
#if !UNITY_ANDROID || UNITY_EDITOR
	        if (!OVRManager.instance.isVRPresent)
				return false;

			uint caps = OVRManager.capiHmd.GetDesc().HmdCaps;
			uint mask = caps & (uint)HmdCaps.ExtendDesktop;
			return mask == 0;
#else
			return false;
#endif
		}
	}

	/// <summary>
	/// If true, direct mode rendering will also show output in the main window.
	/// </summary>
	public bool mirrorMode
	{
		get
		{
#if !UNITY_ANDROID || UNITY_EDITOR
	        if (!OVRManager.instance.isVRPresent)
				return false;

			uint caps = OVRManager.capiHmd.GetEnabledCaps();
			return (caps & (uint)HmdCaps.NoMirrorToWindow) == 0;
#else
			return false;
#endif
		}

		set
		{
#if !UNITY_ANDROID || UNITY_EDITOR
	        if (!OVRManager.instance.isVRPresent)
				return;

			uint caps = OVRManager.capiHmd.GetEnabledCaps();

			if (((caps & (uint)HmdCaps.NoMirrorToWindow) == 0) == value)
				return;

			if (value)
				caps &= ~(uint)HmdCaps.NoMirrorToWindow;
			else
				caps |= (uint)HmdCaps.NoMirrorToWindow;

			OVRManager.capiHmd.SetEnabledCaps(caps);
#endif
		}
	}
	
	/// <summary>
	/// If true, TimeWarp will be used to correct the output of each OVRCameraRig for rotational latency.
	/// </summary>
	internal bool timeWarp
	{
		get { return (distortionCaps & (int)DistortionCaps.TimeWarp) != 0; }
		set
		{
			if (value != timeWarp)
				distortionCaps ^= (int)DistortionCaps.TimeWarp;
		}
	}

	/// <summary>
	/// If true, VR output will be rendered upside-down.
	/// </summary>
	internal bool flipInput
	{
		get { return (distortionCaps & (int)DistortionCaps.FlipInput) != 0; }
		set
		{
			if (value != flipInput)
				distortionCaps ^= (int)DistortionCaps.FlipInput;
		}
	}

	/// <summary>
	/// Enables and disables distortion rendering capabilities from the Ovr.DistortionCaps enum.
	/// </summary>
	public uint distortionCaps
	{
		get
		{
			return _distortionCaps;
		}

		set
		{
			if (value == _distortionCaps)
				return;

			_distortionCaps = value;
#if !UNITY_ANDROID || UNITY_EDITOR
	        if (!OVRManager.instance.isVRPresent)
				return;

			OVR_SetDistortionCaps(value);
#endif
		}
	}
	private uint _distortionCaps =
#if (UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX)
		(uint)DistortionCaps.ProfileNoTimewarpSpinWaits |
#endif
		(uint)DistortionCaps.Chromatic |
		(uint)DistortionCaps.Vignette |
		(uint)DistortionCaps.Overdrive;

	/// <summary>
	/// Gets the current measured latency values.
	/// </summary>
	public LatencyData latency
	{
		get {
#if !UNITY_ANDROID || UNITY_EDITOR
	        if (OVRManager.instance.isVRPresent)
	        {
			float[] values = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
			float[] latencies = OVRManager.capiHmd.GetFloatArray("DK2Latency", values);

			return new LatencyData
			{
				render = latencies[0] * 1000.0f,
				timeWarp = latencies[1] * 1000.0f,
				postPresent = latencies[2] * 1000.0f,
				renderError = latencies[3] * 1000.0f,
				timeWarpError = latencies[4] * 1000.0f,
			};
	        }
#endif
			return new LatencyData
			{
				render = 0.0f,
				timeWarp = 0.0f,
				postPresent = 0.0f,
				renderError = 0.0f,
				timeWarpError = 0.0f,
			};
		}
	}

	private void UpdateDistortionCaps()
	{
#if !UNITY_ANDROID || UNITY_EDITOR
		needsSetDistortionCaps = needsSetDistortionCaps
			|| QualitySettings.antiAliasing != prevAntiAliasing;

		if (needsSetDistortionCaps)
		{
			if (QualitySettings.antiAliasing > 0)
			{
				distortionCaps |= (uint)Ovr.DistortionCaps.HqDistortion;
			}
			else
			{
				distortionCaps &= ~(uint)Ovr.DistortionCaps.HqDistortion;
			}
	
			if (QualitySettings.activeColorSpace == ColorSpace.Linear && !OVRManager.instance.hdr)
			{
				distortionCaps |= (uint)Ovr.DistortionCaps.SRGB;
			}
			else
			{
				distortionCaps &= ~(uint)Ovr.DistortionCaps.SRGB;
			}

			prevAntiAliasing = QualitySettings.antiAliasing;

			needsSetDistortionCaps = false;
			needsSetTexture = true;
		}
#endif
	}

	private void UpdateViewport()
	{
#if !UNITY_ANDROID && !UNITY_EDITOR
		needsSetViewport = needsSetViewport
			|| Screen.width != prevScreenWidth
			|| Screen.height != prevScreenHeight;

		if (needsSetViewport)
		{
			SetViewport(0, 0, Screen.width, Screen.height);

			prevScreenWidth = Screen.width;
			prevScreenHeight = Screen.height;

			needsSetViewport = false;
		}
#endif
	}

	private void UpdateTextures()
	{
#if !UNITY_ANDROID || UNITY_EDITOR
        if (!OVRManager.instance.isVRPresent)
        	return;

		ConfigureEyeTextures();
#endif

		for (int i = 0; i < eyeTextureCount; i++)
		{
			if (!eyeTextures[i].IsCreated())
			{
				eyeTextures[i].Create();
				eyeTextureIds[i] = eyeTextures[i].GetNativeTextureID();

#if !UNITY_ANDROID || UNITY_EDITOR
				needsSetTexture = true;
#endif
			}
		}

#if !UNITY_ANDROID || UNITY_EDITOR
        needsSetTexture = needsSetTexture
			|| Screen.fullScreen != prevFullScreen
			|| OVR_UnityGetModeChange();

        if (needsSetTexture)
        {
			for (int i = 0; i < eyeTextureCount; i += (int)OVREye.Count)
			{
				int leftEyeIndex = i + (int)OVREye.Left;
				int rightEyeIndex = i + (int)OVREye.Right;

				IntPtr leftEyeTexturePtr = eyeTextures[leftEyeIndex].GetNativeTexturePtr();
				IntPtr rightEyeTexturePtr = eyeTextures[rightEyeIndex].GetNativeTexturePtr();

				if (OVRManager.instance.monoscopic)
					rightEyeTexturePtr = leftEyeTexturePtr;

				if (leftEyeTexturePtr == System.IntPtr.Zero || rightEyeTexturePtr == System.IntPtr.Zero)
					return;

				OVR_SetTexture(leftEyeIndex, leftEyeTexturePtr, OVRManager.instance.virtualTextureScale);
				OVR_SetTexture(rightEyeIndex, rightEyeTexturePtr, OVRManager.instance.virtualTextureScale);
			}

			prevFullScreen = Screen.fullScreen;
			OVR_UnitySetModeChange(false);

            needsSetTexture = false;
        }
#else
		currEyeTextureIdx = nextEyeTextureIdx;
		nextEyeTextureIdx = (nextEyeTextureIdx + 2) % eyeTextureCount;
#endif
	}

	private void ConfigureEyeDesc(OVREye eye)
	{
        Vector2 texSize = Vector2.zero;
        Vector2 fovSize = Vector2.zero;

#if !UNITY_ANDROID || UNITY_EDITOR
        if (!OVRManager.instance.isVRPresent)
        	return;

		FovPort fovPort = OVRManager.capiHmd.GetDesc().DefaultEyeFov[(int)eye];
		fovPort.LeftTan = fovPort.RightTan = Mathf.Max(fovPort.LeftTan, fovPort.RightTan);
		fovPort.UpTan = fovPort.DownTan = Mathf.Max(fovPort.UpTan, fovPort.DownTan);

		texSize = OVRManager.capiHmd.GetFovTextureSize((Ovr.Eye)eye, fovPort, OVRManager.instance.nativeTextureScale).ToVector2();
        fovSize = new Vector2(2f * Mathf.Rad2Deg * Mathf.Atan(fovPort.LeftTan), 2f * Mathf.Rad2Deg * Mathf.Atan(fovPort.UpTan));
#else
		texSize = new Vector2(1024, 1024) * OVRManager.instance.nativeTextureScale;
		fovSize = new Vector2(90, 90);
#endif

		eyeDescs[(int)eye] = new EyeRenderDesc()
		{
			resolution = texSize,
            fov = fovSize
		};
	}

	private void ConfigureEyeTextures()
	{
		if (!OVRManager.instance.isVRPresent)
			return;

		ConfigureEyeDesc(OVREye.Left);
		ConfigureEyeDesc(OVREye.Right);

		if (!needsConfigureTexture)
			return;

		for (int eyeBufferIndex = 0; eyeBufferIndex < eyeTextureCount; eyeBufferIndex += 2)
		{
			foreach (var eye in new OVREye[] { OVREye.Left, OVREye.Right })
			{
				int eyeIndex = eyeBufferIndex + (int)eye;
				EyeRenderDesc eyeDesc = eyeDescs[(int)eye];
				
				eyeTextures[eyeIndex] = new RenderTexture(
					(int)eyeDesc.resolution.x,
					(int)eyeDesc.resolution.y,
					(int)OVRManager.instance.eyeTextureDepth,
					OVRManager.instance.eyeTextureFormat);
				
				eyeTextures[eyeIndex].antiAliasing = (int)OVRManager.instance.eyeTextureAntiAliasing;
				
				eyeTextures[eyeIndex].Create();
				eyeTextureIds[eyeIndex] = eyeTextures[eyeIndex].GetNativeTextureID();
			}
		}

#if !UNITY_ANDROID || UNITY_EDITOR
		OVR_UnitySetModeChange(true);
#endif
		needsSetTexture = true;

		needsConfigureTexture = false;
	}

    public void SetViewport(int x, int y, int w, int h)
    {
#if !UNITY_ANDROID || UNITY_EDITOR
        if (!OVRManager.instance.isVRPresent)
        	return;

        OVR_SetViewport(x, y, w, h);
#endif
    }

	private const string LibOVR = "OculusPlugin";

#if UNITY_ANDROID && !UNITY_EDITOR
	//TODO: Get rid of these functions and implement OVR.CAPI.Hmd on Android.

	[DllImport(LibOVR)]
	private static extern bool OVR_ResetSensorOrientation();
	[DllImport(LibOVR)]
	private static extern bool OVR_GetAcceleration(ref float x, ref float y, ref float z);
	[DllImport(LibOVR)]
	private static extern bool OVR_GetAngularVelocity(ref float x, ref float y, ref float z);
	[DllImport(LibOVR)]
	private static extern bool OVR_IsHMDPresent();
	[DllImport(LibOVR)]
	private static extern bool OVR_GetCameraPositionOrientation(
		ref float px,
		ref float py,
		ref float pz,
		ref float ox,
		ref float oy,
		ref float oz,
		ref float ow,
		double atTime);
	[DllImport(LibOVR)]
	private static extern bool OVR_GetSensorState(
		bool monoscopic,
		ref float w,
		ref float x,
		ref float y,
		ref float z,
		ref float fov,
		ref int viewNumber);
#else
    [DllImport(LibOVR, CallingConvention = CallingConvention.Cdecl)]
    private static extern void OVR_SetDistortionCaps(uint distortionCaps);
    [DllImport(LibOVR, CallingConvention = CallingConvention.Cdecl)]
    private static extern bool OVR_SetViewport(int x, int y, int w, int h);
    [DllImport(LibOVR, CallingConvention = CallingConvention.Cdecl)]
    private static extern Posef OVR_GetRenderPose(int frameIndex, int eyeId);
    [DllImport(LibOVR, CallingConvention = CallingConvention.Cdecl)]
    private static extern bool OVR_SetTexture(int id, System.IntPtr texture, float scale);
    [DllImport(LibOVR, CallingConvention = CallingConvention.Cdecl)]
    private static extern bool OVR_UnityGetModeChange();
    [DllImport(LibOVR, CallingConvention = CallingConvention.Cdecl)]
    private static extern bool OVR_UnitySetModeChange(bool isChanged);
#endif
}
