//
//  Mach1 SDK
//
#include "Mach1DecodePluginPrivatePCH.h"

#include "M1SpatialActor.h"

void AM1SpatialActor::SetSoundsMain()
{
	SoundsMain.Add(ChannelMain1);
	SoundsMain.Add(ChannelMain2);
	SoundsMain.Add(ChannelMain3);
	SoundsMain.Add(ChannelMain4);
	SoundsMain.Add(ChannelMain5);
	SoundsMain.Add(ChannelMain6);
	SoundsMain.Add(ChannelMain7);
	SoundsMain.Add(ChannelMain8);
}

void AM1SpatialActor::SetSoundsBlendMode()
{
	SoundsBlendMode.Add(ChannelBlend1);
	SoundsBlendMode.Add(ChannelBlend2);
	SoundsBlendMode.Add(ChannelBlend3);
	SoundsBlendMode.Add(ChannelBlend4);
	SoundsBlendMode.Add(ChannelBlend5);
	SoundsBlendMode.Add(ChannelBlend6);
	SoundsBlendMode.Add(ChannelBlend7);
	SoundsBlendMode.Add(ChannelBlend8);

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
 
