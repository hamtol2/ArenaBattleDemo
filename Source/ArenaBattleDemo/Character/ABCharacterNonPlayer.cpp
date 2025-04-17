// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// 타이머를 사용해 액터 제거.
	FTimerHandle DeadTimerHandle;

	// 타이머 설정.
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				// 액터 제거.
				Destroy();
			}
		),
		DeadEventDelayTime,		// 타이머 설정 시간.
		false					// 반복 여부 설정(반복 안함).
	);
}