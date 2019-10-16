//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

#include "M1SpatialActor.h"

#include "Mach1DecodePluginPrivatePCH.h"

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

AM1SpatialActor::AM1SpatialActor() 
{
	InitComponents(8);

#ifdef LEGACY_POSITIONAL
	mach1Decode.setDecodeAlgoType(Mach1DecodeAlgoType::Mach1DecodeAlgoSpatial);
#else 
	m1Positional.setDecodeAlgoType(Mach1DecodeAlgoType::Mach1DecodeAlgoSpatial);
#endif	
}
 
