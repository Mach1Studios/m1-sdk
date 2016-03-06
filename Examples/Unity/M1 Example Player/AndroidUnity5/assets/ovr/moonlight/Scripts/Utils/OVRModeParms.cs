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

public class OVRModeParms : MonoBehaviour
{
#if (UNITY_ANDROID && !UNITY_EDITOR)
	[DllImport("OculusPlugin")]
	// Set the fixed CPU clock level.
	private static extern void OVR_VrModeParms_SetCpuLevel(int cpuLevel);

	[DllImport("OculusPlugin")]
	// Set the fixed GPU clock level.
	private static extern void OVR_VrModeParms_SetGpuLevel(int gpuLevel);

	[DllImport("OculusPlugin")]
	// If true, when the application detects it has been throttled, the Platform
	// UI will display a dismissable warning, then returns to the app in powersave
	// mode at 30FPS.
	// If false, the Platform UI will display a non-dismissable error message
	// indicating the user must leave VR.
	// Set to false if the application continues to judder even when forced to
	// 30FPS in power save mode.
	private static extern void OVR_VrModeParms_SetAllowPowerSave(bool allow);

	[DllImport("OculusPlugin")]
	// Returns true if the application has been throttled.
	private static extern bool OVR_IsPowerSaveActive();

	[DllImport("OculusPlugin")]
	// Support to fix 60/30/20 FPS frame rate for consistency or power savings.
	private static extern void OVR_TW_SetMinimumVsyncs(OVRTimeWarpUtils.VsyncMode mode);
#endif

#region Member Variables

	public OVRGamepadController.Button	resetButton = OVRGamepadController.Button.X;

#endregion

	/// <summary>
	/// Enable vr mode parm configuration and invoke power state mode test.
	/// </summary>
	void OnEnable()
	{
#if (UNITY_ANDROID && !UNITY_EDITOR)
		OVRManager.OnConfigureVrModeParms += ConfigureVrModeParms;
		
		// Call TestPowerLevelState after 10 seconds 
		// and repeats every 10 seconds.
		InvokeRepeating ("TestPowerStateMode", 10, 10.0f);
#endif
	}

	/// <summary>
	/// Disable vr mode parm configuration and un-invoke power state mode test.
	/// </summary>
	void OnDisable()
	{
#if (UNITY_ANDROID && !UNITY_EDITOR)
		OVRManager.OnConfigureVrModeParms -= ConfigureVrModeParms;

		CancelInvoke();
#endif
	}

	/// <summary>
	/// Change default vr mode parms dynamically.
	/// </summary>
	void Update()
	{
		// NOTE: some of the buttons defined in OVRGamepadController.Button are not available on the Android game pad controller
		if ( Input.GetButtonDown(OVRGamepadController.ButtonNames[(int)resetButton])) 
		{
#if (UNITY_ANDROID && !UNITY_EDITOR)
			OVR_VrModeParms_SetCpuLevel(0);
			OVR_VrModeParms_SetGpuLevel(1);
			OVRPluginEvent.Issue(RenderEventType.ResetVrModeParms);
#endif
		}
	}

	/// <summary>
	/// Check current power state mode.
	/// </summary>
	void TestPowerStateMode()
	{
#if (UNITY_ANDROID && !UNITY_EDITOR)
		//*************************
		// Check power-level state mode
		//*************************
		if (OVR_IsPowerSaveActive())
		{
			// The device has been throttled
			Debug.Log("POWER SAVE MODE ACTIVATED");
		}
#endif
	}

	/// <summary>
	/// Configure vr mode parms.
	/// </summary>
	void ConfigureVrModeParms()
	{
#if (UNITY_ANDROID && !UNITY_EDITOR)
		// De-clock to reduce power and thermal load.
		
		// Performance mode (default)
		OVR_VrModeParms_SetCpuLevel(2);
		OVR_VrModeParms_SetGpuLevel(2);
		OVR_TW_SetMinimumVsyncs(OVRTimeWarpUtils.VsyncMode.VSYNC_60FPS);
		
		// Power-save levels
		//OVR_VrModeParms_SetCpuLevel(0);
		//OVR_VrModeParms_SetGpuLevel(0);
		//OVR_TW_SetMinimumVsyncs(OVRTimeWarpUtils.VsyncMode.VSYNC_30FPS);
		
		// Enable Power Save Mode Handling
		OVR_VrModeParms_SetAllowPowerSave(true);
#endif
	}
}
