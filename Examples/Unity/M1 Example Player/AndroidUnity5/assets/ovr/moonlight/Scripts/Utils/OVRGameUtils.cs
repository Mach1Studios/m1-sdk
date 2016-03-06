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
using System.Collections.Generic;

public class OVRGameUtils
{
	static public GameObject FindChildNoWarning(GameObject obj, string name)
	{
		if (obj.name == name)
		{
			return obj;
		}

		foreach (Transform child in obj.transform)
		{
			GameObject found = FindChildNoWarning(child.gameObject, name);
			if (found)
			{
				return found;
			}
		}

		return null;
	}

	static public GameObject FindChild(GameObject obj, string name)
	{
		GameObject go = FindChildNoWarning(obj, name);
		if (go == null)
		{
			OVRDebugUtils.Print( "child " + name + " was not found!" );
		}

		return go;
	}
};
