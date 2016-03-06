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
using UnityEditor;
using UnityEditor.Callbacks;
using System;
using System.Linq;
using System.Text.RegularExpressions;
using System.IO;

//-------------------------------------------------------------------------------------
// ***** OculusBuildDemo
//
// OculusBuild allows for command line building of the Oculus main demo (Tuscany).
//
[InitializeOnLoad]
class OVRShimLoader
{
	static OVRShimLoader()
	{
		if (EditorUserBuildSettings.activeBuildTarget != BuildTarget.StandaloneWindows &&
		    EditorUserBuildSettings.activeBuildTarget != BuildTarget.StandaloneWindows64)
			return;

		PlayerSettings.displayResolutionDialog = ResolutionDialogSetting.HiddenByDefault;

		_useAutoPatcher = !OVRUnityVersionChecker.hasPreInitSupport;

        //Debug.Log("Version is " + OVRUnityVersionChecker.version.ToString() + " (" + Application.unityVersion + ")");
        //Debug.Log("Use Auto Patcher? " + _useAutoPatcher);
        //Debug.Log("Use Exclusive mode D3D11? " + OVRUnityVersionChecker.hasD3D11ExclusiveModeSupport);

#if UNITY_5_0 || UNITY_5_1 || UNITY_5_2 || UNITY_5_3
		PlayerSettings.d3d11FullscreenMode =
			(OVRUnityVersionChecker.hasD3D11ExclusiveModeSupport) ? D3D11FullscreenMode.ExclusiveMode : D3D11FullscreenMode.FullscreenWindow;
#else
        PlayerSettings.d3d11ForceExclusiveMode = OVRUnityVersionChecker.hasD3D11ExclusiveModeSupport;
#endif

		PlayerSettings.d3d9FullscreenMode =
			(OVRUnityVersionChecker.hasD3D9ExclusiveModeSupport) ? D3D9FullscreenMode.ExclusiveMode : D3D9FullscreenMode.FullscreenWindow;

		PlayerSettings.visibleInBackground = true;
	}

	[PreferenceItem("Oculus VR")]
	static void PreferencesGUI()
	{
		// Load the preferences
		if (!_prefsLoaded) {
			_isEnabled = EditorPrefs.GetBool("OculusBuild", false);
			_prefsLoaded = true;
		}
		
		// Preferences GUI

		bool isEnabled = EditorGUILayout.Toggle("Optimize Builds for Rift", _isEnabled);

		if (isEnabled && !_isEnabled &&
		    (EditorUserBuildSettings.activeBuildTarget == BuildTarget.StandaloneWindows ||
		    EditorUserBuildSettings.activeBuildTarget == BuildTarget.StandaloneWindows64))
			PlayerSettings.displayResolutionDialog = ResolutionDialogSetting.HiddenByDefault;

		_isEnabled = isEnabled;
		
		// Save the preferences
		if (GUI.changed)
			EditorPrefs.SetBool("OculusBuild", _isEnabled);
	}

	/// <summary>
	/// Replaces the built standalone with an auto-patcher for the same architecture.
	/// </summary>
	[PostProcessBuild]
	public static void OnPostProcessBuild(BuildTarget target, string pathToBuiltProject)
	{
        if (_useAutoPatcher)
        {
            // Figure out which architecture we're building for.
            int arch;
            if (target == BuildTarget.StandaloneWindows)
                arch = 32;
            else if (target == BuildTarget.StandaloneWindows64)
                arch = 64;
            else
                return;

            // Rename the target to a .bin file for the auto-patcher to find later.
            string autoPatcherPath = Application.dataPath + "/OVR/Editor/OculusUnityPatcher_" + arch.ToString() + ".exe";
            string targetPath = pathToBuiltProject.Replace(".exe", "_DirectToRift.exe");

            if (File.Exists(targetPath))
                File.Delete(targetPath);

            File.Copy(autoPatcherPath, targetPath);
        }

		string appInfoPath = pathToBuiltProject.Replace(".exe", "_Data/OVRAppInfo");
		var file = new System.IO.StreamWriter(appInfoPath);
		file.Write(PlayerSettings.companyName + "\\" + PlayerSettings.productName);
		file.Dispose();
	}

	static bool _isEnabled = true;
	static bool _prefsLoaded = false;
    static bool _useAutoPatcher = true;
}
