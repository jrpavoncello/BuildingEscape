// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionReporter.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPositionReporter::UPositionReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReporter::BeginPlay()
{
	Super::BeginPlay();

	auto objectName = GetOwner()->GetName();
	auto location = GetOwner()->GetActorTransform().GetLocation();

	UE_LOG(LogTemp, Verbose, TEXT("Position reporter started for %s"), *objectName);
	UE_LOG(LogTemp, Verbose, TEXT("%s is at X=%f, Y=%f, Z=%f"), *objectName, location.X, location.Y, location.Z);
	
}

// Called every frame
void UPositionReporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

