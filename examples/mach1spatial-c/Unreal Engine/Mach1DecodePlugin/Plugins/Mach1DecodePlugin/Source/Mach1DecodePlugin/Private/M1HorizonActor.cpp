//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

#include "Mach1DecodePluginPrivatePCH.h"

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

AM1HorizonActor::AM1HorizonActor()
{
	InitComponents(4);

	mach1Decode.setDecodeAlgoType(Mach1DecodeAlgoType::Mach1DecodeAlgoHorizon);
}
 