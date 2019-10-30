// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"

// Sets default values
AABCharacter::AABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	springArm->TargetArmLength = 400.0f;
	springArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ram.SK_CharM_Ram"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>WARRIOR_ANIM(TEXT("/Game/Book/Animation/WarriorAnim.WarriorAnim_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	isAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	//GTA
	/*springArm->TargetArmLength = 450.f;
	springArm->SetRelativeRotation(FRotator::ZeroRotator);
	springArm->bUsePawnControlRotation = true;
	springArm->bInheritPitch = true;
	springArm->bInheritRoll = true;
	springArm->bInheritYaw = true;
	springArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 90.0f);*/

	//디아블로
	springArm->TargetArmLength = 800.0f;
	springArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	springArm->bUsePawnControlRotation = false;
	springArm->bInheritPitch = false;
	springArm->bInheritRoll = false;
	springArm->bInheritYaw = false;
	springArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 800.0f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//디아블로
	if (DirectionToMove.SizeSquared() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		AddMovementInput(DirectionToMove);
	}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::ONAttackMontageEnded);
	ABAnim->OnNextAttackCheck.AddLambda([this]()->void
	{
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);

}

void AABCharacter::UpDown(float newAxisvalue)
{
	//GTA
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), newAxisvalue);

	//디아블로
	DirectionToMove.X = newAxisvalue;
}

void AABCharacter::LeftRight(float newAxisvalue)
{
	//GTA
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), newAxisvalue);

	//디아블로
	DirectionToMove.Y = newAxisvalue;
}

void AABCharacter::Turn(float newAxisvalue)
{
	//GTA
	//AddControllerYawInput(newAxisvalue);
}

void AABCharacter::LookUp(float newAxisvalue)
{
	//GTA
	//AddControllerPitchInput(newAxisvalue);
}

void AABCharacter::Attack()
{
	if (isAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
		return;
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMotahe();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		isAttacking = true;
	}
	auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance)
	{
		return;
	}
	AnimInstance->PlayAttackMotahe();
	isAttacking = true;
}

void AABCharacter::ONAttackMontageEnded(UAnimMontage * montage, bool bInterrupted)
{
	ABCHECK(isAttacking);
	ABCHECK(CurrentCombo > 0);
	isAttacking = false;
	AttackEndComboState();
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

