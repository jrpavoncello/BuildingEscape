// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 200.f;

	UPhysicsHandleComponent* PhysicsHandle;
	UInputComponent* InputComponent;
	void Grab();
	void Release();

	AActor* grabbedActor;
	FRotator grabbedObjectStartRotation;
	float grabberStartYaw = 0.f;
	FVector grabOffset;

	void GetTraceBegin(FVector& traceBegin, FRotator& rotation);
	FVector GetTraceEnd(const FVector& traceBegin, const FRotator& rotation);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
