#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EventInterface.h"
#include "SinglePlayer.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config = Game)
class GLADOS_API ASinglePlayer : public ACharacter, public IEventInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPSCAM;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scene, meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrabPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* PlayerMappingContext;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputMappingContext* PortalGunMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractionAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputAction* MouseLeftclickAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputAction* MouseRightclickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UPrimitiveComponent* GrabbableComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	class UPhysicsHandleComponent* PhysicsHandleComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	bool bIsGrabbing = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	bool bIsCrouching;

	bool bIsHit;
	bool bDestroyedPortalComponent = false;

	float MaxSpawnDistance = 10000.f;

	FRotator Direction;

	

	ASinglePlayer();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Interaction();

	void Crouching();

	void PauseMenu();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsPause = false;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PickupGunPure() override;

	//void SpawnPortalAlongVector(FVector StartLocation, FVector Direction, bool PortalA);

	//void SpawnLeftBlue();

	//void SpawnRightOrange();

};
