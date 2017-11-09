//
//  Mach1 SDK
//
#include "Mach1DecodePluginPrivatePCH.h" // Change to your project name!

#include "M1HorizonActor.h"

void AM1HorizonActor::SetSoundsMain()
{
	SoundsMain.Add(ChannelMain1);
	SoundsMain.Add(ChannelMain2);
	SoundsMain.Add(ChannelMain3);
	SoundsMain.Add(ChannelMain4);
}

void AM1HorizonActor::SetSoundsBlendMode()
{
	SoundsBlendMode.Add(ChannelBlend1);
	SoundsBlendMode.Add(ChannelBlend2);
	SoundsBlendMode.Add(ChannelBlend3);
	SoundsBlendMode.Add(ChannelBlend4);
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
 