﻿/************************************************************************************

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

public class OVRMonoscopic : MonoBehaviour {
	
	public OVRGamepadController.Button	toggleButton = OVRGamepadController.Button.B;
	private bool						monoscopic = false;

	/// <summary>
	/// Check input and toggle monoscopic rendering mode if necessary
	/// See the input mapping setup in the Unity Integration guide
	/// </summary>
	void Update()
	{
		// NOTE: some of the buttons defined in OVRGamepadController.Button are not available on the Android game pad controller
		if (Input.GetButtonDown(OVRGamepadController.ButtonNames[(int)toggleButton]))
		{
			//*************************
			// toggle monoscopic rendering mode
			//*************************
			monoscopic = !monoscopic;
			OVRManager.instance.monoscopic = monoscopic;
		}
	}

}
