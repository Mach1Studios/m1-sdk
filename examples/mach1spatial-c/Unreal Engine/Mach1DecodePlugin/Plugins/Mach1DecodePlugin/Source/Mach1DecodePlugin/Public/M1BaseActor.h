//
//  Mach1 SDK
//
#pragma once

#include "GameFramework/Actor.h"
#include "Mach1Decode.h"

#include <vector>

#include "M1BaseActor.generated.h"

UCLASS(abstract)
class MACH1DECODEPLUGIN_API AM1BaseActor : public AActor // M1SOUNDPLUGIN_API - change to your project name
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
	bool needPlayAfterInit;

	void Init();
	void SetSoundSet();
	void CalculateChannelVolumes(FQuat quat);
	void SetVolumeWalls(float volume);
	void SetVolumeCenter(float volume);

	virtual void SetSoundsWalls() {};
	virtual void SetSoundsCenter() {};
	virtual std::vector<float> SoundAlgorithm(float Yaw, float Pitch, float Roll) { return std::vector<float>(); };

	Mach1Decode m1;

public:

	void InitComponents(int MAX_SOUNDS_PER_CHANNEL);

	// Called when the game starts or when spawned
	void BeginPlay();

	// Called every frame
	void Tick(float DeltaSeconds);

	// always tick
	bool ShouldTickIfViewportsOnly() const override { return true; }
	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Force HMD rotation instead of Player Controller")
		bool ForceHMDRotation = true;

	UFUNCTION(BlueprintCallable, Category = "M1 Plugin|Audio")
		void Play();

	UFUNCTION(BlueprintCallable, Category = "M1 Plugin|Audio")
		void Stop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin|Fade", DisplayName = "Fade In Duration")
		float fadeInDuration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin|Fade", DisplayName = "Fade Out Duration")
		float fadeOutDuration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Attenuation Curve")
		UCurveFloat* attenuationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Display Debug")
		bool Debug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Use Blend Mode")
		bool useBlendMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Mute When Inside Object")
		bool muteWhenInsideObject = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Mute When Outside Object")
		bool muteWhenOutsideObject = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Use Closest Point Rotation Mute Inside")
		bool useClosestPointRotationMuteInside = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Use Yaw for Positional Rotation")
		bool useYaw = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Use Pitch for Positional Rotation")
		bool usePitch = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Use Roll for Positional Rotation")
		bool useRoll = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin|Volume", DisplayName = "Volume")
		float Volume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin|Volume", DisplayName = "Autoplay")
		bool autoplay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Attenuation BlendMode Curve")
		UCurveFloat* attenuationBlendModeCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "M1 Plugin", DisplayName = "Ignore Top Bottom Walls")
		bool ignoreTopBottom = true;
};
