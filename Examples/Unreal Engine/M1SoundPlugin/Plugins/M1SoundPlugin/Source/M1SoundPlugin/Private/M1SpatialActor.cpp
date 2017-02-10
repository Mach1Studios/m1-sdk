//
//  Mach1 SDK
//
#include "M1SoundPluginPrivatePCH.h" // Change to your project name!

#include "M1SpatialActor.h"
#include "M1DSPAlgorithmsUE.h"

void AM1SpatialActor::SetSoundsWalls()
{
	SoundsWalls.Add(FrontUpLeftChannelWalls);
	SoundsWalls.Add(FrontUpRightChannelWalls);
	SoundsWalls.Add(BackUpLeftChannelWalls);
	SoundsWalls.Add(BackUpRightChannelWalls);
	SoundsWalls.Add(FrontDownLeftChannelWalls);
	SoundsWalls.Add(FrontDownRightChannelWalls);
	SoundsWalls.Add(BackDownLeftChannelWalls);
	SoundsWalls.Add(BackDownRightChannelWalls);
}

void AM1SpatialActor::SetSoundsCenter()
{
	SoundsCenter.Add(FrontUpLeftChannelCenter);
	SoundsCenter.Add(FrontUpRightChannelCenter);
	SoundsCenter.Add(BackUpLeftChannelCenter);
	SoundsCenter.Add(BackUpRightChannelCenter);
	SoundsCenter.Add(FrontDownLeftChannelCenter);
	SoundsCenter.Add(FrontDownRightChannelCenter);
	SoundsCenter.Add(BackDownLeftChannelCenter);
	SoundsCenter.Add(BackDownRightChannelCenter);

}

std::vector<float> AM1SpatialActor::SoundAlgorithm(float Yaw, float Pitch, float Roll) 
{
	return eightChannelsIsotropicAlgorithm(Yaw, Pitch, Roll);
}

AM1SpatialActor::AM1SpatialActor() 
{
	InitComponents(8);
}
 
