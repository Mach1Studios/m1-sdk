// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "M1SoundPluginPrivatePCH.h"

class FM1SoundPlugin : public IM1SoundPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FM1SoundPlugin, M1SoundPlugin )



void FM1SoundPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FM1SoundPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



