/*
 * MIT License
 *
 * Copyright (c) 2019-2022 Yoann Potinet
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
