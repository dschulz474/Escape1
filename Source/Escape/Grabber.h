// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"






UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;	
	//Ray cast and grab in reach of ray
	void Grab();
	void Release();

	void PhysicsHandleComponent();

	//Setup (assumed) attach input component 
	void SetupInputComponent();
	
	//Return Hit for first physics Body in Reach
	const FHitResult GetFirstPhysicsBodyInReach();
	//Returns beginning of reach line
	FVector GetReachLineStart();
	//Returns current end of reach line
	FVector GetReachLineEnd();


	
};
