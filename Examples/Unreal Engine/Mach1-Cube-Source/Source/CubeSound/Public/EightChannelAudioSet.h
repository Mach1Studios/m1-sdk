// Fill out your copyright notice in the Description page of Project Settings.

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
