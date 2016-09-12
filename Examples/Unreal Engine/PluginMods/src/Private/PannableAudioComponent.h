// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PannableAudioComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANNABLE2DAUDIO_API UPannableAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	bool bInitSuccess;
		
	float FirstInitAttemptTime;

	float MaxInitTime;

	UPROPERTY()
		AActor* OwningActor;

	UPROPERTY()
		UAudioComponent* LeftEarAudioComponent;

	UPROPERTY()
		UAudioComponent* RightEarAudioComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Panning")
		float LeftEarVolume;

	UPROPERTY(BlueprintReadOnly, Category = "Panning")
		float RightEarVolume;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* DefaultSound;

	UPROPERTY()
		USoundAttenuation* NullAttenuation;
	
	// Sets default values for this component's properties
	UPannableAudioComponent();

	virtual void Init() ;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	bool ShouldTryInitialisation();

	UFUNCTION(BlueprintCallable, Category = "sound")
		void SetSound(USoundBase* Sound);

	UFUNCTION(BlueprintCallable, Category = "sound")
		void Play(float StartTime);

	UFUNCTION(BlueprintCallable, Category = "sound")
		void Stop();

	UFUNCTION(BlueprintCallable, Category = "sound")
		void SetVolume(float NewLeftVolume, float NewRightVolume);

	UFUNCTION(BlueprintCallable, Category = "sound")
		void SetVolumeLeft(float Volume);

	UFUNCTION(BlueprintCallable, Category = "sound")
		void SetVolumeRight(float Volume);

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

};
