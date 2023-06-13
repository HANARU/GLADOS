#pragma once

#include "CoreMinimal.h"
#include "PortalWall.h"
#include "Engine/World.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PortalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLADOS_API UPortalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPortalComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpawnDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASinglePlayer* Player;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* PortalGunMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MouseLeftclickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MouseRightclickAction;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

public:
	void SpawnPortalAlongVector(FVector StartLocation, FVector Direction, bool PortalA);

	void SpawnLeftBlue();
	void SpawnRightOrange();
};
