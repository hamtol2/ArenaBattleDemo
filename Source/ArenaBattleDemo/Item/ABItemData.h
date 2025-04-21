// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"

// 아이템 종류 열거형.
// 블루프린트와 호환되도록 BlueprintType 지정.
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll
};

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 아이템 타입을 지정하는 열거형 변수.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
};