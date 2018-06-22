// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Mach1PluginPrivatePCH.h"

class FMach1Plugin : public IMach1Plugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FMach1Plugin, Mach1Plugin )



void FMach1Plugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FMach1Plugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



