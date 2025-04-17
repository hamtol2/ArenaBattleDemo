// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABCharacterNonPlayer : public AABCharacterBase
{
	GENERATED_BODY()

public:
	AABCharacterNonPlayer();

protected:

	// 죽음 상태 설정 함수.
	virtual void SetDead() override;
};