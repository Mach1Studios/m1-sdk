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
	std::vector<float> SoundAlgorithm(float Yaw, float Pitch, float Roll);

public:
	// Sets default values for this actor's properties
	AM1SpatialActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "Sound INT")
		USoundWave* FrontUpLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound INT")
		USoundWave* FrontUpRightChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound INT")
		USoundWave* BackUpLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound INT")
		USoundWave* BackUpRightChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound INT")
		USoundWave* FrontDownLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound INT")
		USoundWave* FrontDownRightChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound INT")
		USoundWave* BackDownLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound INT")
		USoundWave* BackDownRightChannelWalls;


	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "RoomMode Wall EXT")
		USoundWave* FrontUpLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "RoomMode Wall EXT")
		USoundWave* FrontUpRightChannelCenter;

	UPROPERTY(EditAnywhere, Category = "RoomMode Wall EXT")
		USoundWave* BackUpLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "RoomMode Wall EXT")
		USoundWave* BackUpRightChannelCenter;

	UPROPERTY(EditAnywhere, Category = "RoomMode Wall EXT")
		USoundWave* FrontDownLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "RoomMode Wall EXT")
		USoundWave* FrontDownRightChannelCenter;

	UPROPERTY(EditAnywhere, Category = "RoomMode Wall EXT")
		USoundWave* BackDownLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "RoomMode Wall EXT")
		USoundWave* BackDownRightChannelCenter;
};
