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

public class OVRTimeWarpUtils
{
	public enum VsyncMode
	{
		VSYNC_60FPS = 1,
		VSYNC_30FPS = 2,
		VSYNC_20FPS = 3
	}

	public enum DebugPerfMode
	{
		DEBUG_PERF_OFF,         // data still being collected, just not displayed
		DEBUG_PERF_RUNNING,     // display continuously changing graph
		DEBUG_PERF_FROZEN,      // no new data collection, but displayed
		DEBUG_PERF_MAX,
	}

	public enum DebugPerfValue
	{
		DEBUG_VALUE_DRAW,       // start and end times of the draw
		DEBUG_VALUE_LATENCY,    // seconds from eye buffer orientation time
		DEBUG_VALUE_MAX,
	}

#if UNITY_ANDROID && !UNITY_EDITOR
	// Support to fix 60/30/20 FPS frame rate for consistency or power savings
	[DllImport("OculusPlugin")]
	private static extern void OVR_TW_SetMinimumVsyncs(VsyncMode mode);

	[DllImport("OculusPlugin")]
	private static extern void OVR_TW_SetDebugMode(DebugPerfMode mode, DebugPerfValue val);

	[DllImport("OculusPlugin")]
	private static extern void OVR_TW_EnableChromaticAberration(bool enable);

	// Allow TW to increase the fov by about 10 degrees if we are not holding 60 fps
	// so there is less black pull-in at the edges.
	[DllImport("OculusPlugin")]
	private static extern void OVR_TW_AllowFovIncrease(bool allow);
#endif
}
