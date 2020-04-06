// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Array.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	DestroySphereComp = CreateDefaultSubobject <USphereComponent>(TEXT("DestroySphereComp"));
	DestroySphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroySphereComp->SetSphereRadius(100);
	DestroySphereComp->AttachTo(MeshComp);

	DestroySphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);

	BlackHoleAreaSphereComp = CreateDefaultSubobject <USphereComponent>(TEXT("BlackHoleAreaSphereComp"));
	BlackHoleAreaSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlackHoleAreaSphereComp->SetSphereRadius(3000);
	BlackHoleAreaSphereComp->AttachTo(MeshComp);
	
	
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor!= this) {
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	BlackHoleAreaSphereComp->GetOverlappingComponents(OverlappingComps);

	UE_LOG(LogTemp, Warning, TEXT("%d"), sizeof(OverlappingComps));



	for (int32 i = 0; i < OverlappingComps.Num() ; i = i + 1)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];

		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = BlackHoleAreaSphereComp->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant,true);
		}
	}
}

