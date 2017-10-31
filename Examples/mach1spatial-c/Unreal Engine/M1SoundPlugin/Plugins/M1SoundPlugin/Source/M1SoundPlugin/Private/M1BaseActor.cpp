//
//  Mach1 SDK
//
#include "M1SoundPluginPrivatePCH.h" // Change to your project name!

// 4.18
#include "HeadMountedDisplay.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// OLD UE VERSION (4.17)
//#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

#include "Kismet/KismetMathLibrary.h"

#include <sstream>
#include "M1BaseActor.h"

#define MIN_SOUND_VOLUME (KINDA_SMALL_NUMBER*2)

template<typename T>
std::string toDebugString(const T& value)
{
	std::ostringstream oss;
	oss.precision(2);
	oss << std::fixed << value;
	return oss.str();
}

template < >
std::string toDebugString<FVector>(const FVector& value)
{
	std::ostringstream oss;
	oss.precision(2);
	oss << std::fixed << "(" << value.X << ", " << value.Y << ", " << value.Z << ")";
	return oss.str();
}

float AM1BaseActor::ClosestPointOnBox(FVector point, FVector center, FVector axis0, FVector axis1, FVector axis2, FVector extents, FVector & closestPoint)
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


bool AM1BaseActor::Clip(float denom, float numer, float& t0, float& t1)
{
	if ((double)denom > 0.0)
	{
		if ((double)numer > (double)denom * (double)t1)
			return false;
		if ((double)numer > (double)denom * (double)t0)
			t0 = numer / denom;
		return true;
	}
	if ((double)denom >= 0.0)
		return (double)numer <= 0.0;
	if ((double)numer > (double)denom * (double)t0)
		return false;
	if ((double)numer > (double)denom * (double)t1)
		t1 = numer / denom;
	return true;
}

int AM1BaseActor::DoClipping(float t0, float t1, FVector origin, FVector direction, FVector center, FVector axis0, FVector axis1, FVector axis2, FVector extents, bool solid, FVector& point0, FVector& point1)
{
	FVector vector = origin - center;
	FVector vector2 = FVector(FVector::DotProduct(vector, axis0), FVector::DotProduct(vector, axis1), FVector::DotProduct(vector, axis2));
	FVector vector3 = FVector(FVector::DotProduct(direction, axis0), FVector::DotProduct(direction, axis1), FVector::DotProduct(direction, axis2));

	float num = t0;
	float num2 = t1;

	int quantity = 0;

	bool flag = Clip(vector3.X, -vector2.X - extents.X, t0, t1) && Clip(-vector3.X, vector2.X - extents.X, t0, t1) && Clip(vector3.Y, -vector2.Y - extents.Y, t0, t1) && Clip(-vector3.Y, vector2.Y - extents.Y, t0, t1) && Clip(vector3.Z, -vector2.Z - extents.Z, t0, t1) && Clip(-vector3.Z, vector2.Z - extents.Z, t0, t1);
	if (flag && (solid || t0 != num || t1 != num2))
	{
		if (t1 > t0)
		{
			quantity = 2;
			point0 = origin + t0 * direction;
			point1 = origin + t1 * direction;
		}
		else
		{

			quantity = 1;
			point0 = origin + t0 * direction;
			point1 = FVector::ZeroVector;
		}
	}
	else
	{
		quantity = 0;
		point0 = FVector::ZeroVector;
		point1 = FVector::ZeroVector;
	}

	return quantity;
}

// Sets default values
void AM1BaseActor::InitComponents(int MAX_SOUNDS_PER_CHANNEL)
{
	this->MAX_SOUNDS_PER_CHANNEL = MAX_SOUNDS_PER_CHANNEL;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Root->bAutoActivate = true;
	Root->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Collision->InitBoxExtent(FVector(100, 100, 100));
	Collision->bEditableWhenInherited = false;
	Collision->SetHiddenInGame(false);
	Collision->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	Billboard = CreateDefaultSubobject< UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetHiddenInGame(false);
	Billboard->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	LeftChannelsWalls.SetNum(MAX_SOUNDS_PER_CHANNEL);
	RightChannelsWalls.SetNum(MAX_SOUNDS_PER_CHANNEL);

	LeftChannelsCenter.SetNum(MAX_SOUNDS_PER_CHANNEL);
	RightChannelsCenter.SetNum(MAX_SOUNDS_PER_CHANNEL);

	Volume = 1;
	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++) VolumeFactor.Add(1);
}

void AM1BaseActor::Init()
{
	if (!isInit)
	{
		USceneComponent* cameraComponent = nullptr;

		if (APlayerController* player = GetWorld()->GetFirstPlayerController())//UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			TArray<UCameraComponent*> Cameras;
			if (APawn* playerPawn = player->GetPawn())
			{
				playerPawn->GetComponents<UCameraComponent>(Cameras);
				if (Cameras.Num() < 1)
				{
					APlayerCameraManager* GameCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
					cameraComponent = GameCameraManager->GetRootComponent();
				}
				else
				{
					cameraComponent = Cameras[0];
				}
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
				LeftChannelsWalls[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("SoundCubeWalls %d_L_%d"), GetUniqueID(), i)));
				RightChannelsWalls[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("SoundCubeWalls %d_R_%d"), GetUniqueID(), i)));

				LeftChannelsWalls[i]->RegisterComponent(); // only for runtime
				RightChannelsWalls[i]->RegisterComponent(); // only for runtime

				LeftChannelsWalls[i]->SetRelativeLocation(FVector(0, -1, 0));
				RightChannelsWalls[i]->SetRelativeLocation(FVector(0, 1, 0));

				LeftChannelsWalls[i]->AttenuationSettings = NullAttenuation;
				RightChannelsWalls[i]->AttenuationSettings = NullAttenuation;

				LeftChannelsWalls[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform); // AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);//   AttachTo(sceneComponent);
				RightChannelsWalls[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform);


			}

			if (useRoomMode)
			{
				for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
				{
					LeftChannelsCenter[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("SoundCubeCenter %d_L_%d"), GetUniqueID(), i)));
					RightChannelsCenter[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("SoundCubeCenter %d_R_%d"), GetUniqueID(), i)));

					LeftChannelsCenter[i]->RegisterComponent(); // only for runtime
					RightChannelsCenter[i]->RegisterComponent(); // only for runtime

					LeftChannelsCenter[i]->SetRelativeLocation(FVector(0, -1, 0));
					RightChannelsCenter[i]->SetRelativeLocation(FVector(0, 1, 0));

					LeftChannelsCenter[i]->AttenuationSettings = NullAttenuation;
					RightChannelsCenter[i]->AttenuationSettings = NullAttenuation;

					LeftChannelsCenter[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform); // AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);//   AttachTo(sceneComponent);
					RightChannelsCenter[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
				}
			}

			isInit = true;
		}
	}

}


void AM1BaseActor::SetSoundSet()
{
	if (isInit)
	{
		SoundsWalls.Empty();

		SetSoundsWalls();

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			SoundsWalls[i]->bVirtualizeWhenSilent = true;
		}

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsWalls[i]->SetSound(SoundsWalls[i]);
			RightChannelsWalls[i]->SetSound(SoundsWalls[i]);
		}

		if (useRoomMode)
		{
			SoundsCenter.Empty();

			SetSoundsCenter();

			for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
			{
				SoundsCenter[i]->bVirtualizeWhenSilent = true;
			}

			for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
			{
				LeftChannelsCenter[i]->SetSound(SoundsCenter[i]);
				RightChannelsCenter[i]->SetSound(SoundsCenter[i]);
			}
		}
	}

}


void AM1BaseActor::Play()
{
	if (isInit)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsWalls[i]->FadeIn(fadeInDuration);
			RightChannelsWalls[i]->FadeIn(fadeInDuration);
		}
	}

	if (isInit && useRoomMode)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsCenter[i]->FadeIn(fadeInDuration);
			RightChannelsCenter[i]->FadeIn(fadeInDuration);
		}
	}

	if (!isInit)
	{
		needPlayAfterInit = true;
	}
}

void AM1BaseActor::Stop()
{
	if (isInit)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsWalls[i]->FadeOut(fadeOutDuration, 0);
			RightChannelsWalls[i]->FadeOut(fadeOutDuration, 0);
		}
	}

	if (isInit && useRoomMode)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsCenter[i]->FadeOut(fadeOutDuration, 0);
			RightChannelsCenter[i]->FadeOut(fadeOutDuration, 0);
		}
	}
	needPlayAfterInit = false;
}

// Called when the game starts or when spawned
void AM1BaseActor::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		if (APlayerController* player = GetWorld()->GetFirstPlayerController())
		{
			Init();
			SetSoundSet();
			if(autoplay || needPlayAfterInit) Play();
		}
	}
}

// Called every frame
void AM1BaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine && Root->IsActive())
	{
		if (APlayerController* player = GetWorld()->GetFirstPlayerController())
		{
			if (APawn* playerPawn = player->GetPawn())
			{
				Collision->SetHiddenInGame(!Debug);
				Billboard->SetHiddenInGame(!Debug);

				FQuat PlayerRotation;
				FVector PlayerPosition;
				if (ForceHMDRotation && UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
				{
					FRotator rotator;
					UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(rotator, PlayerPosition);

					// invert angles
					//PlayerRotation = player->GetControlRotation().Quaternion() * FQuat::MakeFromEuler(FVector(-rotator.Quaternion().Euler().X, -rotator.Quaternion().Euler().Y, rotator.Quaternion().Euler().Z));// rotator.Quaternion() * player->GetControlRotation().Quaternion();
					PlayerRotation = playerPawn->GetActorRotation().Quaternion() * FQuat::MakeFromEuler(FVector(-rotator.Quaternion().Euler().X, -rotator.Quaternion().Euler().Y, rotator.Quaternion().Euler().Z));// rotator.Quaternion() * player->GetControlRotation().Quaternion();
					PlayerPosition = playerPawn->GetActorLocation() + PlayerPosition;
					//GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, TEXT(">> " + DeviceRotation.Euler().ToString()));
				}
				else
				{
					PlayerRotation = player->GetControlRotation().Quaternion();
					PlayerPosition = playerPawn->GetActorLocation();
				}
				// maybe use cameraComponent->bAbsoluteRotation & bAbsolutePosition for position?

				/*
				GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT(">> " + PlayerRotation.Euler().ToString()));
				GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, isInit ? TEXT("init ok") : TEXT("no init"));
				GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, GetWorld()->GetFirstPlayerController()->GetPawn()->GetName());
				*/

				FVector point = GetActorLocation();

				FVector scale = Collision->GetScaledBoxExtent(); // GetActorScale() / 2 *
				scale = FVector(scale.Y, scale.Z, scale.X);

				float vol = Volume;

				FVector outsideClosestPoint;
				FVector insidePoint0, insidePoint1;

				FVector cameraPosition = PlayerPosition;
				if (ignoreTopBottom)
				{
					cameraPosition.Z = GetActorLocation().Z;
				}

				bool isOutside = (ClosestPointOnBox(PlayerPosition, GetActorLocation(), GetActorRightVector(), GetActorUpVector(), GetActorForwardVector(), scale, outsideClosestPoint) > 0);



				if (useClosestPoint && isOutside)
				{
					point = outsideClosestPoint;

					float dist = FVector::Dist(point, PlayerPosition);
					SetVolumeWalls(vol * (attenuationCurve ? attenuationCurve->GetFloatValue(dist) : 1));

					if (Debug)
					{
						DrawDebugLine(
							GetWorld(),
							GetActorLocation(),
							point,
							FColor(255, 0, 0),
							false,
							-1,
							0,
							0
						);

						DrawDebugPoint(GetWorld(),
							point,
							10.0,
							FColor(255, 0, 0),
							false,
							-1,
							0
						);
					}
				}
				else if (useRoomMode && !isOutside)// && DoClipping(0, std::numeric_limits<float>::max(), cameraPosition, (cameraPosition - GetActorLocation()).GetSafeNormal(), GetActorLocation(), GetActorRightVector(), GetActorUpVector(), GetActorForwardVector(), scale, true, insidePoint0, insidePoint1) == 2)
				{
					FVector p0 = GetActorTransform().InverseTransformPosition(cameraPosition) / 100;

					FVector p1 = p0;
					if (FMath::Abs(p0.X) > FMath::Abs(p0.Y) && FMath::Abs(p0.X) > FMath::Abs(p0.Z))
					{
						p1.X = p0.X > 0 ? 1 : -1;
					}
					if (FMath::Abs(p0.Y) > FMath::Abs(p0.X) && FMath::Abs(p0.Y) > FMath::Abs(p0.Z))
					{
						p1.Y = p0.Y > 0 ? 1 : -1;
					}
					if (FMath::Abs(p0.Z) > FMath::Abs(p0.X) && FMath::Abs(p0.Z) > FMath::Abs(p0.Y))
					{
						p1.Z = p0.Z > 0 ? 1 : -1;
					}
					p1 = GetActorTransform().TransformPosition(p1 * 100);

					float dist = 1 - FMath::Max(FMath::Abs(p0.X), FMath::Max(FMath::Abs(p0.Y), FMath::Abs(p0.Z)));

					//float dist = 1.0f - (cameraPosition - GetActorLocation()).Size() / (insidePoint1 - GetActorLocation()).Size();
					SetVolumeWalls(vol * (attenuationRoomModeCurve ? attenuationRoomModeCurve->GetFloatValue(dist) : 1));
					SetVolumeCenter(vol * (1 - (attenuationRoomModeCurve ? attenuationRoomModeCurve->GetFloatValue(dist) : 1)));

					if (Debug)
					{
						DrawDebugLine(
							GetWorld(),
							cameraPosition,
							p1,
							FColor(255, 255, 0),
							false,
							-1,
							0,
							0
						);

						DrawDebugPoint(GetWorld(),
							p1,
							10.0,
							FColor(255, 255, 0),
							false,
							-1,
							0
						);
					}

					if (Debug)
					{
						std::string str = "vol:    " + toDebugString((attenuationRoomModeCurve ? attenuationRoomModeCurve->GetFloatValue(dist) : 1));
						GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue, str.c_str());
					}
				}
				else if (useRotator)
				{
					float dist = FVector::Dist(point, PlayerPosition);
					SetVolumeWalls(vol * (attenuationCurve ? attenuationCurve->GetFloatValue(dist) : 1));
				}
				else
				{
					vol = 0;
					SetVolumeWalls(vol);
					SetVolumeCenter(vol);
				}

				//FindLookAtRotation seems wrong angle
				// compate with unity

				// Compute rotation for sound
				FQuat quat = UKismetMathLibrary::FindLookAtRotation(PlayerPosition, point).Quaternion().Inverse() * GetActorRotation().Quaternion();
				quat = FQuat::MakeFromEuler(FVector(useRoll ? quat.Euler().X : 0, usePitch ? quat.Euler().Y : 0, useYaw ? quat.Euler().Z : 0));
				quat *= PlayerRotation;

				CalculateChannelVolumes(quat);
			}
		}
	}

}

#if WITH_EDITOR
void AM1BaseActor::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
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
#endif

void AM1BaseActor::CalculateChannelVolumes(FQuat quat)
{
	std::vector<float> result = SoundAlgorithm(quat.Euler().Y, (quat.Euler().Z < 0 ? 360 + quat.Euler().Z : quat.Euler().Z), quat.Euler().X);


	// test
//	FVector vec = UKismetMathLibrary::FindLookAtRotation(FVector(0,0,0), FVector(100,100,100)).Quaternion().Euler();
//	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, vec.ToString());

	//#if UE_BUILD_DEBUG
	if (Debug)
	{
		std::string str = "angles:    " + toDebugString(quat.Euler().Y) + " , " + toDebugString(quat.Euler().Z < 0 ? 360 + quat.Euler().Z : quat.Euler().Z) + " , " + toDebugString(quat.Euler().X);
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
	}

	//UE_LOG(LogTemp, Log, TEXT("Your message"));
	//#endif

	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
	{
		VolumeFactor[i] = result[i];
	}
}

void AM1BaseActor::SetVolumeWalls(float volume)
{
	if (isInit)
	{
		float vol = FMath::Max(MIN_SOUND_VOLUME, this->Volume * volume);
		float newVolume = 0;

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			newVolume = VolumeFactor[i * 2] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			LeftChannelsWalls[i]->SetVolumeMultiplier(newVolume);

			newVolume = VolumeFactor[i * 2 + 1] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			RightChannelsWalls[i]->SetVolumeMultiplier(newVolume);
		}
	}
}

void AM1BaseActor::SetVolumeCenter(float volume)
{
	if (isInit && useRoomMode)
	{
		float vol = FMath::Max(MIN_SOUND_VOLUME, this->Volume * volume);
		float newVolume = 0;

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			newVolume = VolumeFactor[i * 2] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			LeftChannelsCenter[i]->SetVolumeMultiplier(newVolume);

			newVolume = VolumeFactor[i * 2 + 1] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			RightChannelsCenter[i]->SetVolumeMultiplier(newVolume);
		}
	}
}
