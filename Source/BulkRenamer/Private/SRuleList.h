// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STableRow.h"
#include "SScrollBox.h"
#include "BulkRenamingRule.h"

DECLARE_DELEGATE(FOnModified);

class SRuleList : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SRuleList) {}

	SLATE_EVENT(FOnModified, OnModified)
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void AddRule(UBulkRenamingRule* Rule);
	
	inline TArray<UBulkRenamingRule*> GetRules() const
	{
		return Rules;
	}

private:
	void OnRuleChanged(const FPropertyChangedEvent& InEvent);
	
	FOnModified ModifiedDelegate;
	
	TArray<UBulkRenamingRule*> Rules;
	TSharedPtr<SScrollBox> RulesBox;
};
