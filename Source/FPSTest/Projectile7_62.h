// Copyrighted by XLebeL Studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "Projectile7_62.generated.h"

class USphereComponent;
UCLASS()
class FPSTEST_API AProjectile7_62 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile7_62();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY()
	class AFPSCharacter* AfpsCharacter;
	
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshcomponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	UPROPERTY(VisibleAnywhere,Category = Projectile)
	UMaterialInterface* BulletHoleMaterialInterface;
	
	//Function called when the projectile hits something.
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	void FireInDirection(const FVector& ShootDirection);
	
	UPROPERTY(BlueprintReadWrite)
	float Damage = 20;
	UPROPERTY(BlueprintReadWrite)
	float DamageMultiply = 2.0f;
};
