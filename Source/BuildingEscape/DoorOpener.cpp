#include "DoorOpener.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

UDoorOpener::UDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!ActorThatOpens)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor that opens (first player controller) was not configured on %s."), *GetOwner()->GetName());
	}

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Pressure plate was not configured on %s."), *GetOwner()->GetName());
	}

	OriginalAngle = GetOwner()->GetActorRotation().Yaw;
}

void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (LastDoorOpenTime != 0 &&
		GetWorld()->GetTimeSeconds() - LastDoorOpenTime > SecondsToClose)
	{
		CloseDoor();
	}
}

void UDoorOpener::OpenDoor()
{
	GetOwner()->SetActorRelativeRotation(FRotator(0, OpenAngle, 0));
}

void UDoorOpener::CloseDoor()
{
	GetOwner()->SetActorRelativeRotation(FRotator(0, OriginalAngle, 0));
}
