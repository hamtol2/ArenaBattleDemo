// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

// 델리게이트 선언.
// 체력 값이 0이 되었을 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// 체력 변경이 발생할 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLEDEMO_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

	// 컴포넌트가 초기화되는 함수.
	virtual void InitializeComponent() override;
	
public:
	// Getter.
	// __forceinline.
	//FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// 캐릭터 레벨을 설정하는 함수.
	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }

	// 부가 스탯데이터 설정 함수.
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;
	}

	// 전체 스탯 데이터 반환 함수.
	FORCEINLINE FABCharacterStat GetTotalStat() const
	{ 
		return BaseStat + ModifierStat;
	}

	// 대미지 전달 함수.
	float ApplyDamage(float InDamage);

protected:
	// HP가 변경됐을 때 실행할 함수.
	void SetHp(float NewHp);

public:
	// 체력을 모두 소진했을 때 발행되는 델리게이트.
	FOnHpZeroDelegate OnHpZero;

	// 체력 변경 델리게이트.
	FOnHpChangedDelegate OnHpChanged;

	// 스탯.
protected:

	// 기존에 임시로 사용하던 데이터 제거(비활성화).
	//// 최대 체력 값.
	//UPROPERTY(VisibleInstanceOnly, Category = Stat)
	//float MaxHp;

	// 현재 체력 값.
	// Transient: 현재 체력 값은 게임을 진행할 때마다 바뀌는 값.
	// 따라서 디스크에 명시적으로 저장할 필요하지 않을 수 있음.
	// 이럴 때는 Transient로 지정 가능.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// 현재 레벨.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	// 공격 범위.
	UPROPERTY(VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	// 캐릭터의 기본 스탯 데이터.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// 부가 스탯 데이터.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};