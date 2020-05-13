// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BulkRenamerEditorToolkit.h"
#include "EditorStyle.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "BulkRenamingRuleFactory.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "SAssetTable.h"
#include "Subsystems/AssetEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "BulkRenamer"

class FAssetToolsModule;
const FName FBulkRenamerEditorToolkit::ToolkitFName(TEXT("BulkRenamerEditor"));
const FName FBulkRenamerEditorToolkit::ApplicationId(TEXT("BulkRenamerEditorToolkitApp"));
const FName FBulkRenamerEditorToolkit::AssetTableId(TEXT("BulkRenamerEditorToolkit_AssetTable"));
const FName FBulkRenamerEditorToolkit::MethodListTabId(TEXT("BulkRenamerEditorToolkit_MethodList"));

FBulkRenamerEditorToolkit::FBulkRenamerEditorToolkit()
{
}

void FBulkRenamerEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->RegisterTabSpawner(AssetTableId, FOnSpawnTab::CreateSP(this, &FBulkRenamerEditorToolkit::SpawnTab_AssetTable))
		.SetDisplayName(LOCTEXT("PropertyTableTab", "Grid"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	InTabManager->RegisterTabSpawner(MethodListTabId, FOnSpawnTab::CreateSP(this, &FBulkRenamerEditorToolkit::SpawnTab_RuleList))
		.SetDisplayName(LOCTEXT("PropertiesTab", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "PropertyEditor.Grid.TabIcon"));
}

void FBulkRenamerEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(AssetTableId);
	InTabManager->UnregisterTabSpawner(MethodListTabId);
}

FName FBulkRenamerEditorToolkit::GetToolkitFName() const
{
	return ToolkitFName;
}

FText FBulkRenamerEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Bulk Renamer");
}

FText FBulkRenamerEditorToolkit::GetToolkitName() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();

	const int32 NumEditingObjects = EditingObjs.Num();

	check(NumEditingObjects > 0);

	if (NumEditingObjects == 1)
	{
		const UObject* EditingObject = EditingObjs[0];


		FFormatNamedArguments Args;
		Args.Add(TEXT("ObjectName"), FText::FromString(EditingObject->GetName()));
		Args.Add(TEXT("ToolkitName"), GetBaseToolkitName());
		return FText::Format(LOCTEXT("ToolkitName_SingleObject", "{ObjectName} - {ToolkitName}"), Args);
	}
	else
	{
		UClass* SharedBaseClass = NULL;
		for (int32 x = 0; x < NumEditingObjects; ++x)
		{
			UObject* Obj = EditingObjs[x];
			check(Obj);

			UClass* ObjClass = Cast<UClass>(Obj);
			if (ObjClass == NULL)
			{
				ObjClass = Obj->GetClass();
			}
			check(ObjClass);

			// Initialize with the class of the first object we encounter.
			if (SharedBaseClass == NULL)
			{
				SharedBaseClass = ObjClass;
			}

			// If we've encountered an object that's not a subclass of the current best baseclass,
			// climb up a step in the class hierarchy.
			while (!ObjClass->IsChildOf(SharedBaseClass))
			{
				SharedBaseClass = SharedBaseClass->GetSuperClass();
			}
		}

		FFormatNamedArguments Args;
		Args.Add(TEXT("NumberOfObjects"), EditingObjs.Num());
		Args.Add(TEXT("ClassName"), FText::FromString(SharedBaseClass->GetName()));
		return FText::Format(LOCTEXT("ToolkitName_MultiObject", "{NumberOfObjects} {ClassName} Objects - Bulk Renamer"), Args);
	}
}


FText FBulkRenamerEditorToolkit::GetToolkitToolTipText() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();

	int32 NumEditingObjects = EditingObjs.Num();

	check(NumEditingObjects > 0);

	if (NumEditingObjects == 1)
	{
		const UObject* EditingObject = EditingObjs[0];
		return FAssetEditorToolkit::GetToolTipTextForObject(EditingObject);
	}
	else
	{
		UClass* SharedBaseClass = NULL;
		for (int32 x = 0; x < NumEditingObjects; ++x)
		{
			UObject* Obj = EditingObjs[x];
			check(Obj);

			UClass* ObjClass = Cast<UClass>(Obj);
			if (ObjClass == NULL)
			{
				ObjClass = Obj->GetClass();
			}
			check(ObjClass);

			// Initialize with the class of the first object we encounter.
			if (SharedBaseClass == NULL)
			{
				SharedBaseClass = ObjClass;
			}

			// If we've encountered an object that's not a subclass of the current best baseclass,
			// climb up a step in the class hierarchy.
			while (!ObjClass->IsChildOf(SharedBaseClass))
			{
				SharedBaseClass = SharedBaseClass->GetSuperClass();
			}
		}

		FFormatNamedArguments Args;
		Args.Add(TEXT("NumberOfObjects"), NumEditingObjects);
		Args.Add(TEXT("ClassName"), FText::FromString(SharedBaseClass->GetName()));
		return FText::Format(LOCTEXT("ToolkitName_MultiObjectToolTip", "{NumberOfObjects} {ClassName} Objects - Bulk Renamer"), Args);
	}
}

FLinearColor FBulkRenamerEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FString FBulkRenamerEditorToolkit::GetWorldCentricTabPrefix() const
{
	return TEXT("");
}

TSharedRef<SDockTab> FBulkRenamerEditorToolkit::SpawnTab_AssetTable(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == AssetTableId);

	TSharedRef<SDockTab> AssetTableTab = SNew(SDockTab)
	.Icon(FEditorStyle::GetBrush("PropertyEditor.Grid.TabIcon"))
	.Label(LOCTEXT("GenericGridTitle", "Grid"))
	.TabColorScale(GetTabColorScale())
	.Content()
	[
		SNew(SBorder)
		.Padding(FMargin(3))
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SAssignNew(AssetTable, SAssetTable, AssetTableRows)
		]
	];

	return AssetTableTab;
}



struct FFactoryItem
{
	UBulkRenamingRuleFactory* Factory;
	FText DisplayName;

	FFactoryItem(UBulkRenamingRuleFactory* InFactory, const FText& InDisplayName)
		: Factory(InFactory), DisplayName(InDisplayName)
	{}
};

TArray<FFactoryItem> FindFactories()
{
	TArray<FFactoryItem> Factories;
	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;

		if (Class->IsChildOf(UBulkRenamingRuleFactory::StaticClass()) && !Class->HasAnyClassFlags(CLASS_Abstract))
		{
			UBulkRenamingRuleFactory* Factory = Class->GetDefaultObject<UBulkRenamingRuleFactory>();
			if (ensure(!Factory->GetDisplayName().IsEmpty()))
			{
				new(Factories)FFactoryItem(Factory, Factory->GetDisplayName());
			}
		}
	}

	return Factories;
}

TSharedRef<SDockTab> FBulkRenamerEditorToolkit::SpawnTab_RuleList(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == MethodListTabId);

	TSharedRef<SDockTab> MethodListTab = SNew(SDockTab)
	.Icon(FEditorStyle::GetBrush("PropertyEditor.Properties.TabIcon"))
	.Label(LOCTEXT("GenericDetailsTitle", "Details"))
	.TabColorScale(GetTabColorScale())
	.Content()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SAssignNew(RuleList, SRuleList)
			.OnModified(this, &FBulkRenamerEditorToolkit::OnRulesChanged)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Bottom)
		[
			SNew(SBorder)
			.Padding(FMargin(3))
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				[
					SNew(SComboButton)
					.ComboButtonStyle(FEditorStyle::Get(), "ToolbarComboButton")
					.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
					.ForegroundColor(FLinearColor::White)
					.ContentPadding(FMargin(6, 2))
					.OnGetMenuContent(this, &FBulkRenamerEditorToolkit::MakeRulesMenu)
					.HasDownArrow(false)
					.ButtonContent()
					[
						SNew(SHorizontalBox)

						// New Text
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						.Padding(4, 0, 0, 0)
						[
							SNew(STextBlock)
							.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
							.Text(LOCTEXT("NewButton", "Add New Rule"))
						]

						// Down Arrow
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.AutoWidth()
						.Padding(4, 0, 0, 0)
						[
							SNew(STextBlock)
							.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
							.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.10"))
							.Text(FEditorFontGlyphs::Caret_Down)
						]
					]
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				[
					SNew(SButton)
					.ButtonStyle(FEditorStyle::Get(), "FlatButton")
					.ContentPadding(FMargin(6, 2))
					.OnClicked(this, &FBulkRenamerEditorToolkit::Run)
					[
						SNew(SHorizontalBox)

						// Icon
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							SNew(STextBlock)
							.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
							.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.11"))
							.Text(FEditorFontGlyphs::Play)
						]

						// Text
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						.Padding(4, 0, 0, 0)
						[
							SNew(STextBlock)
							.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
							.Text(LOCTEXT("Execute", "Execute"))
						]

					]
				]
			]
		]
	];

	return MethodListTab;
}

void FBulkRenamerEditorToolkit::AddRule(UBulkRenamingRuleFactory* Factory)
{
	RuleList->AddRule(Factory->Create());
}

void FBulkRenamerEditorToolkit::Initialize(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UObject*>& ObjectsToEdit)
{
	TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_BulkRenamerEditorToolkit_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.8f)
				->AddTab(AssetTableId, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->SetHideTabWell(true)
				->AddTab(MethodListTabId, ETabState::OpenedTab)
			)
		);

	for(UObject* Asset: ObjectsToEdit)
	{
		TSharedPtr<FAssetTableRowData> RowData = MakeShared<FAssetTableRowData>(Asset->GetName(), Asset->GetName(), Asset->GetPathName());
		AssetTableRows.Add(RowData);
	}

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = false;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, ApplicationId, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectsToEdit);
}


TSharedPtr<FBulkRenamerEditorToolkit> FBulkRenamerEditorToolkit::FindExistingEditor(UObject* Object)
{
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	const TArray<IAssetEditorInstance*> Editors = AssetEditorSubsystem->FindEditorsForAsset(Object);

	IAssetEditorInstance* const * ExistingInstance = Editors.FindByPredicate([&](IAssetEditorInstance* Editor) {
		return Editor->GetEditorName() == ToolkitFName;
	});

	if (ExistingInstance)
	{
		auto* PropertyEditor = static_cast<FBulkRenamerEditorToolkit*>(*ExistingInstance);
		return PropertyEditor->SharedThis(PropertyEditor);
	}

	return nullptr;
}

TSharedRef<FBulkRenamerEditorToolkit> FBulkRenamerEditorToolkit::CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UObject* ObjectToEdit)
{
	auto ExistingEditor = FindExistingEditor(ObjectToEdit);
	if (ExistingEditor.IsValid())
	{
		ExistingEditor->FocusWindow();
		return ExistingEditor.ToSharedRef();
	}

	TSharedRef<FBulkRenamerEditorToolkit> NewEditor(new FBulkRenamerEditorToolkit());

	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(ObjectToEdit);
	NewEditor->Initialize(Mode, InitToolkitHost, ObjectsToEdit);

	return NewEditor;
}


TSharedRef<FBulkRenamerEditorToolkit> FBulkRenamerEditorToolkit::CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, const TArray<UObject*>& ObjectsToEdit)
{
	if (ObjectsToEdit.Num() == 1)
	{
		auto ExistingEditor = FindExistingEditor(ObjectsToEdit[0]);
		if (ExistingEditor.IsValid())
		{
			ExistingEditor->FocusWindow();
			return ExistingEditor.ToSharedRef();
		}
	}

	TSharedRef<FBulkRenamerEditorToolkit> NewEditor(new FBulkRenamerEditorToolkit());
	NewEditor->Initialize(Mode, InitToolkitHost, ObjectsToEdit);

	return NewEditor;
}

FReply FBulkRenamerEditorToolkit::Run()
{
	TArray<UBulkRenamingRule*> Rules = RuleList->GetRules();
	if(Rules.Num() > 0)
	{
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		TArray<FAssetRenameData> AssetRenameData;

		for (UObject* Asset : GetEditingObjects())
		{
			const FString PackagePath = FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName());
			new(AssetRenameData) FAssetRenameData(Asset, PackagePath, ApplyRules(Asset->GetName()));
		}

		if(AssetToolsModule.Get().RenameAssets(AssetRenameData))
		{
			CloseWindow();
		}
	}

	return FReply::Handled();
}

FString FBulkRenamerEditorToolkit::ApplyRules(FString str)
{
	for(UBulkRenamingRule* Rule: RuleList->GetRules())
	{
		str = Rule->Apply(str);
	}
	return str;
}

void FBulkRenamerEditorToolkit::OnRulesChanged()
{
	for (const TSharedPtr<FAssetTableRowData>& Asset : AssetTableRows)
	{
		Asset->NewFilename = ApplyRules(Asset->Filename);
	}
	AssetTable->Refresh();
}

TSharedRef<SWidget> FBulkRenamerEditorToolkit::MakeRulesMenu()
{
	FMenuBuilder MenuBuilder(/*bInShouldCloseWindowAfterMenuSelection=*/true, NULL);

	TArray<FFactoryItem> Factories = FindFactories();
	if (Factories.Num() > 0)
	{
		MenuBuilder.BeginSection("ContentBrowserGetContent", LOCTEXT("GetContentMenuHeading", "Content"));
		{
			for (const FFactoryItem& FactoryItem : Factories)
			{
				MenuBuilder.AddMenuEntry(
					FactoryItem.DisplayName,
					FText::GetEmpty(),
					FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentBrowser.AddContent"),
					FUIAction(FExecuteAction::CreateRaw(this, &FBulkRenamerEditorToolkit::AddRule, FactoryItem.Factory))
				);
			}
		}
		MenuBuilder.EndSection();
	}

	FDisplayMetrics DisplayMetrics;
	FSlateApplication::Get().GetCachedDisplayMetrics(DisplayMetrics);

	const FVector2D DisplaySize(
		DisplayMetrics.PrimaryDisplayWorkAreaRect.Right - DisplayMetrics.PrimaryDisplayWorkAreaRect.Left,
		DisplayMetrics.PrimaryDisplayWorkAreaRect.Bottom - DisplayMetrics.PrimaryDisplayWorkAreaRect.Top);

	return
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.MaxHeight(DisplaySize.Y * 0.9)
		[
			MenuBuilder.MakeWidget()
		];
}

#undef LOCTEXT_NAMESPACE
