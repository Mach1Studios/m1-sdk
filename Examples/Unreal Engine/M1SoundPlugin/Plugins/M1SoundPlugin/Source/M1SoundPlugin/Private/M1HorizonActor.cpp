//
//  Mach1 SDK
//
#include "M1SoundPluginPrivatePCH.h" // Change to your project name!

#include "M1HorizonActor.h"
#include "M1DSPAlgorithmsUE.h"

void AM1HorizonActor::SetSoundsWalls()
{
	SoundsWalls.Add(FrontUpLeftChannelWalls);
	SoundsWalls.Add(FrontUpRightChannelWalls);
	SoundsWalls.Add(BackUpLeftChannelWalls);
	SoundsWalls.Add(BackUpRightChannelWalls);
}

void AM1HorizonActor::SetSoundsCenter()
{
	SoundsCenter.Add(FrontUpLeftChannelCenter);
	SoundsCenter.Add(FrontUpRightChannelCenter);
	SoundsCenter.Add(BackUpLeftChannelCenter);
	SoundsCenter.Add(BackUpRightChannelCenter);
}

std::vector<float> AM1HorizonActor::SoundAlgorithm(float Yaw, float Pitch, float Roll) 
{
	return fourChannelAlgorithm(Yaw, Pitch, Roll);
}

AM1HorizonActor::AM1HorizonActor()
{
	InitComponents(4);
}
 