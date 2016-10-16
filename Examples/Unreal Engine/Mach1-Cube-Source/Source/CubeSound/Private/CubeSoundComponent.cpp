// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeSound.h"
#include "CubeSoundComponent.h"


// Sets default values for this component's properties
UCubeSoundComponent::UCubeSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	Volume = 1;

	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL*2; i++)
		VolumeFactor.Add(1);

	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
	{
		LeftChannels.Add( CreateDefaultSubobject<UAudioComponent>(FName(*("Left Channel "+FString::FromInt(i)))) );
		RightChannels.Add( CreateDefaultSubobject<UAudioComponent>(FName(*("Right Channel " + FString::FromInt(i)))) );
	}
	// ...
}


// Called when the game starts
void UCubeSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
		return;

	FirstInitAttemptTime = GetWorld()->TimeSeconds;

	Init();
}

void UCubeSoundComponent::SetSoundSet(UEightChannelAudioSet* NewAudioSet)
{
	if (NewAudioSet == nullptr)
		return;

	Sounds.Empty();
	
	CurrentAudioSet = NewAudioSet;

	Sounds.Add(NewAudioSet->FrontUpLeftChannel);
	Sounds.Add(NewAudioSet->FrontUpRightChannel);
	Sounds.Add(NewAudioSet->BackUpLeftChannel);
	Sounds.Add(NewAudioSet->BackUpRightChannel);
	Sounds.Add(NewAudioSet->FrontDownLeftChannel);
	Sounds.Add(NewAudioSet->FrontDownRightChannel);
	Sounds.Add(NewAudioSet->BackDownLeftChannel);
	Sounds.Add(NewAudioSet->BackDownRightChannel);

	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
	{
		LeftChannels[i]->SetSound(Sounds[i]);
		RightChannels[i]->SetSound(Sounds[i]);
	}
}

// Called every frame
void UCubeSoundComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldTryInitialisation())
	{
		Init();

		if (!bInitSuccess && !ShouldTryInitialisation())
			UE_LOG(LogTemp, Warning, TEXT("Audio Component Initialisation failed after %f seconds of attempts"), (GetWorld()->TimeSeconds - FirstInitAttemptTime))
	}

	if (bInitSuccess)
		CalculateChannelVolumes();
}

bool UCubeSoundComponent::ShouldTryInitialisation()
{
	return !bInitSuccess && (GetWorld()->TimeSeconds - FirstInitAttemptTime) < MaxInitTime && GetNetMode() != NM_DedicatedServer && NullAttenuation != nullptr;
}

void UCubeSoundComponent::Init()
{
	
	OwningActor = GetOwner();

	if (OwningActor != nullptr)
		APCM = UGameplayStatics::GetPlayerCameraManager(OwningActor, 0);
	else
		return;

	if (APCM != nullptr)
	{
		/*
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL / 2; i++)
		{
			VirtualPositions.Add(FRotator(45, -45 + (i * 90), 0).Vector());	
			UE_LOG(LogTemp, Warning, TEXT("Virtual position %s %i"), *VirtualPositions[i].ToString(),i);

		}

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL / 2; i++)
			VirtualPositions.Add(FRotator(-45, -45 + (i * 90), 0).Vector());
		*/
		NullAttenuation = NewObject<USoundAttenuation>(this);
		NullAttenuation->Attenuation.bAttenuate = false;
		NullAttenuation->Attenuation.bSpatialize = true;

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannels[i]->AttenuationSettings = NullAttenuation;
			RightChannels[i]->AttenuationSettings = NullAttenuation;

			LeftChannels[i]->AttachToComponent(APCM->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			RightChannels[i]->AttachToComponent(APCM->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			LeftChannels[i]->SetRelativeLocation(FVector(0, -1, 0));
			RightChannels[i]->SetRelativeLocation(FVector(0, 1, 0));
		}

		
		SetSoundSet(NewObject<UEightChannelAudioSet>(OwningActor, AudioSet));

		bInitSuccess = true;
	}
}

void UCubeSoundComponent::Play()
{
	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
	{
		LeftChannels[i]->Play();
		RightChannels[i]->Play();
	}
}

void UCubeSoundComponent::Stop()
{
	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
	{
		LeftChannels[i]->Stop();
		RightChannels[i]->Stop();
	}
}

void UCubeSoundComponent::SetVolume(float NewVolume)
{

	Volume = FMath::Max(MIN_SOUND_VOLUME, NewVolume);


	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
	{
		float NetVolume = VolumeFactor[i] * Volume;

		NetVolume = FMath::Max(MIN_SOUND_VOLUME, NetVolume);

		LeftChannels[i]->SetVolumeMultiplier(NetVolume);
		
		NetVolume = VolumeFactor[i+8] * Volume;

		NetVolume = FMath::Max(MIN_SOUND_VOLUME, NetVolume);

		RightChannels[i]->SetVolumeMultiplier(NetVolume);
	}
}

void UCubeSoundComponent::CalculateChannelVolumes()
{
	if (APCM == nullptr)
		return;
	FRotator CameraRotation = APCM->GetCameraRotation();

	std::vector<float> result = eightChannelsAlgorithm(CameraRotation.Yaw, CameraRotation.Pitch, CameraRotation.Roll);

	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL*2; i++)
		VolumeFactor[i] = result[i];

	SetVolume(Volume);
}