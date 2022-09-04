// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "cuda_runtime.h"
#include "CUDA_Volume_Rendering.h"
#include <iostream>
#include <fstream>
#include <string>
#include "GameFramework/Actor.h"
#include "volume_renderer.generated.h"

UCLASS()
class CUDA_RENDERING_API Avolume_renderer : public AActor
{
	GENERATED_BODY()

	std::string error_massage;
	unsigned char* outTexture;
	unsigned char* volumeSource;
public:	
	// Sets default values for this actor's properties
	Avolume_renderer();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "CUDA")
	void readBighaed();

	UFUNCTION(BlueprintCallable, Category = "CUDA")
	UTexture2D* CUDA_Volume_Rendering_Init(USceneComponent* cameraTransform, FVector2D tableRange, FVector aColor, FVector bColor);

	UFUNCTION(BlueprintCallable, Category = "CUDA")
	UTexture2D* CUDA_Volume_Rendering_Tick(USceneComponent* cameraTransform, FVector2D tableRange, FVector aColor, FVector bColor);
};
