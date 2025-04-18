// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}

// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작되면 체력을 가득 채우기.
	SetHp(MaxHp);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	// 대미지 처리.
	const float PrevHp = CurrentHp;

	// 대미지 보정 값.
	// 오류로 인해 전달된 대미지가 음수일 수 있음.
	// 음수인 경우는 0 설정.
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);

	// 대미지 계산.
	SetHp(PrevHp - ActualDamage);

	// 죽었는지 (체력을 모두 소진했는지) 확인.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// 이벤트 발행.
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	// 현재 체력 업데이트.
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	// 체력 변경 이벤트 발행.
	OnHpChanged.Broadcast(CurrentHp);
}