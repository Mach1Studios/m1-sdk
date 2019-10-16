// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Mach1DecodePluginPrivatePCH.h"

class FMach1DecodePlugin : public IMach1DecodePlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FMach1DecodePlugin, Mach1DecodePlugin )



void FMach1DecodePlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FMach1DecodePlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



