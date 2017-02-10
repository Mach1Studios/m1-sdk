//
//  Mach1 SDK
//
#include "M1SoundPluginPrivatePCH.h" // Change to your project name!

#include "Runtime/Engine/Public/AudioDecompress.h"
#include "Runtime/Engine/Public/AudioDevice.h"
#include "Runtime/Engine/Classes/Sound/AudioSettings.h"

#include "Developer/TargetPlatform/Public/Interfaces/ITargetPlatformManagerModule.h"
#include "Developer/TargetPlatform/Public/Interfaces/ITargetPlatform.h"

#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h" 
#include "M1StSPActor.h"
 
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

	audioComponentMidLeft = CreateDefaultSubobject< UAudioComponent>(TEXT("audioComponentMidLeft"));
	audioComponentMidRight = CreateDefaultSubobject< UAudioComponent>(TEXT("audioComponentMidRight"));
	audioComponent = CreateDefaultSubobject< UAudioComponent>(TEXT("audioComponent"));

	audioComponentMidLeft->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	audioComponentMidRight->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	audioComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	audioComponentMidLeft->SetRelativeLocation(FVector(0, 0, 0));
	audioComponentMidRight->SetRelativeLocation(FVector(0, 0, 0));
	audioComponent->SetRelativeLocation(FVector(0, 0, 0));

}

// Called when the game starts or when spawned
void AM1StSPActor::BeginPlay()
{
	Super::BeginPlay();

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

				TArray<uint8> rawFileMidLeft;
				TArray<uint8> rawFileMidRight;
				rawFileMidLeft.InsertZeroed(0, size);
				rawFileMidRight.InsertZeroed(0, size);

				float l, r;
				float mid, slide;

				int16* rawWaveData = (int16*)rawFile.GetData();
				int16* bufL = (int16*)rawFileMidLeft.GetData();
				int16* bufR = (int16*)rawFileMidRight.GetData();

				for (int32 position = 0, cnt = size / sizeof(int16); position < cnt; position += 2)
				{
					l = rawWaveData[position + 0] / 32767.0f;
					r = rawWaveData[position + 1] / 32767.0f;

					mid = (l + r) / 2;
					slide = (l - r) / 2;

					bufL[position + 0] = (int16)(32767.0f * -mid);
					bufR[position + 1] = (int16)(32767.0f * -mid);
				}

				soundWaveMidLeft = MakeSoundWaveFromBuffer(PlatformFormat, rawFileMidLeft, QualityInfo);
				soundWaveMidRight = MakeSoundWaveFromBuffer(PlatformFormat, rawFileMidRight, QualityInfo);

			}
		}

	 
		audioComponentMidLeft->SetSound(soundWaveMidLeft);
		audioComponentMidRight->SetSound(soundWaveMidRight);
		audioComponent->SetSound(soundWave);

		soundWaveMidLeft->bVirtualizeWhenSilent = true;
		soundWaveMidRight->bVirtualizeWhenSilent = true;
		soundWave->bVirtualizeWhenSilent = true;

		/*
		soundWaveMidLeft->bLooping = true;
		soundWaveMidRight->bLooping = true;
		soundWave->bLooping = true;
		Play();
		*/

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
			float s = UKismetMathLibrary::MapRangeClamped(Spatialize, -1, 1, 0, 1);

			float panL = cos(s * (0.5 * PI));
			float panR = cos((1.0 - s) * (0.5 *  PI));

			float dist = FVector::Dist(GetActorLocation(), player->GetPawn()->GetActorLocation());
			float vol = Volume *  (attenuationCurve ? attenuationCurve->GetFloatValue(dist) : 1);

			// AdjustVolume
			audioComponentMidLeft->SetVolumeMultiplier(FMath::Max(MIN_SOUND_VOLUME, vol * panL));
			audioComponentMidRight->SetVolumeMultiplier(FMath::Max(MIN_SOUND_VOLUME, vol * panR));
			audioComponent->SetVolumeMultiplier(FMath::Max(MIN_SOUND_VOLUME, vol));

			if (Debug)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "panL: " + FString::SanitizeFloat(panL) + " , panR: " + FString::SanitizeFloat(panR));
			}
		}
	}

}

void AM1StSPActor::Play()
{
	if (soundWave != nullptr)
	{
		audioComponentMidLeft->FadeIn(fadeInDuration);
		audioComponentMidRight->FadeIn(fadeInDuration);
		audioComponent->FadeIn(fadeInDuration);
	}
}

void AM1StSPActor::Stop()
{
	if (soundWave != nullptr)
	{
		audioComponentMidLeft->FadeOut(fadeOutDuration, MIN_SOUND_VOLUME);
		audioComponentMidRight->FadeOut(fadeOutDuration, MIN_SOUND_VOLUME);
		audioComponent->FadeOut(fadeOutDuration, MIN_SOUND_VOLUME);
	}
}
