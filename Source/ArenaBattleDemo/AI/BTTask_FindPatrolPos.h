// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrolPos();
	
	// 태스크가 실행될 때 호출되는 함수.
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};