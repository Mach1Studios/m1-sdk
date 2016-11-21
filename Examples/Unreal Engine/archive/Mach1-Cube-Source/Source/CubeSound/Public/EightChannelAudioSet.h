//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  CubeComponent
//

#pragma once

#include "Object.h"
#include "EightChannelAudioSet.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CUBESOUND_API UEightChannelAudioSet : public UObject
{
	GENERATED_BODY()

public:


//Audio positions for the 8channel cube
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontUpLeftChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontUpRightChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackUpLeftChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackUpRightChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontDownLeftChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* FrontDownRightChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackDownLeftChannel;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* BackDownRightChannel;

	
	
};
