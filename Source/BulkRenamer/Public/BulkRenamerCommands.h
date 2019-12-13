// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "BulkRenamerStyle.h"

class FBulkRenamerCommands : public TCommands<FBulkRenamerCommands>
{
public:

	FBulkRenamerCommands()
		: TCommands<FBulkRenamerCommands>(TEXT("BulkRenamer"), NSLOCTEXT("Contexts", "BulkRenamer", "BulkRenamer Plugin"), NAME_None, FBulkRenamerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenBulkRenamerWindow;
};