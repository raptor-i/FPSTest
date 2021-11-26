// Copyrighted by XLebeL Studio. All rights reserved.


#include "Projectile7_62.h"

#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectile7_62::AProjectile7_62()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if(!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->SetRelativeScale3D(FVector(0.12f,0.12f,0.12f));
		CollisionComponent->InitSphereRadius(15.f);

		RootComponent = CollisionComponent;
	}
	if(!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 120000.f;
		ProjectileMovementComponent->MaxSpeed = 120000.f;
		ProjectileMovementComponent->bRotationFollowsVelocity =true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	}
	if(!ProjectileMeshcomponent)
	{
		ProjectileMeshcomponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Models/Projectile/Sphere.Sphere'"));

		if(Mesh.Succeeded())
			ProjectileMeshcomponent->SetStaticMesh(Mesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> Material(
			TEXT("'/Game/Models/Projectile/SphereMaterial.SphereMaterial'"));

		if(Material.Succeeded())
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshcomponent);

		ProjectileMeshcomponent->SetMaterial(0, ProjectileMaterialInstance);
		ProjectileMeshcomponent->SetRelativeScale3D(FVector(0.12f,0.12f,0.12f));
		ProjectileMeshcomponent->SetupAttachment(RootComponent);
	}
	//Destroy the projectile after 3 seconds.
	InitialLifeSpan = 3.f;

	//sets collison component profile name projectile.
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	//Calls function when component hits something.
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile7_62::OnHit);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>BulletMaterialInt(TEXT("'/Game/Models/T_bullet_hole_Mat.T_bullet_hole_Mat'"));
	if(BulletMaterialInt.Succeeded())
		BulletHoleMaterialInterface = BulletMaterialInt.Object;

	
}

// Called when the game starts or when spawned
void AProjectile7_62::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile7_62::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AProjectile7_62::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* Actor = GetWorld()->GetFirstPlayerController()->GetPawn();

	FHitResult HitResult;

	if(OtherActor != nullptr)
	{
		if(OtherActor->ActorHasTag("Enemy_1"))
		{
			FPointDamageEvent DamageEvent(Damage, Hit,NormalImpulse, nullptr);
			GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Black,FString::Printf(TEXT("Hitted Enemy with Ak")));
			
			OtherActor->TakeDamage(Damage*DamageMultiply, DamageEvent, OtherActor->GetInstigatorController(), this);
		}	
		UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),BulletHoleMaterialInterface , FVector(3.f, 3.f, 3.f),
											Hit.Location, Hit.Location.Rotation(),5.0f);
		
		DecalComponent->AttachToComponent(OtherComponent, FAttachmentTransformRules::KeepWorldTransform);
		DrawDebugDirectionalArrow(GetWorld(), Actor->GetActorLocation(), Hit.ImpactPoint, 10.f,
			FColor::Green, false, 5.f);
		
	}

	
	if(OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.f, Hit.ImpactPoint);
	}

	Destroy();
}

void AProjectile7_62::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
	
}

