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
		USoundWave* FrontUpLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* FrontUpRightChannelWalls;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* BackUpLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* BackUpRightChannelWalls;


	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* FrontUpLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* FrontUpRightChannelCenter;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* BackUpLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* BackUpRightChannelCenter;

};
