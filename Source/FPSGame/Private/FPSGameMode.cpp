// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		TArray<AActor*> ReturnedActors;

		if (SpectatingViewpointClass) 
		{
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
		}

		APlayerController* PC = Cast <APlayerController>(InstigatorPawn->GetController());

		//Change view target if any valid actor found

		if (ReturnedActors.Num() > 0)
		{
			AActor* NewViewtarget = ReturnedActors[0];
			if (PC)
			{
				PC->SetViewTargetWithBlend(NewViewtarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
	}

	OnMissionCompleted(InstigatorPawn);

	
}
