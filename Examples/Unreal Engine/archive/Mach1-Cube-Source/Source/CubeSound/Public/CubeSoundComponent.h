//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  CubeComponent
//

#pragma once

#include "Components/ActorComponent.h"
#include "EightChannelAudioSet.h"
#include "M1DSPAlgorithmsUE.h"
#include "CubeSoundComponent.generated.h"

#define MAX_SOUNDS_PER_CHANNEL 8
#define MIN_SOUND_VOLUME (KINDA_SMALL_NUMBER*2)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUBESOUND_API  UCubeSoundComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	bool bInitSuccess;

	float FirstInitAttemptTime;

	float MaxInitTime;

public:	

	UPROPERTY()
		APlayerCameraManager* APCM;

	UPROPERTY()
	TArray<UAudioComponent*> LeftChannels;

	UPROPERTY()
	TArray<UAudioComponent*> RightChannels;

	UPROPERTY()
		USoundAttenuation* NullAttenuation;

	UPROPERTY()
		AActor* OwningActor;

	/**
	Order of sounds is 
	0 - front up left
	1 - front up right
	2 - back up left
	3 - back up right
	4 - front down left
	5 - front down right
	6 - back down left
	7 - back down right
	*/
	UPROPERTY(BlueprintReadWrite,VisibleInstanceOnly,Category="Sound")
		TArray <USoundBase*>Sounds;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Sound")
		TArray <float>VolumeFactor;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		TSubclassOf<UEightChannelAudioSet> AudioSet;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Sound")
		UEightChannelAudioSet* CurrentAudioSet;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Sound")
		TArray <FVector>VirtualPositions;

	UPROPERTY(BlueprintReadOnly, Category = "Sound")
		float Volume;

	// Sets default values for this component's properties
	UCubeSoundComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable,Category = "Audio")
	void Play();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void Stop();

	void Init();

	void SetVolume(float Volume);

	bool ShouldTryInitialisation();

	void CalculateChannelVolumes();

	void SetSoundSet(UEightChannelAudioSet* AudioSet);

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;



};
