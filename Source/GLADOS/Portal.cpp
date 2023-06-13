#include "Portal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"

APortal::APortal()
{
	DefaultSceneRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(DefaultSceneRoot);
	PortalMesh->SetRelativeRotation(FRotator(-90, 0, 0));
	

	PortalBorderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalBorderMesh"));
	PortalBorderMesh->SetupAttachment(DefaultSceneRoot);
	PortalBorderMesh->SetRelativeRotation(FRotator(-90, 0, 0));

	BackFacingScene = CreateDefaultSubobject<USceneComponent>(TEXT("BackFacingScene"));
	BackFacingScene->SetupAttachment(DefaultSceneRoot);
	BackFacingScene->SetRelativeRotation(FRotator(0, -180, 0));

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(DefaultSceneRoot);

	SetMeshes();
}

void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::LinkPortals(APortal Portal)
{
	LinkedPortal = &Portal;
	if (IsValid(LinkedPortal))
	{
		// 포탈 내 RenderTarget 추적
	}

	else
	{
		// 포탈 없는 Material 생성
	}
}

void APortal::SetMeshes()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> BorderMesh(TEXT("/Script/Engine.StaticMesh'/Game/3_Assets/Portal/Mesh/SM_PortalBorder.SM_PortalBorder'"));
	if (BorderMesh.Succeeded()) { PortalBorderMesh->SetStaticMesh(BorderMesh.Object); }

	ConstructorHelpers::FObjectFinder<UStaticMesh> MainMesh(TEXT("/Script/Engine.StaticMesh'/Game/3_Assets/Portal/Mesh/SM_Portal.SM_Portal'"));
	if (MainMesh.Succeeded()) { PortalMesh->SetStaticMesh(MainMesh.Object); }


}

