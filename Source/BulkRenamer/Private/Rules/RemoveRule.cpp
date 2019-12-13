// Copyright 1998 - 2019 Epic Games, Inc.All Rights Reserved.

#include "RemoveRule.h"

#define LOCTEXT_NAMESPACE "BulkRenamer"

FString URemoveRule::Apply(const FString& In) const
{
	FString Copy = In;

	int Idx = At;
	if (Idx < 0 || Idx > Copy.Len())
	{
		return Copy;
	}
	
	if (Backward)
	{
		Idx = Copy.Len() - At - 1;
	}

	int C = Count;
	if ( Idx + C > Copy.Len())
	{
		C = Copy.Len() - Idx;
	}

	if (C >= Copy.Len())
	{
		return Copy;
	}
	
	Copy.RemoveAt(Idx, C);
	return Copy;
}

#undef LOCTEXT_NAMESPACE
