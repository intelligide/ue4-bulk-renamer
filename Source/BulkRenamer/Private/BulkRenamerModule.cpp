// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BulkRenamerModule.h"
#include "BulkRenamerStyle.h"
#include "BulkRenamerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ContentBrowserModule.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Views/SListView.h"
#include "BulkRenamerEditorToolkit.h"

#define LOCTEXT_NAMESPACE "FBulkRenamerModule"

void FBulkRenamerModule::StartupModule()
{
	FBulkRenamerStyle::Initialize();
	FBulkRenamerStyle::ReloadTextures();

	FBulkRenamerCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FBulkRenamerCommands::Get().OpenBulkRenamerWindow,
		FExecuteAction::CreateRaw(this, &FBulkRenamerModule::PluginButtonClicked),
		FCanExecuteAction());

	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		TArray<FContentBrowserMenuExtender_SelectedAssets>& MenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
		MenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FBulkRenamerModule::OnContentBrowserContextMenu));
	}

}

void FBulkRenamerModule::ShutdownModule()
{
	FBulkRenamerStyle::Shutdown();

	FBulkRenamerCommands::Unregister();
}

void FBulkRenamerModule::PluginButtonClicked()
{
	TArray<UObject*> AssetsToEdit;

	for(const FAssetData& AssetData: SelectedAssets)
	{
		if(!AssetData.IsRedirector())
		{
			AssetsToEdit.Add(AssetData.GetAsset());
		}
	}

	if (AssetsToEdit.Num() > 0)
	{
		FBulkRenamerEditorToolkit::CreateEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), AssetsToEdit);
	}
}

TSharedRef<FExtender> FBulkRenamerModule::OnContentBrowserContextMenu(const TArray<struct FAssetData>& Assets)
{
	SelectedAssets = Assets;
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension("AssetContextAdvancedActions", EExtensionHook::After, PluginCommands,
		FMenuExtensionDelegate::CreateRaw(this, &FBulkRenamerModule::AddContentBrowserMenuExtension));
	return MenuExtender.ToSharedRef();
}

void FBulkRenamerModule::AddContentBrowserMenuExtension(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(FBulkRenamerCommands::Get().OpenBulkRenamerWindow);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBulkRenamerModule, BulkRenamer)
