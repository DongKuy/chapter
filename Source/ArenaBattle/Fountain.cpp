// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"

// Sets default values
AFountain::AFountain() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 객체의 컴포넌트 생성 
	body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));
	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Splash"));
	movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Movement"));

	// 루트 컴포턴트의 자식 설정
	RootComponent = body;
	water->SetupAttachment(body);
	light->SetupAttachment(body);
	splash->SetupAttachment(body);

	// 객체 세부 설정
	water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	// 메쉬객체에 설정한 경로의 매쉬를 지정
	// COMMENT : 애셋의 경우 게임 실핼 중 경로가 변경될 일이 없기 때문에 static으로 선언하여 처음 한번만 초기화 되는 것이 바람직
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SM_SPLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01"));
	
	if (SM_BODY.Succeeded()) {
		body->SetStaticMesh(SM_BODY.Object);
	}
	if (SM_WATER.Succeeded()) {
		water->SetStaticMesh(SM_WATER.Object);
	}
	if (SM_SPLASH.Succeeded()) {
		splash->SetTemplate(SM_SPLASH.Object);
	}

	RotateSpeed = 30.0f;
	movement->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);
}

// Called when the game starts or when spawned
void AFountain::BeginPlay() {
	Super::BeginPlay();
	
	ABLOG_S(Warning);
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), id, GetActorLocation().X);
	//UE_LOG(ArenaBattle, Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), id, GetActorLocation().X);
}

void AFountain::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	ABLOG_S(Warning);
}

void AFountain::PostInitializeComponents() {
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// Called every frame
void AFountain::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//AddActorLocalRotation(FRotator(0.0f, RotateSpeed * DeltaTime, 0.0f));
}