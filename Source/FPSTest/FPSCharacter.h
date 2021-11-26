//Copyrighted by XLebeL Studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSCharacter.generated.h"


UCLASS()
class FPSTEST_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Handles moving forward
	UFUNCTION()
	void MoveForward(float value);
	
	//handles moving right
	UFUNCTION()
	void MoveRight(float value);

	//Jump things
	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION(BlueprintCallable)
	void ToggleCrouch();

	UFUNCTION()
	void StopCrouch();
	//FPS Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* FPSMesh;


	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* FPSWeapon;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* FPSWeaponSecondary;
	
	//Gets muzzleoffset from camera location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsHidden; //if 0 it is Ak, else rifle

	
	//Spawns Projectile class
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectiles> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile7_62> ProjectileClass7_62;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadWrite)
	float Health = 0;
	
	UPROPERTY(BlueprintReadWrite)
	float DefaultHealth = 100;
	
	UFUNCTION(BlueprintCallable)
	void GunRecoilSystem();
	
	//Handles Firing
	UFUNCTION(BlueprintCallable)
	void Fire();
	
	UFUNCTION(BlueprintCallable)
	void FireStop();

	void MuzzleFlash();

	UFUNCTION()
	void FPSPawn();
	//Sets muzzle flash emitter for Rifle when fire
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFlashParticleSystemRifle;

	//Sets muzzle flash emitter for AK47 when fire
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFlashParticleSystemAk;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsFiring = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsChangingCPP;

};
