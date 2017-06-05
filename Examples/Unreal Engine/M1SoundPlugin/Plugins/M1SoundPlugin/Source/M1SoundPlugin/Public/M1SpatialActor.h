//
//  Mach1 SDK
//
#pragma once

#include "GameFramework/Actor.h"
#include "M1BaseActor.h"
#include "M1SpatialActor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class M1SOUNDPLUGIN_API AM1SpatialActor : public AM1BaseActor
{
	GENERATED_BODY()

	void SetSoundsWalls();
	void SetSoundsCenter();

	virtual std::vector<float> SoundAlgorithm(float Yaw, float Pitch, float Roll)  override;

public:
	// Sets default values for this actor's properties
	AM1SpatialActor();

	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall1;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall2;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall3;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall4;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall5;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall6;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall7;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound INT")
		USoundWave* ChannelWall8;


	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter1;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter2;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter3;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter4;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter5;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter6;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter7;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|RoomMode Wall EXT")
		USoundWave* ChannelCenter8;
};
