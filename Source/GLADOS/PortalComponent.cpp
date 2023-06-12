#include "PortalComponent.h"

UPortalComponent::UPortalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxSpawnDistance = 10000.f;
}


void UPortalComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UPortalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPortalComponent::SpawnPortalAlongVector(FVector StartLocation, FVector Direction, bool PortalA)
{
	FHitResult HitResult;
	FVector EndLocation = (Direction * MaxSpawnDistance) + StartLocation;
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams QueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel11);
	if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectQueryParams))
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, 5.f, 1.f);
		if (APortalWall* NewPortal = Cast<APortalWall>(HitResult.GetActor()))
		{
			FVector TraceLine = (HitResult.TraceStart - HitResult.TraceEnd);
			//TraceLine = TraceLine.Normalize(0.0001);
			//NewPortal->TryAddPortal()
		}
		
	}
}

