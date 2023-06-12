#pragma once

#include "CoreMinimal.h"
#include "PortalWall.h"
#include "Engine/World.h"
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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SpawnPortalAlongVector(FVector StartLocation, FVector Direction, bool PortalA);

		
};
