// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BulkRenamingRule.h"
#include "MoveRule.generated.h"

UCLASS()
class UMoveRule : public UBulkRenamingRule
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint32 From;

	UPROPERTY(EditAnywhere)
	uint32 Count = 0;

	UPROPERTY(EditAnywhere)
	uint32 To = 0;

	UPROPERTY(EditAnywhere)
	bool Backward = false;
	
	FString Apply(const FString&) const override;
};
