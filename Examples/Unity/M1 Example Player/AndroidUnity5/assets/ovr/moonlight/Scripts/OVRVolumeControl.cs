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

public class OVRVolumeControl : MonoBehaviour
{
	private const float 		showPopupTime = 3;
	private const float			popupOffsetY = 64.0f / 500.0f;
	private const float			popupDepth = 1.8f;
	private const int 			maxVolume = 15;
	private const int 			numVolumeImages = maxVolume + 1;
	
	private Transform			myTransform = null;
	
	void Start()
	{
		DontDestroyOnLoad( gameObject );
		myTransform = transform;
		GetComponent<Renderer>().enabled = false;
		
	}
	
	/// <summary>
	/// Updates the position of the volume popup. 
	/// </summary>
	public virtual void UpdatePosition(Transform cameraTransform)
	{
		// OVRDevice.GetTimeSinceLastVolumeChange() will return -1 if the volume listener hasn't initialized yet,
		// which sometimes takes place after a frame has run in Unity.
		double timeSinceLastVolumeChange = OVRManager.timeSinceLastVolumeChange;
		if ((timeSinceLastVolumeChange != -1) && (timeSinceLastVolumeChange < showPopupTime))
		{
			GetComponent<Renderer>().enabled = true;
			GetComponent<Renderer>().material.mainTextureOffset = new Vector2(0.0f, (float)(maxVolume - OVRManager.volumeLevel) / (float)numVolumeImages);
			if (myTransform != null && cameraTransform != null)
			{
				// place in front of camera
				myTransform.rotation = cameraTransform.rotation;
				myTransform.position = cameraTransform.position + (myTransform.forward * popupDepth) + (myTransform.up * popupOffsetY);
			}
		}
		else
		{
			GetComponent<Renderer>().enabled = false;
		}
	}
}
