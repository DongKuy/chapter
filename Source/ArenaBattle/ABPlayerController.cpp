// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"


void AABPlayerController::PostInitializeComponents() {
	ABLOG(Warning, TEXT("PostInitializeComponents Begin"));
	Super::PostInitializeComponents();
	ABLOG(Warning, TEXT("PostInitializeComponents End"));
}

void AABPlayerController::OnPossess(APawn * aPawn) {
	ABLOG(Warning, TEXT("OnPossess Begin"));
	Super::OnPossess(aPawn);
	ABLOG(Warning, TEXT("OnPossess End"));
}
