//
//  Mach1 SDK
//
#include "M1SoundPluginPrivatePCH.h" // Change to your project name!

#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
//#include "HeadMountedDisplay.h"

#include <sstream>
#include "M1SpatialActor8.h"

#include "M1DSPAlgorithmsUE.h"

void AM1SpatialActor8::SetSoundsWalls()
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

void AM1SpatialActor8::SetSoundsCenter()
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

std::vector<float> AM1SpatialActor8::SoundAlgorithm(float Yaw, float Pitch, float Roll) 
{
	return eightChannelsIsotropicAlgorithm(Yaw, Pitch, Roll);
}

AM1SpatialActor8::AM1SpatialActor8()
{
	AM1BaseActor::Init(8);
}

// Called when the game starts or when spawned
void AM1SpatialActor8::BeginPlay()
{
	Super::BeginPlay();

	AM1BaseActor::BeginPlay();
}

// Called every frame
void AM1SpatialActor8::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AM1BaseActor::Tick(DeltaTime);
}
