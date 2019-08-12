#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!physicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing physics handle component in %s."), *GetOwner()->GetName());
	}

	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!inputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing input component in %s."), *GetOwner()->GetName());
		return;
	}

	inputComponent->BindAction
	(
		"Grab",
		EInputEvent::IE_Pressed,
		this,
		&UGrabber::Grab
	);

	inputComponent->BindAction
	(
		"Grab",
		EInputEvent::IE_Released,
		this,
		&UGrabber::Release
	);
}

void UGrabber::GetTraceBegin(FVector& out_traceBegin, FRotator& out_rotation)
{
	FVector location;
	FRotator rotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(out_traceBegin, out_rotation);
}

FVector UGrabber::GetTraceEnd(const FVector& traceBegin, const FRotator& rotation)
{
	return traceBegin + rotation.Vector() * reach;
}

void UGrabber::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (!physicsHandle)
	{
		return;
	}

	if (!physicsHandle->GrabbedComponent)
	{
		return;
	}

	FVector traceBegin;
	FRotator rotation;
	GetTraceBegin(traceBegin, rotation);

	UE_LOG(LogTemp, VeryVerbose, TEXT("Location: %s, Rotation: %s"), *traceBegin.ToString(), *rotation.ToString());

	auto traceEnd = GetTraceEnd(traceBegin, rotation);

	auto grabberYaw = rotation.Yaw;

	UE_LOG(LogTemp, VeryVerbose, TEXT("Grabber yaw %f"), grabberYaw);

	float grabberYawDelta = grabberYaw - grabberStartYaw;

	UE_LOG(LogTemp, VeryVerbose, TEXT("Grabber delta %f"), grabberYawDelta);

	auto newRotation = grabbedObjectStartRotation;

	newRotation.Yaw = grabbedObjectStartRotation.Yaw + grabberYawDelta;

	UE_LOG(LogTemp, VeryVerbose, TEXT("New target rotation %s"), *newRotation.ToString());

	physicsHandle->SetTargetLocation(traceEnd);
	physicsHandle->SetTargetRotation(newRotation);
}

void UGrabber::Grab()
{
	if (!physicsHandle)
	{
		return;
	}

	FVector traceBegin;
	FRotator rotation;
	GetTraceBegin(traceBegin, rotation);
	auto traceEnd = GetTraceEnd(traceBegin, rotation);

	//DrawDebugLine(GetWorld(), traceBegin, traceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

	auto queryParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult outHit;
	if (inputComponent->
		GetWorld()->LineTraceSingleByObjectType(
			outHit,
			traceBegin,
			traceEnd,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			queryParams))
	{
		grabbedActor = outHit.GetActor();

		if (grabbedActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Grabbed actor %s"), *grabbedActor->GetName());

			auto component = outHit.GetComponent();

			physicsHandle->GrabComponent(
				component,
				NAME_None,
				traceEnd,
				// constrain rotation
				true);

			UE_LOG(LogTemp, VeryVerbose, TEXT("Grabbed object start rotation %s"), *grabbedObjectStartRotation.ToString());
			UE_LOG(LogTemp, VeryVerbose, TEXT("Grabber start yaw %f"), grabberStartYaw);

			grabbedObjectStartRotation = grabbedActor->GetActorRotation();
			grabberStartYaw = rotation.Yaw;
		}
	}
}

void UGrabber::Release()
{
	if (!physicsHandle)
	{
		return;
	}

	grabbedActor = nullptr;
	UE_LOG(LogTemp, Log, TEXT("Released!"));
	physicsHandle->ReleaseComponent();
}

