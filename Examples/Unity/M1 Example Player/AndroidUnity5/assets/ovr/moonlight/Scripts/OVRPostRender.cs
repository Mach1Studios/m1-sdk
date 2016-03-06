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
using UnityEngine;
using System.Collections; // required for Coroutines

public class OVRPostRender : MonoBehaviour
{
#if UNITY_ANDROID && !UNITY_EDITOR
	public static event Action OnCustomPostRender = null;
#endif

	void OnPostRender()
	{
#if UNITY_ANDROID && !UNITY_EDITOR
		// Allow custom code to render before we kick off the plugin
		if (OnCustomPostRender != null)
		{
			OnCustomPostRender();
		}

		OVREye eye = ((RenderEventType)Camera.current.depth == RenderEventType.RightEyeEndFrame) ?
			OVREye.Right : OVREye.Left;
		OVRManager.EndEye(eye);
#endif
	}
}
