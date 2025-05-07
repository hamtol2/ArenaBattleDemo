// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "ABCharacterControlData.h"
#include "UI/ABHUDWidget.h"
#include "CharacterStat/ABCharacterStatComponent.h"

#include "Interface/ABGameInterface.h"
#include "GameFramework/GameModeBase.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Input.

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderMoveActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
	if (ShoulderMoveActionRef.Object)
	{
		ShoulderMoveAction = ShoulderMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderLookActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
	if (ShoulderLookActionRef.Object)
	{
		ShoulderLookAction = ShoulderLookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuarterMoveActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_QuarterMove.IA_QuarterMove"));
	if (QuarterMoveActionRef.Object)
	{
		QuarterMoveAction = QuarterMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeControlActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl"));
	if (ChangeControlActionRef.Object)
	{
		ChangeControlAction = ChangeControlActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

	// 초기 설정.
	CurrentCharacterControlType = ECharacterControlType::Quarter;
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Controller.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// 입력 활성화.
		EnableInput(PlayerController);
	}

	// 입력 설정.
	SetCharacterControl(CurrentCharacterControlType);
}

void AABCharacterPlayer::SetDead()
{
	Super::SetDead();

	// Controller.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// 입력 비활성화.
		DisableInput(PlayerController);

		// 게임 종료 처리를 위해 게임 모드 가져오기.
		IABGameInterface* ABGameMode 
			= Cast<IABGameInterface>(GetWorld()->GetAuthGameMode());

		// 형변환 확인.
		if (ABGameMode)
		{
			// 플레이어의 죽음을 알림.
			ABGameMode->OnPlayerDead();
		}
	}
}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Binding.
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Attack);
	
	
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuarterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuarterMove);
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	// 변경할 컨트롤 타입에 대응하는 데이터 애셋 로드(TMap으로부터).
	UABCharacterControlData* NewCharacterControl =  CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	// 데이터 애셋을 사용해 관련 값 설정.
	SetCharacterControlData(NewCharacterControl);

	// Add InputMapping Context to Enhanced Input System.
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(
			NewCharacterControl->InputMappingContext,
			0
		);
	}

	// 현재 사용 중인 캐릭터 컨트롤 타입 업데이트.
	CurrentCharacterControlType = NewCharacterControlType;
}

void AABCharacterPlayer::SetCharacterControlData(const class UABCharacterControlData* InCharacterControlData)
{
	Super::SetCharacterControlData(InCharacterControlData);

	// SpringArm 관련 설정.
	SpringArm->TargetArmLength = InCharacterControlData->TargetArmLength;
	SpringArm->SetRelativeRotation(InCharacterControlData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = InCharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = InCharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = InCharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = InCharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = InCharacterControlData->bDoCollisionTest;
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	// 사용할 캐릭터 컨트롤을 변경하는 함수.
	if (CurrentCharacterControlType == ECharacterControlType::Quarter)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quarter);
	}
}

void AABCharacterPlayer::QuarterMove(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();

	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = Movement.SizeSquared();

	// 두 방향으로 입력이 들어오면, 이동 방향은 정규화해 크기를 1로 만들고,
	// 입력 스케일을 1로 강제 설정.
	if (MovementVectorSizeSquared > 1.0f)
	{
		Movement.Normalize();
		MovementVectorSize = 1.0f;
	}
	// 입력이 1이하이면, 해당 입력을 스케일로 사용하기 위해 값 계산.
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(Movement.X, Movement.Y, 0.0f);

	// 캐릭터가 이동하는 방향에 맞게 컨트롤러 회전 설정.
	Controller->SetControlRotation(
		FRotationMatrix::MakeFromX(MoveDirection).Rotator()
	);
	
	// 입력에 따른 방향으로 이동하도록 입력 전달.
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AABCharacterPlayer::Attack()
{
	// 공격 입력 처리 함수 호출.
	ProcessComboCommand();
}

void AABCharacterPlayer::SetupHUDWidget(UABHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		// 스탯 정보를 UI에 전달.
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());

		// Hp 정보 전달.
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

		// 델리게이트에 등록.
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBar);
	}
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();

	// 컨트롤러의 회전 값.
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// 방향 구하기.
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 무브먼트 컴포넌트에 값 전달.
	AddMovementInput(ForwardVector, Movement.X);
	AddMovementInput(RightVector, Movement.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D LookVector = Value.Get<FVector2D>();

	// 컨트롤러에 회전 추가.
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}