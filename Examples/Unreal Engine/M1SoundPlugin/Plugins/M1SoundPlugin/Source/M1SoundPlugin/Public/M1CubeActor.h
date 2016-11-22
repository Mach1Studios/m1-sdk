//
//  Mach1 SDK
//
#pragma once

#include "GameFramework/Actor.h"
#include "M1CubeActor.generated.h" 

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class M1SOUNDPLUGIN_API AM1CubeActor : public AActor // M1SOUNDPLUGIN_API - change to your project name
{
	GENERATED_BODY()

	static float ClosestPointOnBox(FVector point, FVector center, FVector axis0, FVector axis1, FVector axis2, FVector extents, FVector& closestPoint);
 
	USoundAttenuation* NullAttenuation;
	TArray<USoundBase*> Sounds;
	TArray<float> VolumeFactor;

	TArray<UAudioComponent*> LeftChannels;
	TArray<UAudioComponent*> RightChannels;

	USceneComponent* Root;
	UBoxComponent* Collision;
	UBillboardComponent* Billboard;

	bool isInit;

	void Init();
	void SetSoundSet();
	void CalculateChannelVolumes(FRotator rotator, FQuat quat);
	void SetVolume(float Volume);

public:
	// Sets default values for this actor's properties
	AM1CubeActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// always tick
	bool ShouldTickIfViewportsOnly() const override { return true; }

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void Play();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void Stop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Attenuation Curve")
		UCurveFloat* attenuationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Display Debug")
		bool Debug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Yaw for Positional Rotation")
		bool useYaw = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Pitch for Positional Rotation")
		bool usePitch = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Roll for Positional Rotation")
		bool useRoll = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", DisplayName = "Volume")
		float Volume = 1.0f;

	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontUpLeftChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontUpRightChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackUpLeftChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackUpRightChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontDownLeftChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontDownRightChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackDownLeftChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackDownRightChannel;
};
