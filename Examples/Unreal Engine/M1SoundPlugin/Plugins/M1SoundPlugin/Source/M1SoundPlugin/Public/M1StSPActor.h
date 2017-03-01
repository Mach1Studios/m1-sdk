//
//  Mach1 SDK
//
#pragma once

#include "GameFramework/Actor.h"

#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"

#include "M1StSPActor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class M1SOUNDPLUGIN_API AM1StSPActor : public AActor // M1SOUNDPLUGIN_API - change to your project name
{
	GENERATED_BODY()

	USceneComponent* Root;

	UAudioComponent* audioComponentMidLeft;
	UAudioComponent* audioComponentMidRight;
	UAudioComponent* audioComponent;

	USoundWave* soundWaveMidLeft;
	USoundWave* soundWaveMidRight;

public:
	// Sets default values for this actor's properties
	AM1StSPActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	USoundWave* MakeSoundWaveFromBuffer(FName PlatformFormat, TArray<uint8>& rawFile, FSoundQualityInfo QualityInfo);

	// always tick
	bool ShouldTickIfViewportsOnly() const override { return true; }

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundWave * soundWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Attenuation Curve")
		UCurveFloat* attenuationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin|Audio", DisplayName = "Fade In Duration")
		float fadeInDuration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin|Audio", DisplayName = "Fade Out Duration")
		float fadeOutDuration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin|Sound", meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float Spatialize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin|Sound", DisplayName = "Volume")
		float Volume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Display Debug")
		bool Debug = true;

	UFUNCTION(BlueprintCallable, Category = "M1 Plugin|Audio")
		void Play();

	UFUNCTION(BlueprintCallable, Category = "M1 Plugin|Audio")
		void Stop();
};
