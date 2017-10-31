//
//  Mach1 SDK
//
#include "M1SoundPluginPrivatePCH.h" // Change to your project name!

#include "M1HorizonActor.h"

void AM1HorizonActor::SetSoundsWalls()
{
	SoundsWalls.Add(ChannelWall1);
	SoundsWalls.Add(ChannelWall2);
	SoundsWalls.Add(ChannelWall3);
	SoundsWalls.Add(ChannelWall4);
}

void AM1HorizonActor::SetSoundsCenter()
{
	SoundsCenter.Add(ChannelCenter1);
	SoundsCenter.Add(ChannelCenter2);
	SoundsCenter.Add(ChannelCenter3);
	SoundsCenter.Add(ChannelCenter4);
}

std::vector<float> AM1HorizonActor::SoundAlgorithm(float Yaw, float Pitch, float Roll) 
{
	return m1.horizonAlgo(Yaw, Pitch, Roll);
}

AM1HorizonActor::AM1HorizonActor()
{
	InitComponents(4);

	m1.setAngularSettingsType(Mach1Decode::m1UE);
}
 