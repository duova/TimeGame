// Copyright (C) Varian Daemon 2023. All Rights Reserved.

#include "InventoryFrameworkPlugin.h"

#define LOCTEXT_NAMESPACE "FInventoryFrameworkPluginModule"

void FInventoryFrameworkPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FInventoryFrameworkPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventoryFrameworkPluginModule, InventoryFrameworkPlugin)