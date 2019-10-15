//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//

#include "M1BaseActor.h"

#include "Runtime/Launch/Resources/Version.h"

#include "Mach1DecodePluginPrivatePCH.h"

#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 18
#include "HeadMountedDisplay.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#elif ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 17
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#endif

#include <sstream>

#ifdef WIN32
void OutputDebugStringA(const char* lpOutputString) 
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), lpOutputString);
}
#endif

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

#ifdef LEGACY_POSITIONAL

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

#endif

FVector AM1BaseActor::GetEuler(FQuat q1)
{
	float sq = q1.X * q1.Y + q1.Z * q1.W;
	float sqx = q1.X * q1.X;
	float sqy = q1.Y * q1.Y;
	float sqz = q1.Z * q1.Z;

	return FMath::RadiansToDegrees(FVector(
		atan2(2.0f * q1.X * q1.W - 2 * q1.Y * q1.Z, 1.0f - 2.0f * sqx - 2.0f * sqz),
		atan2(2.0f * q1.Y * q1.W - 2 * q1.X * q1.Z, 1.0f - 2.0f * sqy - 2.0f * sqz),
		sin(2.0f * sq)
	));

}

Mach1Point3D AM1BaseActor::ConvertToMach1Point3D(FVector vec)
{
	return Mach1Point3D{ vec.X, vec.Y, vec.Z };
}

Mach1Point4D AM1BaseActor::ConvertToMach1Point4D(FQuat quat)
{
	return Mach1Point4D{ quat.X, quat.Y, quat.Z, quat.W };
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

	LeftChannelsMain.SetNum(MAX_SOUNDS_PER_CHANNEL);
	RightChannelsMain.SetNum(MAX_SOUNDS_PER_CHANNEL);

	LeftChannelsBlend.SetNum(MAX_SOUNDS_PER_CHANNEL);
	RightChannelsBlend.SetNum(MAX_SOUNDS_PER_CHANNEL);

	Volume = 1;
	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++) VolumeFactor.Add(1);
	 
#ifdef LEGACY_POSITIONAL
	mach1Decode.setPlatformType(Mach1PlatformType::Mach1PlatformUE);
#else 
	m1Positional.setPlatformType(Mach1PlatformType::Mach1PlatformUnity);
#endif
}

void AM1BaseActor::Init()
{
	if (!isInited)
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
		}

		if (cameraComponent)
		{
			NullAttenuation = NewObject<USoundAttenuation>(this);
			NullAttenuation->Attenuation.bAttenuate = false;
			NullAttenuation->Attenuation.bSpatialize = true;

			for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
			{
				LeftChannelsMain[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("SoundCubeWalls %d_L_%d"), GetUniqueID(), i)));
				RightChannelsMain[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("SoundCubeWalls %d_R_%d"), GetUniqueID(), i)));

				LeftChannelsMain[i]->RegisterComponent(); // only for runtime
				RightChannelsMain[i]->RegisterComponent(); // only for runtime

				LeftChannelsMain[i]->SetRelativeLocation(FVector(0, -1, 0));
				RightChannelsMain[i]->SetRelativeLocation(FVector(0, 1, 0));

				LeftChannelsMain[i]->AttenuationSettings = NullAttenuation;
				RightChannelsMain[i]->AttenuationSettings = NullAttenuation;

				LeftChannelsMain[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform); // AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);//   AttachTo(sceneComponent);
				RightChannelsMain[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}

			if (useBlendMode)
			{
				for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
				{
					LeftChannelsBlend[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("SoundCubeCenter %d_L_%d"), GetUniqueID(), i)));
					RightChannelsBlend[i] = NewObject <UAudioComponent>(cameraComponent, FName(*FString::Printf(TEXT("SoundCubeCenter %d_R_%d"), GetUniqueID(), i)));

					LeftChannelsBlend[i]->RegisterComponent(); // only for runtime
					RightChannelsBlend[i]->RegisterComponent(); // only for runtime

					LeftChannelsBlend[i]->SetRelativeLocation(FVector(0, -1, 0));
					RightChannelsBlend[i]->SetRelativeLocation(FVector(0, 1, 0));

					LeftChannelsBlend[i]->AttenuationSettings = NullAttenuation;
					RightChannelsBlend[i]->AttenuationSettings = NullAttenuation;

					LeftChannelsBlend[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform); // AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);//   AttachTo(sceneComponent);
					RightChannelsBlend[i]->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
				}
			}

			isInited = true;
		}
	}

}

void AM1BaseActor::SetSoundSet()
{
	if (isInited)
	{
		SoundsMain.Empty();

		SetSoundsMain();

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			//if(SoundsMain[i]) SoundsMain[i]->bVirtualizeWhenSilent = true;
		}

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			if (SoundsMain[i])
			{
				LeftChannelsMain[i]->SetSound(SoundsMain[i]);
				RightChannelsMain[i]->SetSound(SoundsMain[i]);
			}
		}

		if (useBlendMode)
		{
			SoundsBlendMode.Empty();

			SetSoundsBlendMode();

			for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
			{
				//if (SoundsBlendMode[i]) SoundsBlendMode[i]->bVirtualizeWhenSilent = true;
			}

			for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
			{
				if (SoundsBlendMode[i])
				{
					LeftChannelsBlend[i]->SetSound(SoundsBlendMode[i]);
					RightChannelsBlend[i]->SetSound(SoundsBlendMode[i]);
				}
			}
		}
	}

}


void AM1BaseActor::Play()
{
	if (isInited)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsMain[i]->FadeIn(fadeInDuration);
			RightChannelsMain[i]->FadeIn(fadeInDuration);
		}
	}

	if (isInited && useBlendMode)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsBlend[i]->FadeIn(fadeInDuration);
			RightChannelsBlend[i]->FadeIn(fadeInDuration);
		}
	}

	if (!isInited)
	{
		needToPlayAfterInit = true;
	}
}

void AM1BaseActor::Stop()
{
	if (isInited)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsMain[i]->FadeOut(fadeOutDuration, 0);
			RightChannelsMain[i]->FadeOut(fadeOutDuration, 0);
		}
	}

	if (isInited && useBlendMode)
	{
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			LeftChannelsBlend[i]->FadeOut(fadeOutDuration, 0);
			RightChannelsBlend[i]->FadeOut(fadeOutDuration, 0);
		}
	}
	needToPlayAfterInit = false;
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
			if(autoplay || needToPlayAfterInit) Play();
		}
	}
}

void PrintDebug(const char* str) {
#ifdef WIN32
	OutputDebugStringA(str);
	OutputDebugStringA("\r\n");
#endif
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

				FQuat PlayerRotation = FQuat::Identity;
				FVector PlayerPosition = FVector(0,0,0);

				 
				if (manualPawn != nullptr)
				{
					if (useReferenceObjectRotation) PlayerRotation = manualPawn->GetControlRotation().Quaternion();
					if (useReferenceObjectPosition) PlayerPosition = manualPawn->GetActorLocation();
				}
				else if (manualActor != nullptr)
				{
					if (useReferenceObjectRotation) PlayerRotation = manualActor->GetActorRotation().Quaternion();
					if (useReferenceObjectPosition) PlayerPosition = manualActor->GetActorLocation();
				}
				else if (manualCameraActor != nullptr)
				{
					if (useReferenceObjectRotation) PlayerRotation = manualCameraActor->GetActorRotation().Quaternion();
					if (useReferenceObjectPosition) PlayerPosition = manualCameraActor->GetActorLocation();
				}

				if (ForceHMDRotation && UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
				{
					FRotator hmdRotator;
					FVector hmdPosition;
					UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(hmdRotator, hmdPosition);

					FQuat hmdQuat = FQuat::MakeFromEuler(FVector(-hmdRotator.Quaternion().Euler().X, -hmdRotator.Quaternion().Euler().Y, hmdRotator.Quaternion().Euler().Z));

					PlayerRotation = PlayerRotation * hmdQuat;// rotator.Quaternion() * player->GetControlRotation().Quaternion();
					PlayerPosition = PlayerPosition + hmdPosition;
				}
				else
				{
				//	PlayerRotation = player->GetControlRotation().Quaternion();
				//	PlayerPosition = playerPawn->GetActorLocation();
				}




				PlayerRotation = PlayerRotation * FQuat::MakeFromEuler(cameraManualAngleOffset);

				if (Debug) {
					std::string info;

					info = "Camera Rotation:  " + toDebugString(PlayerRotation.Euler());
					PrintDebug(info.c_str());
					GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());

					info = "Camera Position:  " + toDebugString(PlayerPosition);
					PrintDebug(info.c_str());
					GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());

					info = "Actor Rotation:  " + toDebugString(GetActorRotation().Quaternion().Euler());
					PrintDebug(info.c_str());
					GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());

					info = "Actor Position:  " + toDebugString(GetActorLocation());
					PrintDebug(info.c_str());
					GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());


				}

				FVector point = GetActorLocation();

				FVector scale = Collision->GetScaledBoxExtent(); // GetActorScale() / 2 *
				//scale = FVector(scale.Y, scale.Z, scale.X);

				float vol = Volume;

#ifdef LEGACY_POSITIONAL

				FVector outsideClosestPoint;

				FVector cameraPosition = PlayerPosition;
				if (ignoreTopBottom && useBlendMode)
				{
					cameraPosition.Z = GetActorLocation().Z;
				}

				FQuat actorRotation = GetActorRotation().Quaternion();

				//bool isOutside = (ClosestPointOnBox(cameraPosition, GetActorLocation(), GetActorRightVector(), GetActorUpVector(), GetActorForwardVector(), scale, outsideClosestPoint) > 0);
				bool isOutside = (ClosestPointOnBox(cameraPosition, GetActorLocation(), actorRotation * FVector(1,0,0), actorRotation * FVector(0, 1, 0), actorRotation * FVector(0, 0, 1), scale / 2.0f, outsideClosestPoint) > 0);
				bool hasSoundOutside = isOutside && !muteWhenOutsideObject;
				bool hasSoundInside = !isOutside && !muteWhenInsideObject;
				 
				if (hasSoundOutside && useClosestPointRotationMuteInside) // useClosestPointRotation
				{
					point = outsideClosestPoint;

					float dist = FVector::Dist(point, cameraPosition);
					SetVolumeMain(vol * (attenuationCurve ? attenuationCurve->GetFloatValue(dist) : 1));
					
					
					if (Debug)
					{
						std::string info; 
						info = "dist_:  " + toDebugString(dist) + " , ignoreTopBottom: " + toDebugString(ignoreTopBottom);
						GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, info.c_str());

						
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
				else if (hasSoundInside && useBlendMode)// && DoClipping(0, std::numeric_limits<float>::max(), cameraPosition, (cameraPosition - GetActorLocation()).GetSafeNormal(), GetActorLocation(), GetActorRightVector(), GetActorUpVector(), GetActorForwardVector(), scale, true, insidePoint0, insidePoint1) == 2)
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
					SetVolumeMain(vol * (attenuationBlendModeCurve ? attenuationBlendModeCurve->GetFloatValue(dist) : 1));
					SetVolumeBlend(vol * (1 - (attenuationBlendModeCurve ? attenuationBlendModeCurve->GetFloatValue(dist) : 1)));

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
						std::string str = "vol:    " + toDebugString((attenuationBlendModeCurve ? attenuationBlendModeCurve->GetFloatValue(dist) : 1));
						GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue, str.c_str());
					}
				}
				else if (hasSoundOutside || hasSoundInside)
				{
					float dist = FVector::Dist(point, cameraPosition);
                
					if (Debug)
					{
						std::string info;
						info = "dist old:  " + toDebugString(dist) + " , ignoreTopBottom: " + toDebugString(ignoreTopBottom);
						GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, info.c_str());
						info = "curve:  " + toDebugString((attenuationCurve ? attenuationCurve->GetFloatValue(dist) : 1));
						GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, info.c_str());
					}
					
					if (hasSoundOutside)
                    {
						SetVolumeMain(vol * (attenuationCurve ? attenuationCurve->GetFloatValue(dist) : 1));
                    }
                    if (useBlendMode)
                    {
						SetVolumeMain(vol * (attenuationBlendModeCurve ? attenuationBlendModeCurve->GetFloatValue(dist) : 1));
                    }
				}
				else
				{
					vol = 0;
					SetVolumeMain(vol);
					SetVolumeBlend(vol);
				}

				FQuat quat = FLookAtMatrix(point, cameraPosition, FVector::UpVector).ToQuat();
				quat = quat.Inverse();
				quat = quat * actorRotation;

				//FQuat quat = FLookAtMatrix(point, cameraPosition, FVector::UpVector).Inverse().ToQuat() * actorRotation;
				quat = FQuat::MakeFromEuler(FVector(useRollForRotation ? quat.Euler().X : 0, usePitchForRotation ? quat.Euler().Y : 0, useYawForRotation ? quat.Euler().Z : 0));
				quat *= PlayerRotation;

				CalculateChannelVolumes(quat);

				DrawDebugLine(
					GetWorld(),
					GetActorLocation(),
					quat * (FVector::ForwardVector * 100),
					FColor(255, 255, 0),
					false,
					-1,
					0,
					20
				);
				DrawDebugLine(
					GetWorld(),
					GetActorLocation(),
					quat * (FVector::RightVector * 50),
					FColor(255, 255, 0),
					false,
					-1,
					0,
					20
				);

#else

				m1Positional.setUseBlendMode(useBlendMode);
				m1Positional.setIgnoreTopBottom(ignoreTopBottom);
				m1Positional.setMuteWhenOutsideObject(muteWhenOutsideObject);
				m1Positional.setMuteWhenInsideObject(muteWhenInsideObject);
				m1Positional.setUseAttenuation(useFalloff);
				m1Positional.setUsePlaneCalculation(useClosestPointRotationMuteInside);
				m1Positional.setUseYawForRotation(useYawForRotation);
				m1Positional.setUsePitchForRotation(usePitchForRotation);
				m1Positional.setUseRollForRotation(useRollForRotation);
				 
				m1Positional.setListenerPosition(ConvertToMach1Point3D(FVector(PlayerPosition.Y, PlayerPosition.Z, PlayerPosition.X))); //ConvertToMach1Point3D(PlayerPosition));
				FVector listenerAngle = (PlayerRotation.Euler());
				m1Positional.setListenerRotation(ConvertToMach1Point3D(FVector(listenerAngle.Y, listenerAngle.Z, -listenerAngle.X)));
				m1Positional.setDecoderAlgoPosition(ConvertToMach1Point3D(FVector(GetActorLocation().Y, GetActorLocation().Z, GetActorLocation().X))); //ConvertToMach1Point3D(GetActorLocation()));
				FVector decoderAngle = (GetActorRotation().Euler());
				m1Positional.setDecoderAlgoRotation(ConvertToMach1Point3D(FVector(decoderAngle.Y, decoderAngle.Z, -decoderAngle.X))); //ConvertToMach1Point3D(GetEuler(GetActorRotation().Quaternion())));
				m1Positional.setDecoderAlgoScale(ConvertToMach1Point3D(scale));

				m1Positional.evaluatePositionResults();

				if (useFalloff)
				{
					m1Positional.setAttenuationCurve(attenuationCurve ? attenuationCurve->GetFloatValue(m1Positional.getDist()) : 1);
					m1Positional.setAttenuationCurveBlendMode(attenuationBlendModeCurve ? attenuationBlendModeCurve->GetFloatValue(m1Positional.getDist()) : 1);
				}

				float volumesWalls[18];
				m1Positional.getCoefficients(volumesWalls);
				for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
				{
					VolumeFactor[i] = volumesWalls[i];
				}
				SetVolumeMain(1.0);

				float volumesRoom[18];
				m1Positional.getCoefficientsInterior(volumesRoom);
				if (useBlendMode)
				{
					for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
					{
						VolumeFactor[i] = volumesRoom[i];
					}
					SetVolumeBlend(1.0);
				}
				 
				if (Debug)
				{
					std::string info;
					info = "Lib Distance:  " + toDebugString(m1Positional.getDist());
					GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, info.c_str());

					std::string str = "Lib Euler Angles:    " + toDebugString(m1Positional.getCurrentAngle().x) + " , " + toDebugString(m1Positional.getCurrentAngle().y) + " , " + toDebugString(m1Positional.getCurrentAngle().z);
					GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, str.c_str());


					info = "volumes:  ";
					for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
					{
						info += toDebugString(volumesWalls[i]) + ", ";
					}
					GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());
				}
#endif
				 
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

#ifdef LEGACY_POSITIONAL
void AM1BaseActor::CalculateChannelVolumes(FQuat quat)
{
	static float volumes[18];

	FVector angles = GetEuler(quat);

	std::string info = "euler    :  ";
	for (int i = 0; i < 3; i++)
	{
		info += toDebugString(angles[i]) + " , ";
	}
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());

	//SoundAlgorithm(angles.Z, angles.Y, angles.X, volumes);
	SoundAlgorithm(angles.X, angles.Y, angles.Z, volumes);
	

	//#if UE_BUILD_DEBUG
	if (Debug)
	{
		std::string str = "angles m1:    " + toDebugString(angles.X) + " , " + toDebugString(angles.Y) + " , " + toDebugString(angles.Z);
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, str.c_str());

		std::string info;
		info = "left    :  ";
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			info += toDebugString(volumes[i * 2]) + ", ";
		}
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());

		info = "right    : ";
		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			info += toDebugString(volumes[i * 2 + 1]) + ", ";
		}
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, info.c_str());
	}
	 
	//UE_LOG(LogTemp, Log, TEXT("Your message"));
	//#endif

	for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL * 2; i++)
	{
		VolumeFactor[i] = volumes[i];
	}
}
#endif

void AM1BaseActor::SetVolumeMain(float volume)
{
	if (isInited)
	{
		float vol = FMath::Max(MIN_SOUND_VOLUME, this->Volume * volume);
		float newVolume = 0;

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			newVolume = VolumeFactor[i * 2] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			LeftChannelsMain[i]->SetVolumeMultiplier(newVolume);

			newVolume = VolumeFactor[i * 2 + 1] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			RightChannelsMain[i]->SetVolumeMultiplier(newVolume);
		}
	}
}

void AM1BaseActor::SetVolumeBlend(float volume)
{
	if (isInited && useBlendMode)
	{
		float vol = FMath::Max(MIN_SOUND_VOLUME, this->Volume * volume);
		float newVolume = 0;

		for (int i = 0; i < MAX_SOUNDS_PER_CHANNEL; i++)
		{
			newVolume = VolumeFactor[i * 2] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			LeftChannelsBlend[i]->SetVolumeMultiplier(newVolume);

			newVolume = VolumeFactor[i * 2 + 1] * vol;
			newVolume = FMath::Max(MIN_SOUND_VOLUME, newVolume);
			RightChannelsBlend[i]->SetVolumeMultiplier(newVolume);
		}
	}
}

 void AM1BaseActor::SetSoundsMain() 
 {
 }

 void AM1BaseActor::SetSoundsBlendMode() 
 {
 }

#ifdef LEGACY_POSITIONAL
 void AM1BaseActor::SoundAlgorithm(float Yaw, float Pitch, float Roll, float * volumes)
 {
	 mach1Decode.decode(Yaw, Pitch, Roll, volumes);
	 mach1Decode.beginBuffer();
 }
#endif
