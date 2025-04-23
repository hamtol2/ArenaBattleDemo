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
	// FPrimaryAssetId 값을 방환하는 GetPrimaryAssetId 함수 오버라이드.
	// 첫번째 인자: 태그 값.
	// 두번째 인자: 이름 값. 일반적으로 GetFName()을 많이 활용함.
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
	}

public:
	// 아이템 타입을 지정하는 열거형 변수.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
};