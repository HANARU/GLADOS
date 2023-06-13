#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup_PortalGun.generated.h"

UCLASS()
class GLADOS_API APickup_PortalGun : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup_PortalGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* PickupObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereCollision;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
