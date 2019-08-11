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

	Owner = GetOwner();
	World = GetWorld();

	ActorThatOpens = World->GetFirstPlayerController()->GetPawn();
	OriginalAngle = Owner->GetActorRotation().Yaw;
}

void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = World->GetTimeSeconds();
	}

	if (LastDoorOpenTime != 0 &&
		World->GetTimeSeconds() - LastDoorOpenTime > SecondsToClose)
	{
		CloseDoor();
	}
}

void UDoorOpener::OpenDoor()
{
	Owner->SetActorRelativeRotation(FRotator(0, OpenAngle, 0));
}

void UDoorOpener::CloseDoor()
{
	Owner->SetActorRelativeRotation(FRotator(0, OriginalAngle, 0));
}
