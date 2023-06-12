#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class USceneCaptureComponent2D;

UCLASS(config = Game)
class GLADOS_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PortalBorderMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PortalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* BackFacingScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCapture;



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
