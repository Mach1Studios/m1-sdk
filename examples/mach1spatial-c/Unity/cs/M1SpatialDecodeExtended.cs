//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

using UnityEngine;
using System.Collections;
using System.IO;

public class M1SpatialDecodeExtended : M1Base
{
	enum MODE
	{
		Headtracking,
		Stereo
	};

	MODE mode;

	public void SetHeadtrackingMode()
	{
		mode = MODE.Headtracking;
	}

	public void SetStereoMode()
	{
		mode = MODE.Stereo;
	}

	float ClampAngle(float angle, float from, float to) {
		if(angle > 180) angle = 360 - angle;
		angle = Mathf.Clamp(angle, from, to);
		if(angle < 0) angle = 360 + angle;


		return angle;
	}

	public M1SpatialDecodeExtended()
	{
		InitComponents(8); 
	}

	public override float[] SoundAlgorithm(float Yaw, float Pitch, float Roll)
	{
		switch (mode)
		{
		case MODE.Headtracking:
			break;
		case MODE.Stereo:
			Yaw = 0;
			Pitch = 0;
			Roll = 0;
			break;

			GameObject playControl = GameObject.Find ("PlayControl");
			float centerYawAngle = playControl.GetComponent<playcontrol> ().centerYawAngle;
			if (centerYawAngle < -1000f) {
				centerYawAngle = Pitch; // lol TODO: fix this mess
				playControl.GetComponent<playcontrol> ().centerYawAngle = centerYawAngle;
			}

			GameObject text = GameObject.Find ("test_txt");
			//text.GetComponent<Text>().text = "Y:" + Yaw.ToString() + " P:" + Pitch.ToString() + " R:" + Roll.ToString();

			float normalisedPitchAngle = Pitch;//ClampAngle (Pitch - centerYawAngle, -180, 180);

			float finalStereoshiftAngle = Pitch;
			//float finalStereoshiftAngle = Mathf.Clamp (Pitch, -35, 35);

			finalStereoshiftAngle = Pitch;

			float limit = 60;

			if ((Pitch < (360 - limit)) && (Pitch > 180)) {
				finalStereoshiftAngle = 360 - limit;
			} else if ((Pitch > limit) && (Pitch <= 180)) {
				finalStereoshiftAngle = limit;
			}

			Pitch = finalStereoshiftAngle;
			Roll = 0;

			//			text.GetComponent<Text> ().text = "cY:" + finalStereoshiftAngle.ToString() + " ; Pitch: " + Pitch.ToString();
			break;
		}

		return m1Decode.spatialAlgo(Yaw, Pitch, Roll);
	}
}
