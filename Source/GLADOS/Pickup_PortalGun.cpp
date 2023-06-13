#include "Pickup_PortalGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

APickup_PortalGun::APickup_PortalGun()
{
	PickupObject = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupObject;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(PickupObject);
	SphereCollision->SetSphereRadius(32.f);

	//ConstructorHelpers::FObjectFinder<USkeletalMesh> Gun(TEXT(""));
	//if(Gun.Succeed()) { PickupObject->SetSkeletalMesh(Gun.Object);}
}

void APickup_PortalGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickup_PortalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

