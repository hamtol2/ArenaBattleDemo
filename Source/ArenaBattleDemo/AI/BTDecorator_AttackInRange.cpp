// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAI.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	// 에디터에서 보일 노드 이름 설정.
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AIController가 제어하는 폰.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	// 폰이 없으면 종료.
	if (!ControllingPawn)
	{
		return false;
	}

	// 인터페이스로 형변환.
	IABCharacterAIInterface* AIPawn 
		= Cast<IABCharacterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return false;
	}

	// 블랙보드에 저장된 캐릭터 값.
	APawn* Target 
		= Cast<APawn>(
			OwnerComp.GetBlackboardComponent()->GetValueAsObject(
				BBKEY_TARGET
			)
		);

	// 블랙보드에 저장된 값이 없으면 종료.
	if (!Target)
	{
		return false;
	}

	// 캐릭터와의 거리 계산.
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	// 공격 범위 값.
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();

	// 캐릭터와의 거리가 공격 가능거리보다 가까운지 확인 후 결과 반환.
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	return bResult;
}