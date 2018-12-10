//  Mach1 SDK
//  Copyright � 2017 Mach1. All rights reserved.
//

#include "M1StSPActor.h"

#include "Mach1DecodePluginPrivatePCH.h" // Change to your project name!

#include "Runtime/Engine/Public/AudioDecompress.h"
#include "Runtime/Engine/Public/AudioDeviceManager.h"
#include "Runtime/Engine/Public/AudioDevice.h"

#include "Runtime/Engine/Classes/Sound/AudioSettings.h"

#include "CoreMinimal.h"
#include "CoreMisc.h"
#include "Developer/TargetPlatform/Public/Interfaces/ITargetPlatform.h"
#include "Developer/TargetPlatform/Public/Interfaces/ITargetPlatformManagerModule.h"

#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h" 

#define MIN_SOUND_VOLUME (KINDA_SMALL_NUMBER*2)

USoundWave* AM1StSPActor::MakeSoundWaveFromBuffer(FName PlatformFormat, TArray<uint8>& rawFile, FSoundQualityInfo QualityInfo)
{
	const IAudioFormat* AudioFormat = GetTargetPlatformManager()->FindAudioFormat(PlatformFormat);
	TArray<uint8> rawFileNew;
	AudioFormat->Cook(PlatformFormat, rawFile, QualityInfo, rawFileNew);

	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());
	//sw->InvalidateCompressedData();

	FByteBulkData* bulkData = &sw->CompressedFormatData.GetFormat(PlatformFormat);
	bulkData->Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(bulkData->Realloc(rawFileNew.Num()), rawFileNew.GetData(), rawFileNew.Num());
	bulkData->Unlock();

	sw->Duration = QualityInfo.Duration;
	sw->SampleRate = QualityInfo.SampleRate;
	sw->NumChannels = QualityInfo.NumChannels;
	sw->RawPCMDataSize = QualityInfo.SampleDataSize;
	sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;

	return sw;
}

// Sets default values
AM1StSPActor::AM1StSPActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Root->bAutoActivate = true;
	Root->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	audioComponentMid = CreateDefaultSubobject< UAudioComponent>(TEXT("audioComponentMid"));
	audioComponent = CreateDefaultSubobject< UAudioComponent>(TEXT("audioComponent"));

	audioComponentMid->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	audioComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	audioComponentMid->SetRelativeLocation(FVector(0, 0, 0));
	audioComponent->SetRelativeLocation(FVector(0, 0, 0));
}

// Called when the game starts or when spawned
void AM1StSPActor::BeginPlay()
{
	Super::BeginPlay();

	FAudioDevice* AudioDevice = GEngine->GetActiveAudioDevice();
	//AudioDevice->CreateSoundSource()->



	if (soundWave != nullptr)
	{
		soundWave->InitAudioResource(GEngine->GetMainAudioDevice()->GetRuntimeFormat(soundWave));

		TArray<uint8> rawFile;

		ICompressedAudioInfo* AudioInfo = GEngine->GetMainAudioDevice()->CreateCompressedAudioInfo(soundWave);
		FSoundQualityInfo QualityInfo = { 0 };
		if (AudioInfo->ReadCompressedInfo(soundWave->ResourceData, soundWave->ResourceSize, &QualityInfo))
		{
			int32 size = QualityInfo.NumChannels * QualityInfo.Duration * QualityInfo.SampleRate * sizeof(int16);
			rawFile.InsertZeroed(0, size);
			AudioInfo->ExpandFile((uint8*)rawFile.GetData(), &QualityInfo);

			// Compress to whatever format the active target platform wants
			ITargetPlatformManagerModule* TPM = GetTargetPlatformManager();
			if (TPM)
			{
				// find platfrom format
				ITargetPlatform* CurrentPlatform = NULL;
				const TArray<ITargetPlatform*>& Platforms = TPM->GetActiveTargetPlatforms();
				CurrentPlatform = Platforms[0];
				for (int32 Index = 1; Index < Platforms.Num(); Index++)
				{
					if (Platforms[Index]->IsRunningPlatform())
					{
						CurrentPlatform = Platforms[Index];
						break;
					}
				}
				FName PlatformFormat = CurrentPlatform->GetWaveFormat(soundWave);

				// post processing audio

				TArray<uint8> rawFileMid;
				rawFileMid.InsertZeroed(0, size);

				float l, r;
				float mid, slide;

				int16* rawWaveData = (int16*)rawFile.GetData();
				int16* buf = (int16*)rawFileMid.GetData();

				for (int32 position = 0, cnt = size / sizeof(int16); position < cnt; position += 2)
				{
					l = rawWaveData[position + 0] / 32767.0f;
					r = rawWaveData[position + 1] / 32767.0f;

					mid = (l + r) / 2;
					slide = (l - r) / 2;

					buf[position + 0] = (int16)(32767.0f * -mid);
					buf[position + 1] = (int16)(32767.0f * -mid);
				}

				soundWaveMid = MakeSoundWaveFromBuffer(PlatformFormat, rawFileMid, QualityInfo);
			}
		}


  
		audioComponentMid->SetSound(soundWaveMid);
		audioComponent->SetSound(soundWave);

		audioComponentMid->AudioDeviceHandle = GEngine->GetActiveAudioDevice()->DeviceHandle;
		audioComponent->AudioDeviceHandle = GEngine->GetActiveAudioDevice()->DeviceHandle;

		soundWaveMid->bVirtualizeWhenSilent = true;
		soundWave->bVirtualizeWhenSilent = true;

		audioComponentMid->bAllowSpatialization = true;

		soundWaveMid->bLooping = true;
		soundWave->bLooping = true;

		Play();
		//audioComponentMidLeft->Play();// FadeIn(fadeInDuration);
		//audioComponentMidRight->Play();//FadeIn(fadeInDuration);
		//audioComponent->Play();//FadeIn(fadeInDuration);


		//soundWave->RemoveAudioResource();
	}
}

// Called every frame
void AM1StSPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine && Root->IsActive())
	{
		APlayerController* player = GetWorld()->GetFirstPlayerController();

		if (player != nullptr && soundWave != nullptr)
		{
			APawn* playerPawn = player->GetPawn();
			if (playerPawn != nullptr)
			{
				/*
				float s = UKismetMathLibrary::MapRangeClamped(Spatialize, -1, 1, 0, 1);

				float panL = cos(s * (0.5 * PI));
				float panR = cos((1.0 - s) * (0.5 *  PI));
				*/

				float dist = FVector::Dist(GetActorLocation(), playerPawn->GetActorLocation());
				float vol = Volume *  (attenuationCurve ? attenuationCurve->GetFloatValue(dist) : 1);

				// AdjustVolume
				audioComponentMid->SetVolumeMultiplier(FMath::Max(MIN_SOUND_VOLUME, vol));// *panL));
				audioComponent->SetVolumeMultiplier(FMath::Max(MIN_SOUND_VOLUME, vol));

				/*
				if (Debug)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "panL: " + FString::SanitizeFloat(panL) + " , panR: " + FString::SanitizeFloat(panR));
				}
				*/
			}
		}
	}

}

void AM1StSPActor::Play()
{
	if (soundWave != nullptr)
	{
		audioComponentMid->FadeIn(fadeInDuration);
		audioComponent->FadeIn(fadeInDuration);
	}
}

void AM1StSPActor::Stop()
{
	if (soundWave != nullptr)
	{
		audioComponentMid->FadeOut(fadeOutDuration, MIN_SOUND_VOLUME);
		audioComponent->FadeOut(fadeOutDuration, MIN_SOUND_VOLUME);
	}
}
