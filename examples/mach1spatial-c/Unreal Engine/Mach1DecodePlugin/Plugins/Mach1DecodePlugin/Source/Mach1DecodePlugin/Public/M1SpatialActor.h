//
//  Mach1 SDK
//
#pragma once

#include "GameFramework/Actor.h"
#include "M1BaseActor.h"
#include "M1SpatialActor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MACH1DECODEPLUGIN_API AM1SpatialActor : public AM1BaseActor
{
	GENERATED_BODY()

	void SetSoundsMain();
	void SetSoundsBlendMode();

	virtual std::vector<float> SoundAlgorithm(float Yaw, float Pitch, float Roll)  override;

public:
	// Sets default values for this actor's properties
	AM1SpatialActor();

	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound Main")
		USoundWave* ChannelMain1;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound Main")
		USoundWave* ChannelMain2;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound Main")
		USoundWave* ChannelMain3;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound Main")
		USoundWave* ChannelMain4;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound Main")
		USoundWave* ChannelMain5;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound Main")
		USoundWave* ChannelMain6;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound Main")
		USoundWave* ChannelMain7;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|Sound Main")
		USoundWave* ChannelMain8;


	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "M1 Plugin|BlendMode EXT")
		USoundWave* ChannelBlend1;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|BlendMode EXT")
		USoundWave* ChannelBlend2;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|BlendMode EXT")
		USoundWave* ChannelBlend3;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|BlendMode EXT")
		USoundWave* ChannelBlend4;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|BlendMode EXT")
		USoundWave* ChannelBlend5;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|BlendMode EXT")
		USoundWave* ChannelBlend6;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|BlendMode EXT")
		USoundWave* ChannelBlend7;

	UPROPERTY(EditAnywhere, Category = "M1 Plugin|BlendMode EXT")
		USoundWave* ChannelBlend8;
};
