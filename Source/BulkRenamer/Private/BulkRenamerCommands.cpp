// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BulkRenamerCommands.h"

#define LOCTEXT_NAMESPACE "FBulkRenamerModule"

void FBulkRenamerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Bulk rename", "Bring up BulkRenamer window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
