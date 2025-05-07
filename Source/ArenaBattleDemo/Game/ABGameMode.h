// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/ABGameInterface.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABGameMode : 
	public AGameModeBase,
	public IABGameInterface
{
	GENERATED_BODY()
	
public:
	AABGameMode();

	// IABGameInterface 함수 구현.
	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	// 게임을 클리어하는데 필요한 점수.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore;

	// 현재 게임의 점수.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore;

	// 게임이 클리어 됐는지 여부를 나타내는 변수.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;
};