// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDIING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors ()const;
	void FindAudioComponent();
	void FindPressurePlate ();

private:
	float InitialYaw;
	float TargetYaw;
	float DoorLastOpened = 0.f;
	float InGameTime = 0.f;
	bool DoorOpenSound = 1;

	UPROPERTY(EditAnywhere)
	float WeightToOpen = 100.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeedSeconds = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeedSeconds = 2.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	// UPROPERTY(EditAnywhere)
	// AActor* ActorThatOpens =  nullptr;

};
  