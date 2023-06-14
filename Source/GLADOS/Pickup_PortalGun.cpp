#include "Pickup_PortalGun.h"
#include "SinglePlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

APickup_PortalGun::APickup_PortalGun()
{
	PickupObject = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupObject;

	TestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestOBJ"));
	TestMesh->SetupAttachment(PickupObject);


	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(PickupObject);
	SphereCollision->SetSphereRadius(100.f);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickup_PortalGun::OnOverlapBegin);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		TestMesh->SetStaticMesh(TempMesh.Object);
	}
}

void APickup_PortalGun::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		ASinglePlayer* Player = Cast<ASinglePlayer>(OtherActor);
		if (Player)
		{
			Player->PickupGunPure();
			Destroy();
		}
	}
}

void APickup_PortalGun::PickupGunPure()
{
}


