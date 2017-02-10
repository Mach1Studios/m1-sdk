//
//  Mach1 SDK
//
#pragma once

#include "GameFramework/Actor.h"
#include <vector>

#include "M1BaseActor.generated.h" 

UCLASS(abstract)
class M1SOUNDPLUGIN_API AM1BaseActor : public AActor // M1SOUNDPLUGIN_API - change to your project name
{
	GENERATED_BODY()

protected:
	// geometric utils
	static float ClosestPointOnBox(FVector point, FVector center, FVector axis0, FVector axis1, FVector axis2, FVector extents, FVector& closestPoint);
	
	static bool Clip(float denom, float numer, float& t0, float& t1);
	static int DoClipping(float t0, float t1, FVector origin, FVector direction, FVector center, FVector axis0, FVector axis1, FVector axis2, FVector extents, bool solid, FVector& point0, FVector& point1);
	
	USoundAttenuation* NullAttenuation;
	
	TArray<float> VolumeFactor;

	TArray<USoundWave*> SoundsWalls;
	TArray<UAudioComponent*> LeftChannelsWalls;
	TArray<UAudioComponent*> RightChannelsWalls;

	TArray<USoundWave*> SoundsCenter;
	TArray<UAudioComponent*> LeftChannelsCenter;
	TArray<UAudioComponent*> RightChannelsCenter;

	USceneComponent* Root;
	UBoxComponent* Collision;
	UBillboardComponent* Billboard;

	int MAX_SOUNDS_PER_CHANNEL;
	bool isInit;

	void Init();
	void SetSoundSet();
	void CalculateChannelVolumes(FRotator rotator, FQuat quat);
	void SetVolumeWalls(float volume);
	void SetVolumeCenter(float volume);

	virtual void SetSoundsWalls() {};
	virtual void SetSoundsCenter() {};
	virtual std::vector<float> SoundAlgorithm(float Yaw, float Pitch, float Roll) { return std::vector<float>(); };

public:
	// Sets default values for this actor's properties
	void Init(int MAX_SOUNDS_PER_CHANNEL);

	// Called when the game starts or when spawned
	void BeginPlay();

	// Called every frame
	void Tick(float DeltaSeconds);

	// always tick
	bool ShouldTickIfViewportsOnly() const override { return true; }

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Force HMD rotation instead of Player Controller")
		bool ForceHMDRotation = true;

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void Play();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void Stop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", DisplayName = "Fade In Duration")
		float fadeInDuration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", DisplayName = "Fade Out Duration")
		float fadeOutDuration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Attenuation Curve")
		UCurveFloat* attenuationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Display Debug")
		bool Debug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Rotator mode")
		bool useRotator = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Closest Point/Activate Exterior Sounds")
		bool useClosestPoint = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Yaw for Positional Rotation")
		bool useYaw = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Pitch for Positional Rotation")
		bool usePitch = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Roll for Positional Rotation")
		bool useRoll = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", DisplayName = "Volume")
		float Volume = 1.0f;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use RoomMode")
		bool useRoomMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Attenuation RoomMode Curve")
		UCurveFloat* attenuationRoomModeCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Ignore Top Bottom Walls")
		bool ignoreTopBottom = true;


};
