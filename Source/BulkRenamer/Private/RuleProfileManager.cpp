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

#include "RuleProfileManager.h"
#include "HAL/FileManagerGeneric.h"
#include "Misc/FileHelper.h"
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
