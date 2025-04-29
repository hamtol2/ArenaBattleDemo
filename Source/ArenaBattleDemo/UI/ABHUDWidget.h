// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UABHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	// 스탯을 업데이트할 때 사용할 함수.
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

	// HP 값이 업데이트될 때 사용할 함수.
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

	// HpBar 위젯.
	UPROPERTY()
	TObjectPtr<class UABHpBarWidget> HpBar;

	// CharacterStat 위젯.
	UPROPERTY()
	TObjectPtr<class UABCharacterStatWidget> CharacterStat;
};