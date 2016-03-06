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
using System.Runtime.InteropServices; // required for DllImport

public class OVRDebugGraph : MonoBehaviour
{
	public OVRTimeWarpUtils.DebugPerfMode debugMode = OVRTimeWarpUtils.DebugPerfMode.DEBUG_PERF_OFF;
	public OVRGamepadController.Button toggleButton = OVRGamepadController.Button.Start;

#if (UNITY_ANDROID && !UNITY_EDITOR)
	[DllImport("OculusPlugin")]
	private static extern void OVR_TW_SetDebugMode(OVRTimeWarpUtils.DebugPerfMode mode, OVRTimeWarpUtils.DebugPerfValue val);
#endif

	/// <summary>
	/// Initialize the debug mode
	/// </summary>
	void Start()
	{
#if UNITY_ANDROID && !UNITY_EDITOR
		// Turn on/off debug graph
		OVR_TW_SetDebugMode(debugMode, OVRTimeWarpUtils.DebugPerfValue.DEBUG_VALUE_DRAW);
#endif
	}

	/// <summary>
	/// Check input and toggle the debug graph.
	/// See the input mapping setup in the Unity Integration guide.
	/// </summary>
	void Update()
	{
		// NOTE: some of the buttons defined in OVRGamepadController.Button are not available on the Android game pad controller
		if (Input.GetButtonDown( OVRGamepadController.ButtonNames[(int)toggleButton]))
		{
			Debug.Log(" TOGGLE GRAPH ");

			//*************************
			// toggle the debug graph .. off -> running -> paused
			//*************************
			switch (debugMode)
			{
				case OVRTimeWarpUtils.DebugPerfMode.DEBUG_PERF_OFF:
					debugMode = OVRTimeWarpUtils.DebugPerfMode.DEBUG_PERF_RUNNING;
					break;
				case OVRTimeWarpUtils.DebugPerfMode.DEBUG_PERF_RUNNING:
					debugMode = OVRTimeWarpUtils.DebugPerfMode.DEBUG_PERF_FROZEN;
					break;
				case OVRTimeWarpUtils.DebugPerfMode.DEBUG_PERF_FROZEN:
					debugMode = OVRTimeWarpUtils.DebugPerfMode.DEBUG_PERF_OFF;
					break;
			}

#if UNITY_ANDROID && !UNITY_EDITOR
			OVR_TW_SetDebugMode(debugMode, OVRTimeWarpUtils.DebugPerfValue.DEBUG_VALUE_DRAW);
#endif
		}
	}
}
