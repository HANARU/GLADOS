#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "PortalWall.generated.h"

class UStaticMeshComponent;

UCLASS()
class GLADOS_API APortalWall : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalWall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Wall_Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Wall_Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> PortalsOnwall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> NeedSpawnActor;

	float PortalSizeY, PortalSizeZ;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float ClampPointToWall(float Point, float WallSize, float PortalRadius);
	UFUNCTION(BlueprintCallable)
	FVector2D ConstrainPortalToWall(float PortalY, float PortalZ);
	UFUNCTION(BlueprintCallable)
	AActor* TryAddPortal(FVector PortalOrigin, bool PortalA);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasRoomforNewPortal(float NewPortalY, float NewPortalZ);

	bool RectToRectCollision(FVector2D Rect1Origin, FVector2D Rect1Extents, FVector2D Rect2Origin, FVector2D Rect2Extents);




};
