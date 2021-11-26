//Copyrighted by XLebeL Studio. All rights reserved.


#include "FPSCharacter.h"

#include <activation.h>
#include <filesystem>
#include <ThirdParty/PhysX3/PxShared/src/foundation/include/PsBasicTemplates.h>

#include "CollisionDebugDrawingPublic.h"
#include "DrawDebugHelpers.h"
#include "SkeletalRenderPublic.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "FPSProjectiles.h"
#include "Projectile7_62.h"
#include "GenericPlatform/GenericPlatformChunkInstall.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates FPS camera component
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	check(FPSCameraComponent != nullptr);

	//Attaches the camera component to the capsule component
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent,UCapsuleComponent>(GetCapsuleComponent()));

	//Posits the camera to the little bit above from the eyes 
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f,0.0f,50.f+BaseEyeHeight));

	//enables pawn control rotation
	FPSCameraComponent->bUsePawnControlRotation = true;

	//Creates FPS mesh component
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	check(FPSMesh != nullptr);

	//sets only owner sees the mesh
	FPSMesh->SetOnlyOwnerSee(true);

	//Attaches to the FPS camera component
	FPSMesh->SetupAttachment(FPSCameraComponent);

	//Disables some shadows due to possible view illusions.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);

	FPSWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SecondaryTest"));
	check(FPSWeapon != nullptr);
	FPSWeapon->AttachToComponent(FPSMesh,FAttachmentTransformRules::KeepRelativeTransform, TEXT("R_GunSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SecondaryMesh(
		TEXT("'/Game/FPS_Assault_Pack/Meshes/RU74/SK_RU74.SK_RU74'"));
	
	if (SecondaryMesh.Succeeded())
		FPSWeapon->SetSkeletalMesh(SecondaryMesh.Object);

	FPSWeapon->CastShadow = false;
	FPSWeapon->SetRelativeLocation(FVector(0.f,0.f,-3.f));
	FPSWeapon->SetRelativeRotation(FRotator(0.f,0.f,0.f));
	FPSWeapon->SetRelativeScale3D(FVector(1.5f,1.5f,1.5f));

	FPSWeaponSecondary = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSWeaponSecondary"));
	check(FPSWeaponSecondary != nullptr);
	FPSWeaponSecondary->AttachToComponent(FPSMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("R_GunSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FpsWeaponSec(
		TEXT("'/Game/FPS_Assault_Pack/Meshes/Rifle/SK_Rifle_NoAddons.SK_Rifle_NoAddons'"));
	if(FpsWeaponSec.Succeeded())
		FPSWeaponSecondary->SetSkeletalMesh(FpsWeaponSec.Object);
	FPSWeaponSecondary->CastShadow = false;
	FPSWeaponSecondary->SetRelativeLocation(FVector(0.f,0.f,-2.f));
	FPSWeaponSecondary->SetRelativeRotation(FRotator(0.f,0.f,0.f));
	FPSWeaponSecondary->SetRelativeScale3D(FVector(1.5f,1.5f,1.5f));

	Health = DefaultHealth;

	static ConstructorHelpers::FObjectFinder<USoundCue> PainCue(
		TEXT("'/Game/Sounds/C_PainSound.C_PainSound'"));
	if(PainCue.Succeeded())
		SoundCue = PainCue.Object;
	
}
// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this,&AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&AFPSCharacter::StopJump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSCharacter::StopCrouch);
}

void AFPSCharacter::MoveForward(float value)
{
	//finds the X axis of the character
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	//AddMovementInput(Direction,value);
	AddMovementInput(GetActorForwardVector()*value);
}

void AFPSCharacter::MoveRight(float value)
{
	//finds the Y axis of the character
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
	
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::ToggleCrouch()
{
	AFPSCharacter::Crouch();
}

void AFPSCharacter::StopCrouch()
{
	ACharacter::UnCrouch();
}

void AFPSCharacter::GunRecoilSystem()
{
	
	if(!bIsHidden)
	{
		APawn::AddControllerPitchInput(FMath::Lerp(0.f,-0.60f,1));
		APawn::AddControllerYawInput(FMath::Lerp(0.f,FMath::RandRange(-0.40f,0.40f),1));
	}
	else
	{
		APawn::AddControllerPitchInput(FMath::Lerp(0.f,-0.20f,1));
		APawn::AddControllerYawInput(FMath::Lerp(0.f,FMath::RandRange(-0.25f,0.25f),1));
	}
	
}

void AFPSCharacter::Fire()
{

	if(!bIsChangingCPP && ProjectileClass)
	{
		//Gets ActorEyesViewPoint 
		FVector CameraLocation;
		FRotator CameraRotator;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CameraLocation,CameraRotator);
	
		//Spawn muzzle
		MuzzleOffset.Set(50.f,0,0);

		FVector MuzzleLocation = CameraLocation + CameraRotator.Vector();
		FRotator MuzzleRotation = CameraRotator;

			
		UWorld* World = GetWorld();
		if (World)
		{
			bIsFiring = true;
			if(bIsHidden)
			{
				FActorSpawnParameters SpawnParams;
				AFPSProjectiles* Projectile = World->SpawnActor<AFPSProjectiles>(
					ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

				if(Projectile)
				{
					Projectile->SetActorHiddenInGame(false);
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
					MuzzleFlash();
					GunRecoilSystem();
				
				}
			}
			else
			{
				FActorSpawnParameters SpawnParams;
				AProjectile7_62* Projectile7_62 = World->SpawnActor<AProjectile7_62>(
					ProjectileClass7_62, MuzzleLocation, MuzzleRotation, SpawnParams);

				if(Projectile7_62)
				{
					Projectile7_62->SetActorHiddenInGame(false);
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile7_62->FireInDirection(LaunchDirection);
					MuzzleFlash();
					GunRecoilSystem();
				}
			}
		}
	}
}


void AFPSCharacter::FireStop()
{
	bIsFiring = false;
}

void AFPSCharacter::MuzzleFlash()
{
	if(FPSWeapon->SkeletalMesh->GetName().Compare("SK_RU74"))
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystemAk, FPSWeapon,TEXT("MuzzleSocket"));
	else
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystemRifle, FPSWeapon,TEXT("MuzzleSocket"));

}

void AFPSCharacter::FPSPawn()
{
	return;
}

float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Health -= DamageAmount;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("Current Healt %f"),Health));
	if(Health <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You Died")));
	}
	float Pitcher = FMath::FRandRange(0.75f,1.f);
	
    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundCue, .5f, Pitcher, 0.f);
	return DamageAmount;
}


