// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	PhysicsHandleComponent();
	SetupInputComponent();
	
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))

		//LineTrace to find actors actors with a physics body collision channel set
		//On ray hit attach physics handle
		//Attach Physics Handle
		auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
	
	

}


void UGrabber::UnGrab()
{
	//TODO Release Physics handle
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"))
		PhysicsHandle->ReleaseComponent();
}

void UGrabber::PhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has physics handle"), *GetOwner()->GetName())
	}
	else
	{
		//handle not found	
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handle"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//input handle is found
		UE_LOG(LogTemp, Warning, TEXT("%s has input handle"), *GetOwner()->GetName())
			//Bind input axis
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::UnGrab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing input handle"), *GetOwner()->GetName())
	}
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Get Player ViewPoint 
	//Ray Casting from actor/player
	//Check Ray Collision
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	//If Pyhsics hand is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//move object we are holding 
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	

	
	
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get Player ViewPoint 
	//Ray Casting from actor/player
	//Check Ray Collision
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);



	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	
	//Log out to test
	//Set up query parameters

	//Line trace/Ray cast ,reach distance
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody)
	);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit : %s"), *(ActorHit->GetName()))
	}
	return Hit;
}