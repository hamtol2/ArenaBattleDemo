// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

	// 최대 체력 설정 함수.
	//FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	// HpBar에 퍼센트를 조정할 때 호출할 함수.
	void UpdateHpBar(float NewCurrentHp);

	// 스탯을 업데이트할 때 호출할 함수.
	void UpdateStat(
		const FABCharacterStat& BaseStat, 
		const FABCharacterStat& ModifierStat
	);

	// 100 / 100 형태로 텍스트를 생성하는 함수.
	FString GetHpStatText();

protected:

	// UMG가 초기화될 때 호출되는 함수.
	virtual void NativeConstruct() override;

protected:

	// HP 게이지를 보여주기 위해 사용할 프로그레스바 참조 변수.
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	// HP 스탯 정보를 텍스트로 보여주기 위한 텍스트 블록 변수.
	// CurrentHp / MaxHp.
	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	// 현재 체력 값.
	UPROPERTY()
	float CurrentHp;

	// 최대 체력 값.
	UPROPERTY()
	float MaxHp;
};