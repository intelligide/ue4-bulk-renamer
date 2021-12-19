// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AddRule.h"

#define LOCTEXT_NAMESPACE "BulkRenamer"

FString UAddRule::Apply(const FString& In) const
{
	FString Copy = In;

	int Idx = Index;
	if(Idx < 0 || Idx > Copy.Len())
	{
		return Copy;
	}
	
	if(Backward)
	{
		Idx = Copy.Len() - Index;
	}
	
	Copy.InsertAt(Idx, Str);
	return Copy;
}

#undef LOCTEXT_NAMESPACE
