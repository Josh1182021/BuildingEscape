// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{  
	Super::BeginPlay();

	// Initialize Rotation
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = InitialYaw + OpenAngle;

	FindAudioComponent ();

	FindPressurePlate ();
	// if (ActorThatOpens == nullptr)
	// {
	// 	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	// }	

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InGameTime = GetWorld()->GetTimeSeconds();

	// Opens Door
	if (PressurePlate && TotalMassOfActors() >= WeightToOpen)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = InGameTime;
	}
	// Closed Door
	else if (PressurePlate && TotalMassOfActors() < WeightToOpen)
	{
		if (DoorLastOpened + DoorCloseDelay < InGameTime)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	FRotator DesiredRotation = 
	{
	CurrentRotation.Pitch,
	(FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw, DeltaTime, DoorOpenSpeedSeconds)),
	CurrentRotation.Roll
	};
	
	if (!AudioComponent) {return;}
	if (DoorOpenSound == 1)
	{
	AudioComponent->Play();
	DoorOpenSound = 0;
	}

	GetOwner()->SetActorRotation(DesiredRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	FRotator DesiredRotation = 
	{
	CurrentRotation.Pitch,
	(FMath::FInterpTo(CurrentRotation.Yaw, InitialYaw, DeltaTime, DoorCloseSpeedSeconds)),
	CurrentRotation.Roll
	};

	if (!AudioComponent) {return;}
	if (DoorOpenSound == 0)
	{
	AudioComponent->Play();
	DoorOpenSound = 1;
	}

	GetOwner()->SetActorRotation(DesiredRotation);
}

float UOpenDoor::TotalMassOfActors() const


{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (int Index = 0; Index < OverlappingActors.Num(); ++Index)
	{
		TotalMass += OverlappingActors[Index]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("INdex : %i"), Index);
		UE_LOG(LogTemp, Warning, TEXT("Weight : %f"), TotalMass);
	}

	return TotalMass;
}

void UOpenDoor::FindAudioComponent ()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate () 
{
	if(!PressurePlate)
	{
		UE_LOG (LogTemp, Error, TEXT("%s has the OpenDoorComponet on it, but no presureplate set."), *GetOwner()->GetName())
	}
}

