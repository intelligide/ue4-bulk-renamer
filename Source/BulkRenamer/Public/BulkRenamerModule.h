// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "BulkRenamingRule.h"

class FToolBarBuilder;
class FMenuBuilder;

class FBulkRenamerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	TSharedRef<class FExtender> OnContentBrowserContextMenu(const TArray<struct FAssetData>& Assets);
	void AddContentBrowserMenuExtension(FMenuBuilder& MenuBuilder);
	void PluginButtonClicked();
	
private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TArray<struct FAssetData> SelectedAssets;
};
