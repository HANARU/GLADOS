#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventInterface.h"
#include "Pickup_PortalGun.generated.h"

UCLASS()
class GLADOS_API APickup_PortalGun : public AActor, public IEventInterface
{
	GENERATED_BODY()
	
public:	
	APickup_PortalGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* PickupObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* TestMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereCollision;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PickupGunPure();

};
