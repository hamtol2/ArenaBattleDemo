// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"

AABGameMode::AABGameMode()
{
	// DefaultPawnClass.
	//static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(
	//	TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(
		TEXT("/Script/ArenaBattleDemo.ABCharacterPlayer"));

	if (DefaultCharacterRef.Class)
	{
		DefaultPawnClass = DefaultCharacterRef.Class;
	}

	// PlayerControllerClass.
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattleDemo.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	//PlayerControllerClass = AABPlayerController::StaticClass();
}