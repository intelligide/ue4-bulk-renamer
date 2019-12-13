// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BulkRenamingRule.h"
#include "RuleProfileManager.generated.h"

USTRUCT()
struct FRuleProfile
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	TArray<UBulkRenamingRule*> Rules;
};

class FRuleProfileManager
{
public:
	FRuleProfileManager();
	
	TArray<FString> List() const;
	void Add(const FString& InName, const TArray<UBulkRenamingRule*>& InRules);
	TArray<UBulkRenamingRule*> Load(const FString& InName) const;

private:
	TMap<FString, FRuleProfile> Profiles;
};
