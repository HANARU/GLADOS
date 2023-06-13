#include "SinglePlayer.h"
#include "Engine/World.h"
#include "PortalComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
	
}

void ASinglePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASinglePlayer::Move);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASinglePlayer::Look);	

	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ASinglePlayer::Interaction);

}

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

void ASinglePlayer::Interaction()
{
	FVector StartLocation;
	FRotator Direction;
	FHitResult HitResult;
	FVector CompLocation;

	StartLocation = FPSCAM->GetComponentLocation();
	GetController()->GetPlayerViewPoint(StartLocation, Direction);
	FVector EndLocation = StartLocation + (Direction.Vector() * 10000.f);

	FCollisionQueryParams QueryParams;

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 2.f);

	if (bIsGrabbing == false)
	{
		bIsGrabbing = !bIsGrabbing;

		GrabbableComp = HitResult.GetComponent();

		CompLocation = GrabbableComp->GetComponentLocation();

		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, UKismetStringLibrary::Conv_ObjectToString(GrabbableComp));
			
		PhysicsHandleComp->GrabComponentAtLocation(GrabbableComp, TEXT("None"), CompLocation);
	}

	else if(IsValid(GrabbableComp))
	{
		PhysicsHandleComp->ReleaseComponent();
		//GrabbableComp = nullptr;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, UKismetStringLibrary::Conv_ObjectToString(GrabbableComp));
		bIsGrabbing = !bIsGrabbing;
	}

}

void ASinglePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GrabbableComp))
	{
		PhysicsHandleComp->SetTargetLocationAndRotation(GrabPoint->GetComponentLocation(),GetControlRotation());
	}

}
