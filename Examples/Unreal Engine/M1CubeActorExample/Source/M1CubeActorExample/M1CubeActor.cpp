//
//  Mach1 SDK
//
#include "M1CubeActorExample.h" // Change to your project name!

#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "HeadMountedDisplay.h"

#include <sstream>
#include "M1CubeActor.h"
#include "M1DSPAlgorithmsUE.h"

#define MAX_SOUNDS_PER_CHANNEL 8
#define MIN_SOUND_VOLUME (KINDA_SMALL_NUMBER*2)

float AM1CubeActor::ClosestPointOnBox(FVector point, FVector center, FVector axis0, FVector axis1, FVector axis2, FVector extents, FVector & closestPoint)
{
	FVector vector = point - center;
	float num = 0.0f;

	float num0 = FVector::DotProduct(vector, axis0);
	if (num0 < -extents.X)
	{
		num += powf(num0 + extents.X, 2);
		num0 = -extents.X;
	}
	else if (num0 > extents.X)
	{
		num += powf(num0 - extents.X, 2);
		num0 = extents.X;
	}

	float num1 = FVector::DotProduct(vector, axis1);
	if (num1 < -extents.Y)
	{
		num += powf(num1 + extents.Y, 2);
		num1 = -extents.Y;
	}
	else if (num1 > extents.Y)
	{
		num += powf(num1 - extents.Y, 2);
		num1 = extents.Y;
	}

	float num2 = FVector::DotProduct(vector, axis2);
	if (num2 < -extents.Z)
	{
		num += powf(num2 + extents.Z, 2);
		num2 = -extents.Z;
	}
	else if (num2 > extents.Z)
	{
		num += powf(num2 - extents.Z, 2);
		num2 = extents.Z;
	}
	closestPoint = center + num0 * axis0 + num1 * axis1 + num2 * axis2;

	return sqrt(num);
}


// Sets default values
AM1CubeActor::AM1CubeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Root->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Collision->InitBoxExtent(FVector(100, 100, 100));
	Collision->bEditableWhenInherited = false;
	Collision->SetHiddenInGame(false);
	Collision->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	Billboard = CreateDefaultSubobject< UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetHiddenInGame(false);
	Billboard->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	LeftChannels.SetNum(MAX_SOUNDS_PER_CHANNEL);
	RightChannels.SetNum(MAX_SOUNDS_PER_CHANNEL);

	Volume = 1;
	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++) VolumeFactor.Add(1);
}

void AM1CubeActor::Init()
{
	if (!isInit)
	{
		USceneComponent* cameraComponent = nullptr;

		if (APlayerController* player = GetWorld()->GetFirstPlayerController())//UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			TArray<UCameraComponent*> Cameras;
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponents<UCameraComponent>(Cameras);

			if (Cameras.Num() == 0)
			{
				APlayerCameraManager* GameCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				cameraComponent = GameCameraManager->GetRootComponent();
			}
			else
			{
				cameraComponent = Cameras[0];
			}
			//GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, FString::FromInt(Cameras.Num()));

			/*
			AActor *vt = GetWorld()->GetFirstPlayerController()->GetViewTarget();
			ACameraActor* camera = Cast<ACameraActor>(vt);
			if (camera) {
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, camera->GetCameraComponent()->GetName());
			}
			*/
		}

		if (cameraComponent)
		{
			NullAttenuation = NewObject<USoundAttenuation>(this);
			NullAttenuation->Attenuation.bAttenuate = false;
			NullAttenuation->Attenuation.bSpatialize = true;

			/*
			if (GEngine->HMDDevice.IsValid() && !GEngine->HMDDevice->IsStereoEnabled())
			{
			FTransform transform;
			transform.SetIdentity();

			FQuat orientation;
			FVector position;
			GEngine->HMDDevice->GetCurrentOrientationAndPosition(orientation, position);
			transform.SetRotation(orientation);
			transform.SetTranslation(position);
			}
			*/

			for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
			{

				LeftChannels[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("Left Channel %d"), i))); 
				RightChannels[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("Right Channel %d"), i))); 

				LeftChannels[i]->RegisterComponent(); // only for runtime
				RightChannels[i]->RegisterComponent(); // only for runtime

				LeftChannels[i]->SetRelativeLocation(FVector(0, -1, 0));
				RightChannels[i]->SetRelativeLocation(FVector(0, 1, 0));

				LeftChannels[i]->AttenuationSettings = NullAttenuation;
				RightChannels[i]->AttenuationSettings = NullAttenuation;

				LeftChannels[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform); // AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);//   AttachTo(sceneComponent);
				RightChannels[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}

			isInit = true;
		}
	}

}


void AM1CubeActor::SetSoundSet()
{
	if (isInit)
	{
		Sounds.Empty();

		Sounds.Add(FrontUpLeftChannel);
		Sounds.Add(FrontUpRightChannel);
		Sounds.Add(BackUpLeftChannel);
		Sounds.Add(BackUpRightChannel);
		Sounds.Add(FrontDownLeftChannel);
		Sounds.Add(FrontDownRightChannel);
		Sounds.Add(BackDownLeftChannel);
		Sounds.Add(BackDownRightChannel);

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannels[i]->SetSound(Sounds[i]);
			RightChannels[i]->SetSound(Sounds[i]);
		}
	}

}

void AM1CubeActor::Play()
{
	if (isInit)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannels[i]->Play();
			RightChannels[i]->Play();
		}
	}
}

void AM1CubeActor::Stop()
{
	if (isInit)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannels[i]->Stop();
			RightChannels[i]->Stop();
		}
	}
}

// Called when the game starts or when spawned
void AM1CubeActor::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		if (APlayerController* player = GetWorld()->GetFirstPlayerController())
		{
			Init();
			SetSoundSet();
			Play();
		}
	}
}

// Called every frame
void AM1CubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		if (APlayerController* player = GetWorld()->GetFirstPlayerController())
		{

			/*
			bool bHMDIsReady = (GEngine && GEngine->HMDDevice.IsValid() && GEngine->HMDDevice->IsHMDConnected());
			if (bHMDIsReady)
			{
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, TEXT("HMD detected"));
			FQuat DeviceRotation;
			FVector  DevicePosition;
			GEngine->HMDDevice->GetCurrentOrientationAndPosition(DeviceRotation, DevicePosition);
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, TEXT(">> " + DeviceRotation.Euler().ToString()));
			}

			FRotator DeviceRotation;
			FVector  DevicePosition;
			UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, TEXT(">> " + DeviceRotation.Euler().ToString()));
			*/
 
			/*
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT(">> " + player->GetControlRotation().Euler().ToString()));
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, isInit ? TEXT("init ok") : TEXT("no init"));
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, GetWorld()->GetFirstPlayerController()->GetPawn()->GetName());
			*/

			FVector closestPoint = GetActorLocation();

			FVector scale = Collision->GetScaledBoxExtent(); // GetActorScale() / 2 * 
			scale = FVector(scale.Y, scale.Z, scale.X);

			if (ClosestPointOnBox(player->GetPawn()->GetActorLocation(), GetActorLocation(), GetActorRightVector(), GetActorUpVector(), GetActorForwardVector(), scale, closestPoint) > 0)
			{
				DrawDebugLine(
					GetWorld(),
					GetActorLocation(),
					closestPoint,
					FColor(255, 0, 0),
					false,
					-1,
					0,
					0
				);

				DrawDebugPoint(GetWorld(),
					closestPoint,
					10.0,
					FColor(255, 0, 0),
					false,
					-1,
					0
				);
			}

	
			//FindLookAtRotation seems wrong angle
			// compate with unity 

		 	// Compute rotation for sound
			FQuat quat = UKismetMathLibrary::FindLookAtRotation( player->GetPawn()->GetActorLocation(), closestPoint).Quaternion().Inverse() * GetActorRotation().Quaternion();
			quat = FQuat::MakeFromEuler(FVector(useRoll ? quat.Euler().X : 0, usePitch ? quat.Euler().Y : 0, useYaw ? quat.Euler().Z : 0));
			quat *= player->GetControlRotation().Quaternion();

			CalculateChannelVolumes(player->GetControlRotation(), quat);

			float dist = FVector::Dist(closestPoint, player->GetPawn()->GetActorLocation());
			SetVolume(Volume * (attenuationCurve ? attenuationCurve->GetFloatValue(dist) : 1));
		}
	}
}

void AM1CubeActor::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != NULL)
	{
		if (PropertyChangedEvent.Property->GetFName() == FName(TEXT("Debug")))
		{
			Collision->SetHiddenInGame(!Debug);
			Billboard->SetHiddenInGame(!Debug);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

template<typename T>
std::string toDebugString(const T& value)
{
	std::ostringstream oss;
	oss.precision(2);
	oss << std::fixed << value;
	return oss.str();
}

void AM1CubeActor::CalculateChannelVolumes(FRotator CameraRotation, FQuat quat)
{
	//quat.Euler().X
	std::vector<float> result = eightChannelsAlgorithm(quat.Euler().Y, quat.Euler().Z < 0 ? 360 + quat.Euler().Z : quat.Euler().Z, quat.Euler().X);

	
	// test
//	FVector vec = UKismetMathLibrary::FindLookAtRotation(FVector(0,0,0), FVector(100,100,100)).Quaternion().Euler();
//	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, vec.ToString());

	//#if UE_BUILD_DEBUG
	std::string str = "angles:    " + toDebugString(quat.Euler().Y) + " , " + toDebugString(quat.Euler().Z < 0 ? 360 + quat.Euler().Z : quat.Euler().Z) + " , " + toDebugString(quat.Euler().X);
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, str.c_str());

	str  ="angles Orig: " + toDebugString(CameraRotation.Pitch >= 270 ? CameraRotation.Pitch - 360 : CameraRotation.Pitch) + " , " + toDebugString(CameraRotation.Yaw) + " , " + toDebugString(CameraRotation.Roll > 270 ? CameraRotation.Roll - 360 : CameraRotation.Roll);
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, str.c_str());

	 
	std::string info;
	info = "left:  ";
	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
	{
		info += toDebugString(result[i * 2]) + ", ";
	}
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());

	info = "right: ";
	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
	{
		info += toDebugString(result[i * 2 + 1]) + ", ";
	}
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());

	//UE_LOG(LogTemp, Log, TEXT("Your message"));
	//#endif

	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
		VolumeFactor[i] = result[i];
}

void AM1CubeActor::SetVolume(float Volume)
{
	if (isInit)
	{
		float vol = FMath::Max(MIN_SOUND_VOLUME, this->Volume * Volume);
		float newVolume = 0;

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			newVolume = VolumeFactor[i * 2] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			LeftChannels[i]->SetVolumeMultiplier(newVolume);

			newVolume = VolumeFactor[i * 2 + 1] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			RightChannels[i]->SetVolumeMultiplier(newVolume);
		}
	}
}