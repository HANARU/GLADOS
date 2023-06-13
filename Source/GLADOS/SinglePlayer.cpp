#include "SinglePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"

ASinglePlayer::ASinglePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);
	RootComponent = GetCapsuleComponent();

	FPSCAM = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCAM"));
	FPSCAM->SetupAttachment(GetCapsuleComponent());
	FPSCAM->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FPSCAM->bUsePawnControlRotation = true;

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	PlayerMesh->SetOnlyOwnerSee(true);
	PlayerMesh->SetupAttachment(FPSCAM);
	PlayerMesh->bCastDynamicShadow = false;
	PlayerMesh->CastShadow = false;
	PlayerMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ASinglePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
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

	// EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ASinglePlayer::Interaction);

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

/*void ASinglePlayer::Interaction(const FInputActionValue& Value)
{
	bool Interact = Value.Get<bool>();

	if (Controller != nullptr)
	{
		FHitResult HitResult;

		FVector Start = CameraComponent->GetWorldLocation();
		FVector End((GetForwaordVector * MaxInteractDistance) + GetWorldLocation.FistPersonCamera);
		ECollisionChannel 
	}
}*/

void ASinglePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
