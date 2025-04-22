// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmick/ABStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"

// Sets default values
AABStageGimmick::AABStageGimmick()
{
	// State Section.
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	// 리소스 설정.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(
		this, 
		&AABStageGimmick::OnStageTriggerBeginOverlap
	);

	// Gate Section.
	static FName GateSockets[] = 
	{
		TEXT("+XGate"),
		TEXT("-XGate"),
		TEXT("+YGate"),
		TEXT("-YGate")
	};

	// 리소스 로드.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE"));

	// 게이트 생성.
	for (const FName& GateSocket : GateSockets)
	{
		// 컴포넌트 생성.
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);

		// 생성한 스태틱 메시 컴포넌트에 애셋 설정.
		if (GateMeshRef.Object)
		{
			Gate->SetStaticMesh(GateMeshRef.Object);
		}

		// 컴포넌트 설정.
		Gate->SetupAttachment(Stage, GateSocket);
		Gate->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		// 생성한 컴포넌트를 맵에 추가.
		Gates.Add(GateSocket, Gate);

		// 문마다 배치할 BoxComponent 생성 및 설정.
		FName TriggerName = *GateSocket.ToString().Append("Trigger");

		// 컴포넌트 생성.
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);

		// 컴포넌트 설정.
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(
			this,
			&AABStageGimmick::OnGateTriggerBeginOverlap
		);

		// 태그 설정.
		GateTrigger->ComponentTags.Add(GateSocket);

		// 생성한 컴포넌트를 배열에 추가.
		GateTriggers.Add(GateTrigger);
	}

	// 시작할 때는 준비 상태로 설정.
	CurrentState = EStageState::Ready;

	// 열거형 - 델리게이트 맵 설정.
	StageChangedActions.Add(EStageState::Ready, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady));

	StageChangedActions.Add(EStageState::Fight, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight));

	StageChangedActions.Add(EStageState::Reward, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward));

	StageChangedActions.Add(EStageState::Next, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext));
}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// OnConstruction이 호출될 때 상태 값도 갱신되도록 처리.
	SetState(CurrentState);
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	// 예외 처리.
	//if (CurrentState == InNewState)
	//{
	//	return;
	//}

	// 현재 상태 업데이트.
	CurrentState = InNewState;

	// 전달된 상태에 맵에 포함돼 있으면 델리게이트 실행.
	if (StageChangedActions.Contains(InNewState))
	{
		StageChangedActions[CurrentState].StageChangedDelegate.ExecuteIfBound();
	}
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AABStageGimmick::SetReady()
{
	// 가운데 트리거 활성화.
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// 플레이어가 게이트와 상호작용하지 않도록 콜리전 끄기.
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 모든 문 닫기.
	CloseAllGates();
}

void AABStageGimmick::SetFight()
{
	// 가운데 트리거 활성화.
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// 플레이어가 게이트와 상호작용하지 않도록 콜리전 끄기.
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 모든 문 닫기.
	CloseAllGates();
}

void AABStageGimmick::SetChooseReward()
{
	// 가운데 트리거 활성화.
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// 플레이어가 게이트와 상호작용하지 않도록 콜리전 끄기.
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 모든 문 닫기.
	CloseAllGates();
}

void AABStageGimmick::SetChooseNext()
{
	// 가운데 트리거 활성화.
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// 플레이어가 게이트와 상호작용하지 않도록 콜리전 끄기.
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}

	// 모든 문 열기 (다른 스테이지로 이동할 수 있도록).
	OpenAllGates();
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AABStageGimmick::OpenAllGates()
{
	// 문 액터의 배열을 순회하면서 회전 설정.
	for (const auto& Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	// 문 액터의 배열을 순회하면서 회전 설정.
	for (const auto& Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator::ZeroRotator);
	}
}