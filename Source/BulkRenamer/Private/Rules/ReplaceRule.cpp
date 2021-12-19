// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ReplaceRule.h"

#define LOCTEXT_NAMESPACE "BulkRenamer"

FString UReplaceRule::Apply(const FString& In) const
{	
	return In.Replace(ToCStr(TextToBeReplaced), ToCStr(ReplaceWith), CaseSensitive ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase);
}

#undef LOCTEXT_NAMESPACE

