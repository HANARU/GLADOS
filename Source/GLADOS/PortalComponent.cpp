#include "PortalComponent.h"
#include "SinglePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"


UPortalComponent::UPortalComponent()
{
	MaxSpawnDistance = 10000.f;
}


void UPortalComponent::BeginPlay()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PortalGunMappingContext, 0);
		}
	}

	Player = Cast<ASinglePlayer>(GetOwner());

	

}

void UPortalComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MouseLeftclickAction, ETriggerEvent::Triggered, this, &UPortalComponent::SpawnLeftBlue);
	EnhancedInputComponent->BindAction(MouseRightclickAction, ETriggerEvent::Triggered, this, &UPortalComponent::SpawnRightOrange);


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

void UPortalComponent::SpawnLeftBlue()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("Blue Portal Spawn!"));
	UCameraComponent* Cam = Player->FPSCAM;
	FVector StartLocation = Cam->GetComponentLocation();
	FVector Direction = Cam->GetForwardVector();
	//Player->PortalComponent->SpawnPortalAlongVector(StartLocation, Direction, true);
}

void UPortalComponent::SpawnRightOrange()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, TEXT("Orange Portal Spawn!"));
	UCameraComponent* Cam = Player->FPSCAM;
	FVector StartLocation = Cam->GetComponentLocation();
	FVector Direction = Cam->GetForwardVector();
	//Player->PortalComponent->SpawnPortalAlongVector(StartLocation, Direction, false);
}

