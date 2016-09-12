// Fill out your copyright notice in the Description page of Project Settings.


#include "Pannable2DAudioPrivatePCH.h"
#include "PannableAudioComponent.h"


// Sets default values for this component's properties
UPannableAudioComponent::UPannableAudioComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	MaxInitTime = 5.0f;
	RightEarVolume = LeftEarVolume = 1.0;
	// ...
}

// Called when the game starts
void UPannableAudioComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
		return;

	FirstInitAttemptTime = GetWorld()->TimeSeconds ;

		Init();
		
}

void UPannableAudioComponent::Init()
{

	OwningActor = GetOwner();

	if (OwningActor != nullptr)
	{
		APlayerCameraManager* APCM = nullptr;

		APCM = UGameplayStatics::GetPlayerCameraManager(OwningActor, 0);

		if (APCM != nullptr)
		{
			LeftEarAudioComponent = NewObject<UAudioComponent>(APCM);
			RightEarAudioComponent = NewObject<UAudioComponent>(APCM);

			LeftEarAudioComponent->RegisterComponent();
			RightEarAudioComponent->RegisterComponent();
			NullAttenuation = NewObject<USoundAttenuation>(this);
			NullAttenuation->Attenuation.bAttenuate = false;
			NullAttenuation->Attenuation.bSpatialize= true;

			LeftEarAudioComponent->AttenuationSettings = NullAttenuation;
			RightEarAudioComponent->AttenuationSettings = NullAttenuation;

			LeftEarAudioComponent->AttachToComponent(APCM->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			RightEarAudioComponent->AttachToComponent(APCM->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			LeftEarAudioComponent->SetRelativeLocation(FVector(0, -1, 0));
			RightEarAudioComponent->SetRelativeLocation(FVector(0, 1, 0));

			bInitSuccess = true;
			
			UE_LOG(LogTemp, Warning, TEXT("Init Success"));
			SetVolume(1.0f, 1.0f);
			SetSound(DefaultSound);

		}
	}

}


// Called every frame
void UPannableAudioComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
			
		if (ShouldTryInitialisation())
		{
				Init();

				if (!bInitSuccess && !ShouldTryInitialisation())
				UE_LOG(LogTemp, Warning, TEXT("Audio Component Initialisation failed after %f seconds of attempts"), (GetWorld()->TimeSeconds - FirstInitAttemptTime));
		}
	
}

void UPannableAudioComponent::SetVolume(float NewLeftVolume, float NewRightVolume)
{

	if (!bInitSuccess)
		return;

	NewLeftVolume = FMath::Clamp(NewLeftVolume, KINDA_SMALL_NUMBER*2, 1.0f);
	NewRightVolume = FMath::Clamp(NewRightVolume, KINDA_SMALL_NUMBER*2, 1.0f);

	LeftEarVolume = NewLeftVolume;
	RightEarVolume = NewRightVolume;

	LeftEarAudioComponent->SetVolumeMultiplier(LeftEarVolume);
	RightEarAudioComponent->SetVolumeMultiplier(RightEarVolume);
}

void UPannableAudioComponent::SetSound(USoundBase* Sound)
{
	if (!bInitSuccess)
		return;

	LeftEarAudioComponent->SetSound(Sound);
	RightEarAudioComponent->SetSound(Sound);
}

void UPannableAudioComponent::Play(float StartTime)
{
	if (!bInitSuccess)
		return;

	LeftEarAudioComponent->Play(StartTime);
	RightEarAudioComponent->Play(StartTime);

	
}

void UPannableAudioComponent::Stop()
{
	if (!bInitSuccess)
		return;

	LeftEarAudioComponent->Stop();
	RightEarAudioComponent->Stop();
}

void UPannableAudioComponent::SetVolumeLeft(float Volume)
{
	SetVolume(Volume, RightEarVolume);
}

void UPannableAudioComponent::SetVolumeRight(float Volume)
{
	SetVolume(LeftEarVolume, Volume);
}

bool UPannableAudioComponent::ShouldTryInitialisation()
{
	return !bInitSuccess && (GetWorld()->TimeSeconds - FirstInitAttemptTime) < MaxInitTime && GetNetMode()!= NM_DedicatedServer && NullAttenuation != nullptr;
}

void UPannableAudioComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{


	if(LeftEarAudioComponent!=nullptr)
	LeftEarAudioComponent->DestroyComponent();

	if (RightEarAudioComponent != nullptr)
	RightEarAudioComponent->DestroyComponent();

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}