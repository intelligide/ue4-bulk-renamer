// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RuleProfileManager.h"
#include "FileManagerGeneric.h"
#include "FileHelper.h"
#include "JsonObjectConverter.h"

FRuleProfileManager::FRuleProfileManager()
{
	TArray<FString> FileNames;

	FFileManagerGeneric FileManager;
	FileManager.FindFiles(FileNames, *FPaths::Combine(FPaths::ProjectSavedDir(), "BulkRenamer", "Profiles"), TEXT("json"));

	if (FileNames.Num() > 0)
	{
		for (const FString& FileName : FileNames)
		{
			FString Content;
			if (FFileHelper::LoadFileToString(Content, *FileName))
			{
				FRuleProfile Profile;

				if (FJsonObjectConverter::JsonObjectStringToUStruct(Content, &Profile, 0, 0))
				{
					Profiles.Add(Profile.Name, Profile);
				}
			}
		}
	}
}

TArray<FString> FRuleProfileManager::List() const
{
	TArray<FString> Out;
	Profiles.GetKeys(Out);
	return Out;
}

void FRuleProfileManager::Add(const FString& InName, const TArray<UBulkRenamingRule*>& InRules)
{
	if(!Profiles.Contains(InName))
	{
		FRuleProfile Profile;
		Profile.Name = InName;
		Profile.Rules = InRules;

		Profiles.Add(InName, Profile);

		

		FString Content;
		if (FJsonObjectConverter::UStructToJsonObjectString(Profile, Content, 0, 0, 4, nullptr, true))
		{
			// save to File
		}
	}
}

TArray<UBulkRenamingRule*> FRuleProfileManager::Load(const FString& InName) const
{
	if (Profiles.Contains(InName))
	{
		return  Profiles[InName].Rules;
	}
	return TArray<UBulkRenamingRule*>();
}
