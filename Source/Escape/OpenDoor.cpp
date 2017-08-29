// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "OpenDoor.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	
	if (!PressurePlate)
	{
		UE_LOG(LogTemp,Error,TEXT("%s is missing pressure plate"), *GetOwner()->GetName())
	}
	
	// ...
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));//set door rotation
}

void UOpenDoor::CloseDoor()
{
	//Create Rotator
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));//set door rotation
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	//Poll Trigger volume every frame	
	if (GetTotalMassOfActorsOnPlate() > 30.f)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	//check if its time to close door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}

}
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	//Find all overlapping actors
	
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* Actor : OverlappingActors)
	{
		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
		
	}
	//Iterate through them adding their masses
	
	
	return TotalMass;
}

