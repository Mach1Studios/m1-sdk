//
//  Mach1 SDK
//
#pragma once

#include "GameFramework/Actor.h"
#include "M1BaseActor.h"
#include "M1HorizonActor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class M1SOUNDPLUGIN_API AM1HorizonActor : public AM1BaseActor
{
	GENERATED_BODY()

	void SetSoundsWalls();
	void SetSoundsCenter();

	virtual std::vector<float> SoundAlgorithm(float Yaw, float Pitch, float Roll)  override;

public:
	// Sets default values for this actor's properties
	AM1HorizonActor();

	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall1;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall2;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall3;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall4;


	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter1;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter2;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter3;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter4;

};
