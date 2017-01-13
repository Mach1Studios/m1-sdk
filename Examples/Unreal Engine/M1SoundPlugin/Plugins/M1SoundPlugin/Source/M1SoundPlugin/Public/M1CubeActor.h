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

	// geometric utils
	static float ClosestPointOnBox(FVector point, FVector center, FVector axis0, FVector axis1, FVector axis2, FVector extents, FVector& closestPoint);
	
	static bool Clip(float denom, float numer, float& t0, float& t1);
	static int DoClipping(float t0, float t1, FVector origin, FVector direction, FVector center, FVector axis0, FVector axis1, FVector axis2, FVector extents, bool solid, FVector& point0, FVector& point1);

	USoundAttenuation* NullAttenuation;

	TArray<float> VolumeFactor;

	TArray<USoundBase*> SoundsWalls;
	TArray<UAudioComponent*> LeftChannelsWalls;
	TArray<UAudioComponent*> RightChannelsWalls;

	TArray<USoundBase*> SoundsCenter;
	TArray<UAudioComponent*> LeftChannelsCenter;
	TArray<UAudioComponent*> RightChannelsCenter;

	USceneComponent* Root;
	UBoxComponent* Collision;
	UBillboardComponent* Billboard;

	bool isInit;

	void Init();
	void SetSoundSet();
	void CalculateChannelVolumes(FRotator rotator, FQuat quat);
	void SetVolumeWalls(float Volume);
	void SetVolumeCenter(float Volume);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Attenuation RoomMode Curve")
		UCurveFloat* attenuationRoomModeCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Display Debug")
		bool Debug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Room Mode")
		bool useRoomMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Use Closest Point")
		bool useClosestPoint = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Ignore Top Bottom Walls")
		bool ignoreTopBottom = true;

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
		USoundBase* FrontUpLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontUpRightChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackUpLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackUpRightChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontDownLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontDownRightChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackDownLeftChannelWalls;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackDownRightChannelWalls;


	// Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "Sound RoomMode")
		USoundBase* FrontUpLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "Sound RoomMode")
		USoundBase* FrontUpRightChannelCenter;

	UPROPERTY(EditAnywhere, Category = "Sound RoomMode")
		USoundBase* BackUpLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "Sound RoomMode")
		USoundBase* BackUpRightChannelCenter;

	UPROPERTY(EditAnywhere, Category = "Sound RoomMode")
		USoundBase* FrontDownLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "Sound RoomMode")
		USoundBase* FrontDownRightChannelCenter;

	UPROPERTY(EditAnywhere, Category = "Sound RoomMode")
		USoundBase* BackDownLeftChannelCenter;

	UPROPERTY(EditAnywhere, Category = "Sound RoomMode")
		USoundBase* BackDownRightChannelCenter;
};
