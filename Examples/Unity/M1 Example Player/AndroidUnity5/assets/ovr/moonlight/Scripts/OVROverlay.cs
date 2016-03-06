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

using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

/// <summary>
/// Add OVROverlay script to an object with a Quad mesh filter to have the quad
/// rendered as a TimeWarp overlay instead by drawing it into the eye buffer.
/// This will take full advantage of the display resolution and avoid double
/// resampling of the texture.
/// 
/// If the texture is dynamically generated, as for an interactive GUI or
/// animation, it must be explicitly triple buffered to avoid flickering
/// when it is referenced asynchronously by TimeWarp.
/// </summary>
public class OVROverlay : MonoBehaviour
{
	public enum OverlayType
	{
		None,			// Disabled the overlay
		Underlay,		// Eye buffers blend on top
		Overlay,		// Blends on top of the eye buffer
		OverlayShowLod	// Blends on top and colorizes texture level of detail
	};

#pragma warning disable 414		// The private field 'x' is assigned but its value is never used
	Matrix4x4 		toOculusMatrix = Matrix4x4.Scale(new Vector3(0.5f, -0.5f, -0.5f));
	OverlayType		currentOverlayType = OverlayType.Overlay;
	int 			texId = 0;
#pragma warning restore 414		// The private field 'x' is assigned but its value is never used

#if UNITY_ANDROID && !UNITY_EDITOR
	[DllImport("OculusPlugin")]
	private static extern void OVR_TW_SetOverlayPlane (int texId, int eye, int program,
	                                          float m0, float m1, float m2, float m3,
	                                          float m4, float m5, float m6, float m7,
	                                          float m8, float m9, float m10, float m11,
	                                          float m12, float m13, float m14, float m15);
#endif

	void Awake()
	{
		Debug.Log ("Overlay Awake");

		// Getting the NativeTextureID/PTR synchronizes with the multithreaded renderer, which
		// causes a problem on the first frame if this gets called after the OVRDisplay initialization,
		// so do it in Awake() instead of Start().
		texId = this.GetComponent<Renderer>().material.mainTexture.GetNativeTextureID();
	}

	void Update()
	{
		if (Input.GetKey (KeyCode.Joystick1Button0))
		{
			currentOverlayType = OverlayType.None;
		}
		else if (Input.GetKey (KeyCode.Joystick1Button1))
		{
			currentOverlayType = OverlayType.OverlayShowLod;
		}
		else
		{
			currentOverlayType = OverlayType.Overlay;
		}
	}

	void OnRenderObject ()
	{
		// The overlay must be specified every eye frame, because it is positioned relative to the
		// current head location.  If frames are dropped, it will be time warped appropriately,
		// just like the eye buffers.

		if (currentOverlayType == OverlayType.None)
		{
			GetComponent<Renderer>().enabled = true;	// use normal renderer
			return;
		}

		GetComponent<Renderer>().enabled = false;		// render with the overlay plane instead of the normal renderer

#if UNITY_ANDROID && !UNITY_EDITOR
		int eyeNum = (Camera.current.depth == (int)RenderEventType.RightEyeEndFrame) ? 1 : 0;
		Matrix4x4 mv = Camera.current.worldToCameraMatrix * this.transform.localToWorldMatrix * toOculusMatrix;

		OVR_TW_SetOverlayPlane (texId, eyeNum, (int)currentOverlayType,
	                        mv [0, 0], mv [0, 1], mv [0, 2], mv [0, 3],
	                        mv [1, 0], mv [1, 1], mv [1, 2], mv [1, 3],
	                        mv [2, 0], mv [2, 1], mv [2, 2], mv [2, 3],
	       					mv [3, 0], mv [3, 1], mv [3, 2], mv [3, 3]);
#endif
	}
	
}
