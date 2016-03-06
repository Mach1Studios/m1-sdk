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
using System.Runtime.InteropServices;		// required for DllImport

public class OVRChromaticAberration : MonoBehaviour {
	
	public OVRGamepadController.Button			toggleButton = OVRGamepadController.Button.X;	

	private bool								chromatic = false;

#if (UNITY_ANDROID && !UNITY_EDITOR)
	[DllImport("OculusPlugin")]
	private static extern void OVR_TW_EnableChromaticAberration( bool enable );
#endif

	/// <summary>
	/// Start this instance.
	/// </summary>
	void Start ()
	{
#if (UNITY_ANDROID && !UNITY_EDITOR)
		// Enable/Disable Chromatic Aberration Correction.
		// NOTE: Enabling Chromatic Aberration for mobile has a large performance cost.
		OVR_TW_EnableChromaticAberration(chromatic);
#endif
	}

	/// <summary>
	/// Check input and toggle chromatic aberration correction if necessary.
	/// See the input mapping setup in the Unity Integration guide.
	/// </summary>
	void Update()
	{
		// NOTE: some of the buttons defined in OVRGamepadController.Button are not available on the Android game pad controller
		if (Input.GetButtonDown(OVRGamepadController.ButtonNames[(int)toggleButton]))
		{
			//*************************
			// toggle chromatic aberration correction
			//*************************
			chromatic = !chromatic;
#if (UNITY_ANDROID && !UNITY_EDITOR)
			OVR_TW_EnableChromaticAberration(chromatic);
#endif
		}
	}

}
