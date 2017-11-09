//
//  Mach1 SDK
//
#include "M1SoundPluginPrivatePCH.h" // Change to your project name!

#include "M1SpatialActor.h"

void AM1SpatialActor::SetSoundsWalls()
{
	SoundsWalls.Add(ChannelWall1);
	SoundsWalls.Add(ChannelWall2);
	SoundsWalls.Add(ChannelWall3);
	SoundsWalls.Add(ChannelWall4);
	SoundsWalls.Add(ChannelWall5);
	SoundsWalls.Add(ChannelWall6);
	SoundsWalls.Add(ChannelWall7);
	SoundsWalls.Add(ChannelWall8);
}

void AM1SpatialActor::SetSoundsCenter()
{
	SoundsCenter.Add(ChannelCenter1);
	SoundsCenter.Add(ChannelCenter2);
	SoundsCenter.Add(ChannelCenter3);
	SoundsCenter.Add(ChannelCenter4);
	SoundsCenter.Add(ChannelCenter5);
	SoundsCenter.Add(ChannelCenter6);
	SoundsCenter.Add(ChannelCenter7);
	SoundsCenter.Add(ChannelCenter8);

}

std::vector<float> AM1SpatialActor::SoundAlgorithm(float Yaw, float Pitch, float Roll) 
{
	return m1.spatialAlgo(Yaw, Pitch, Roll);
}

AM1SpatialActor::AM1SpatialActor() 
{
	InitComponents(8);

	m1.setAngularSettingsType(Mach1Decode::m1UE);
}
 
