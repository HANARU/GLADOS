#include "PortalWall.h"	
#include "Portal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

APortalWall::APortalWall()
{
	PortalSizeY = 75.f;
	PortalSizeZ = 124.5f;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(DefaultSceneRoot);
	WallMesh->SetWorldScale3D(FVector(1.0, Wall_Width / 100.f, Wall_Height / 100.f));

	Material_Wall = CreateDefaultSubobject<UMaterialInterface>("Material_Wall");

	
	SetConstructor();
}

void APortalWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float APortalWall::ClampPointToWall(float Point, float WallSize, float PortalRadius)
{
	float Offset = FMath::Clamp(((WallSize / 2) - PortalRadius) - abs(Point), -999999999, 0);
	if (Point < 0.f)
	{
		Offset = Offset;
	}
	else
	{
		Offset = -Offset;
	}
	float ReturnValue = Point - Offset;
	return ReturnValue;
}

FVector2D APortalWall::ConstrainPortalToWall(float PortalY, float PortalZ)
{
	float ClampedY = ClampPointToWall(PortalY, Wall_Width, PortalSizeY);
	float ClampedZ = ClampPointToWall(PortalZ, Wall_Height, PortalSizeZ);
	FVector2D PortalYZ = FVector2D(ClampedY, ClampedZ);
	return PortalYZ;
}


AActor* APortalWall::TryAddPortal(FVector PortalOrigin, bool PortalA)
{
	FVector RelativePortalOrigin = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), PortalOrigin);
	RelativePortalOrigin.Y = ConstrainPortalToWall(RelativePortalOrigin.Y, RelativePortalOrigin.Z).X;
	RelativePortalOrigin.Z = ConstrainPortalToWall(RelativePortalOrigin.Y, RelativePortalOrigin.Z).Y;
	RelativePortalOrigin = FVector(RelativePortalOrigin.X, RelativePortalOrigin.Y, RelativePortalOrigin.Z);

	if (HasRoomforNewPortal(RelativePortalOrigin.Y, RelativePortalOrigin.Z) == true)
	{
		FVector SpawnLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(), RelativePortalOrigin);
		FRotator SpawnRotation = GetActorRotation();
		

		
	}

	return nullptr;
}

bool APortalWall::HasRoomforNewPortal(float NewPortalY, float NewPortalZ)
{
	for (auto Actor : PortalsOnwall)
	{
		FVector Rect3D_1 = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), Actor->GetActorLocation());
		FVector2D Rect2D_1 = FVector2D(Rect3D_1.Y, Rect3D_1.Z);
		FVector2D RectExtents = FVector2D(PortalSizeY, PortalSizeZ);
		FVector2D Rect2D_2 = FVector2D(NewPortalY, NewPortalZ);

		bool Collidea = RectToRectCollision(Rect2D_1, RectExtents, Rect2D_2, RectExtents);
		if (Collidea == true)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}

bool APortalWall::RectToRectCollision(FVector2D Rect1Origin, FVector2D Rect1Extents, FVector2D Rect2Origin, FVector2D Rect2Extents)
{
	bool CollsionCheck;
	if (Rect1Origin.X + Rect1Extents.X < Rect2Origin.X - Rect2Extents.X) { CollsionCheck = false; }

	if (Rect1Origin.X - Rect1Extents.X < Rect2Origin.X + Rect2Extents.X) { CollsionCheck = false; }

	if (Rect1Origin.Y + Rect1Extents.Y < Rect2Origin.Y - Rect2Extents.Y) { CollsionCheck = false; }

	if (Rect1Origin.Y - Rect1Extents.Y < Rect2Origin.Y + Rect2Extents.Y) { CollsionCheck = false; }

	else { CollsionCheck = true; }
	return CollsionCheck;
}

void APortalWall::SetConstructor()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/3_Assets/PortalWall/SM_PortalWall.SM_PortalWall'"));
	if (Mesh.Succeeded())
	{
		WallMesh->SetStaticMesh(Mesh.Object);
	}

	ConstructorHelpers::FClassFinder<APortal> TempPortal(TEXT("/Script/Engine.Blueprint'/Game/2_BP/BP_Portal.BP_Portal_C'"));
	if (TempPortal.Succeeded()) { Portal->StaticClass(); }

	ConstructorHelpers::FObjectFinder<UMaterialInterface> M_Wall(TEXT("/Script/Engine.Material'/Game/3_Assets/PortalWall/M_PortalWall.M_PortalWall'"));
	if(M_Wall.Succeeded()) { WallMesh->SetMaterial(0, Material_Wall); }
}
