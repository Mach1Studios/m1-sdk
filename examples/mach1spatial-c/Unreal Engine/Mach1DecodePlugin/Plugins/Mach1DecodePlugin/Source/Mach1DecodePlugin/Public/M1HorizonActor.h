//
//  Mach1 SDK
//
#pragma once

#include "GameFramework/Actor.h"
#include "M1BaseActor.h"
#include "M1HorizonActor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MACH1DECODEPLUGIN_API AM1HorizonActor : public AM1BaseActor
{
	GENERATED_BODY()

	void SetSoundsMain();
	void SetSoundsBlendMode();

	virtual std::vector<float> SoundAlgorithm(float Yaw, float Pitch, float Roll)  override;

public:
	// Sets default values for this actor's properties
	AM1HorizonActor();

	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "Mach1 Spatial Files|Main")
		USoundWave* ChannelMain1;

	UPROPERTY(EditAnywhere, Category = "Mach1 Spatial Files|Main")
		USoundWave* ChannelMain2;

	UPROPERTY(EditAnywhere, Category = "Mach1 Spatial Files|Main")
		USoundWave* ChannelMain3;

	UPROPERTY(EditAnywhere, Category = "Mach1 Spatial Files|Main")
		USoundWave* ChannelMain4;


	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "Mach1 Spatial Files|BlendMode")
		USoundWave* ChannelBlend1;

	UPROPERTY(EditAnywhere, Category = "Mach1 Spatial Files|BlendMode")
		USoundWave* ChannelBlend2;

	UPROPERTY(EditAnywhere, Category = "Mach1 Spatial Files|BlendMode")
		USoundWave* ChannelBlend3;

	UPROPERTY(EditAnywhere, Category = "Mach1 Spatial Files|BlendMode")
		USoundWave* ChannelBlend4;

};
