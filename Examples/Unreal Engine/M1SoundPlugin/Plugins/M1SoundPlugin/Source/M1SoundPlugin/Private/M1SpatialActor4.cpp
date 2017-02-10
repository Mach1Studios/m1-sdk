//
//  Mach1 SDK
//
#include "M1SoundPluginPrivatePCH.h" // Change to your project name!

#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
//#include "HeadMountedDisplay.h"

#include <sstream>
#include "M1SpatialActor4.h"

#include "M1DSPAlgorithmsUE.h"

void AM1SpatialActor4::SetSoundsWalls()
{
	SoundsWalls.Add(FrontUpLeftChannelWalls);
	SoundsWalls.Add(FrontUpRightChannelWalls);
	SoundsWalls.Add(BackUpLeftChannelWalls);
	SoundsWalls.Add(BackUpRightChannelWalls);
}

void AM1SpatialActor4::SetSoundsCenter()
{
	SoundsCenter.Add(FrontUpLeftChannelCenter);
	SoundsCenter.Add(FrontUpRightChannelCenter);
	SoundsCenter.Add(BackUpLeftChannelCenter);
	SoundsCenter.Add(BackUpRightChannelCenter);
}

std::vector<float> AM1SpatialActor4::SoundAlgorithm(float Yaw, float Pitch, float Roll) 
{
	return fourChannelAlgorithm(Yaw, Pitch, Roll);
}

AM1SpatialActor4::AM1SpatialActor4()
{
	AM1BaseActor::Init(4);
}

// Called when the game starts or when spawned
void AM1SpatialActor4::BeginPlay()
{
	Super::BeginPlay();

	AM1BaseActor::BeginPlay();
}

// Called every frame
void AM1SpatialActor4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AM1BaseActor::Tick(DeltaTime);
}
