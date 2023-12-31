#include "SinglePlayer.h"
#include "PortalWall.h"
#include "Engine/World.h"
#include "PortalComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASinglePlayer::ASinglePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsGrabbing = false;

	GetCapsuleComponent()->InitCapsuleSize(30.f, 80.f);
	RootComponent = GetCapsuleComponent();

	FPSCAM = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCAM"));
	FPSCAM->SetupAttachment(GetCapsuleComponent());
	FPSCAM->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FPSCAM->bUsePawnControlRotation = true;

	TraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TracePoint"));
	TraceStartPoint->SetupAttachment(FPSCAM);
	TraceStartPoint->SetRelativeLocation(FVector(50, 0, 0));

	GrabPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrabPoint"));
	GrabPoint->SetupAttachment(FPSCAM);
	GrabPoint->SetRelativeLocation(FVector(130.f, 0.f, 0.f));

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	PlayerMesh->SetOnlyOwnerSee(true);
	PlayerMesh->SetupAttachment(FPSCAM);
	PlayerMesh->bCastDynamicShadow = false;
	PlayerMesh->CastShadow = false;
	PlayerMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

void ASinglePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
			//Subsystem->AddMappingContext(PortalGunMappingContext, 1);
		}
	}
	
}

void ASinglePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASinglePlayer::Move);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASinglePlayer::Look);	

	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ASinglePlayer::Interaction);

	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASinglePlayer::Crouching);
	EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &ASinglePlayer::PauseMenu);



	//EnhancedInputComponent->BindAction(MouseLeftclickAction, ETriggerEvent::Started, this, &ASinglePlayer::SpawnLeftBlue);
	//EnhancedInputComponent->BindAction(MouseRightclickAction, ETriggerEvent::Started, this, &ASinglePlayer::SpawnRightOrange);

}


////////////////////// Basic Movement /////////////////////////


void ASinglePlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASinglePlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(-LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASinglePlayer::Crouching()
{
	bIsCrouching = !(bIsCrouching);

	if (bIsCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
		FPSCAM->SetRelativeLocation(FVector(0,0,30));
		GetMesh()->SetRelativeLocation(FVector(0, 0, -45));
		GetCapsuleComponent()->SetCapsuleSize(30, 40);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		FPSCAM->SetRelativeLocation(FVector(0, 0, 60));
		GetMesh()->SetRelativeLocation(FVector(0, 0, -75));
		GetCapsuleComponent()->SetCapsuleSize(30 ,80);
	}
}

////////////////////// Interaction /////////////////////////

void ASinglePlayer::PickupGunPure()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("Overlap from Player"));
	bDestroyedPortalComponent = true;
	FString BoolCheck = UKismetStringLibrary::Conv_BoolToString(bDestroyedPortalComponent);
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, BoolCheck);
}

void ASinglePlayer::Interaction()
{
	FVector StartLocation;
	FHitResult HitResult;
	FVector CompLocation;

	//StartLocation = FPSCAM->GetComponentLocation() + (FPSCAM->GetForwardVector());
	StartLocation = TraceStartPoint->GetComponentLocation();
	GetController()->GetPlayerViewPoint(StartLocation, Direction);
	FVector EndLocation = StartLocation + (Direction.Vector() * 10000.f);

	FCollisionQueryParams QueryParams;

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, UKismetStringLibrary::Conv_VectorToString(FPSCAM->GetComponentLocation()));
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, UKismetStringLibrary::Conv_VectorToString(TraceStartPoint->GetComponentLocation()));

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel5, QueryParams);

	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 10.f);

	GrabbableComp = HitResult.GetComponent();
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, UKismetStringLibrary::Conv_ObjectToString(GrabbableComp));

	bool PhysicsCheck = GrabbableComp->IsAnySimulatingPhysics();

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, UKismetStringLibrary::Conv_BoolToString(PhysicsCheck));

	if (bIsGrabbing == false && PhysicsCheck == true)
	{
		bIsGrabbing = !bIsGrabbing;

		CompLocation = GrabbableComp->GetComponentLocation();
		FString CompLocationString = UKismetStringLibrary::Conv_VectorToString(CompLocation);

		/*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, UKismetStringLibrary::Conv_ObjectToString(GrabbableComp));*/

		PhysicsHandleComp->GrabComponentAtLocation(GrabbableComp, TEXT("None"), CompLocation);
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, CompLocationString);

	}

	else if (IsValid(GrabbableComp))
	{
		PhysicsHandleComp->ReleaseComponent();
		bIsGrabbing = !bIsGrabbing;
	}

}

void ASinglePlayer::PauseMenu()
{
	bIsPause = !bIsPause;
}

void ASinglePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GrabbableComp))
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 0.002, FColor::Black, UKismetStringLibrary::Conv_ObjectToString(GrabbableComp));*/
		PhysicsHandleComp->SetTargetLocationAndRotation(GrabPoint->GetComponentLocation(), Direction);
	}

}






/*if (bIsGrabbing == false)
{
	bIsGrabbing = !bIsGrabbing;

	GrabbableComp = HitResult.GetComponent();

	CompLocation = GrabbableComp->GetComponentLocation();
	FString CompLocationString = UKismetStringLibrary::Conv_VectorToString(CompLocation);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, UKismetStringLibrary::Conv_ObjectToString(GrabbableComp));

	PhysicsHandleComp->GrabComponentAtLocation(GrabbableComp, TEXT("None"), CompLocation);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, CompLocationString);
}*/

//else if(IsValid(GrabbableComp))
//{
//	PhysicsHandleComp->ReleaseComponent();
//	//GrabbableComp = nullptr;
//	
//	bIsGrabbing = !bIsGrabbing;
//}



////////////////////// Portal Function /////////////////////////

//void ASinglePlayer::SpawnPortalAlongVector(FVector StartLocation, FVector Direction, bool PortalA)
//{
//	FHitResult HitResult;
//	FVector EndLocation = (Direction * MaxSpawnDistance) + StartLocation;
//
//	FCollisionObjectQueryParams ObjectQueryParams(FCollisionObjectQueryParams::AllObjects);
//	FCollisionQueryParams QueryParams;
//
//	QueryParams.AddIgnoredActor(this);
//
//	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel11);
//	
//	if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectQueryParams, QueryParams))
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, TEXT("Spawn Activated"));
//		UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, 5.f, 1.f);
//		if (APortalWall* NewPortalWall = Cast<APortalWall>(HitResult.GetActor()))
//		{
//			FVector TraceLine = (HitResult.TraceStart - HitResult.TraceEnd);
//			FVector TempPortalOrigin = HitResult.Location + TraceLine.Normalize(0.0001);
//			NewPortalWall->TryAddPortal(TempPortalOrigin, PortalA);
//		}
//	}
//}

//void ASinglePlayer::SpawnLeftBlue()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("Blue Portal Spawn!"));
//	UCameraComponent* Cam = FPSCAM;
//	FVector StartLocation = Cam->GetComponentLocation();
//	FVector Direction = Cam->GetForwardVector();
//	SpawnPortalAlongVector(StartLocation, Direction, true);
//}

//void ASinglePlayer::SpawnRightOrange()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, TEXT("Orange Portal Spawn!"));
//	UCameraComponent* Cam = FPSCAM;
//	FVector StartLocation = Cam->GetComponentLocation();
//	FVector Direction = Cam->GetForwardVector();
//	SpawnPortalAlongVector(StartLocation, Direction, false);
//}