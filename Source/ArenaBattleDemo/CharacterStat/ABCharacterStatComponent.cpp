// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	//MaxHp = 200.0f;
	//CurrentHp = MaxHp;
	CurrentLevel = 1.0f;
	AttackRadius = 50.0f;

	// bWantsInitializeComponent 플래그를 설정해야
	// InitializeComponent 함수가 호출됨.
	// 성능 상 영향을 줄 수 있기 때문에 필요한 경우에만 
	// 사용하도록 설계한 것으로 보임.
	bWantsInitializeComponent = true;
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 레벨 스탯 데이터 설정.
	SetLevelStat(static_cast<int32>(CurrentLevel));

	// 게임이 시작되면 체력을 가득 채우기.
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	// 유효한 레벨 범위로 고정.
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);

	// 게임 싱글톤으로부터 레벨 데이터 가져오기.
	BaseStat = UABGameSingleton::Get().GetCharacterStat(CurrentLevel);

	// 설정한 값에 문제 없는지 확인.
	check(BaseStat.MaxHp > 0.0f);
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
	//CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	// 체력 변경 이벤트 발행.
	OnHpChanged.Broadcast(CurrentHp);
}