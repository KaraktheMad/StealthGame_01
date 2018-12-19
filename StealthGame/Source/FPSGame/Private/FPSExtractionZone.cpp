// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include <Components/PrimitiveComponent.h>
#include <Components/DecalComponent.h>
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;

	// Shows the wire frame of the collusion volume while working on it
	OverlapComp->SetHiddenInGame(false);

	// This is an alternative way bind the event whenever we overlap the player. See void AFPSObjective::NotifyActorBeginOverlap(AActor* OtherActor)
	// in FPSObjective.cpp for code.
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	// Material component
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f);
	DecalComp->SetupAttachment(RootComponent);
}



void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
		AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (!MyPawn)
	{
		return;
	}

	if (MyPawn->bIsCarryingObjective)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"))

	
	
}



