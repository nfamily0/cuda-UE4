// Fill out your copyright notice in the Description page of Project Settings.


#include "volume_renderer.h"

// Sets default values
Avolume_renderer::Avolume_renderer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void Avolume_renderer::BeginDestroy()
{
	Super::BeginDestroy();
	
	free();
	UE_LOG(LogTemp, Warning, TEXT("destory"));
	delete[] outTexture;
	delete[] volumeSource;
}


// Called when the game starts or when spawned
void Avolume_renderer::BeginPlay()
{
	Super::BeginPlay();

	readBighaed();

	auto texture = CUDA_Volume_Rendering_Init(
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetTransformComponent(),
		FVector2D(100, 140),
		FVector(0.6, 0.6, 0.6),
		FVector(1.0, 1.0, 1.0)
	);
	
	TArray<UStaticMeshComponent*> comps;
	GetComponents<UStaticMeshComponent>(comps);
	UStaticMeshComponent* plane = nullptr;
	for(int i = 0; i <comps.Num(); ++i)
	{
		if(comps[i]->GetName() == FString("Plane"))
		{
			plane = comps[i];
			break;
		}
	}
	
	auto dmi = plane->CreateDynamicMaterialInstance(0);
	dmi->SetTextureParameterValue("volumeTexture", texture);
}

// Called every frame
void Avolume_renderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//시간의 따른 Transfer function 변경으로 색상 애니메이션 효과 추가
	static float accDT = 0;
	accDT += DeltaTime;
	
	FVector aColor = { 0.6,0.6,0.6 };
	FVector bColor = { 1.0,1.0,1.0 };
	aColor.X = __max(0.3, aColor.X * sin(accDT) * 1);
	aColor.Y = __max(0.3, aColor.Y * sin(accDT) * 2);
	aColor.Z = __max(0.3, aColor.Z * cos(accDT) * 1.5);
	
	bColor.X = __max(0.3, bColor.X * sin(accDT) * 1.5);
	bColor.Y = __max(0.3, bColor.Y * cos(accDT) * 2);
	bColor.Z = __max(0.3, bColor.Z * cos(accDT) * 1);
	
	auto texture = CUDA_Volume_Rendering_Tick(
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetTransformComponent(),
		FVector2D(100, 140),
		aColor,
		bColor
	);
	
	TArray<UStaticMeshComponent*> comps;
	GetComponents<UStaticMeshComponent>(comps);
	UStaticMeshComponent* plane = nullptr;
	for (int i = 0; i < comps.Num(); ++i)
	{
		if (comps[i]->GetName() == FString("Plane"))
		{
			plane = comps[i];
			break;
		}
	}
	
	static auto dmi = plane->CreateDynamicMaterialInstance(0);
	dmi->SetTextureParameterValue("volumeTexture", texture);
}

void Avolume_renderer::readBighaed()
{
	outTexture = new unsigned char[256 * 256 * 4];
	volumeSource = new unsigned char[256 * 256 * 225];

	std::ifstream myfile;
	myfile.open(ToCStr(FPaths::ProjectContentDir() + "Bighead.den"), std::ios::in | std::ios::binary);
	if (!myfile.is_open()) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("read fail"));
	}

	myfile.read((char*)volumeSource, 256 * 256 * 225);
	myfile.close();
}

UTexture2D* Avolume_renderer::CUDA_Volume_Rendering_Init(USceneComponent* cameraTransform, FVector2D tableRange, FVector aColor, FVector bColor)
{
	FVector feye = cameraTransform->GetRelativeLocation();
	FVector fdir = FVector(128.0, 128.0, 128.0) - feye;
	FVector fup = cameraTransform->GetUpVector();
	FVector fcross;

	fdir.Normalize();
	fcross = FVector::CrossProduct(fup, fdir);
	fup = FVector::CrossProduct(fcross, fdir);

	float eye[3] = { feye.X, feye.Y, feye.Z };
	float dir[3] = { fdir.X, fdir.Y, fdir.Z };
	float up[3] = { fup.X, fup.Y, fup.Z };
	float cross[3] = { fcross.X, fcross.Y, fcross.Z };

	int table_range[2] = { tableRange.X, tableRange.Y };
	float a_Color[3] = { aColor.X, aColor.Y, aColor.Z };
	float b_Color[3] = { bColor.X, bColor.Y, bColor.Z };

	init(volumeSource, table_range, a_Color, b_Color, eye, dir, cross, up, &error_massage);

	volumeRender(outTexture, &error_massage);


	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, error_massage.c_str());

	auto texture = UTexture2D::CreateTransient(256, 256, PF_R8G8B8A8);

	void* textureData = texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

	FMemory::Memcpy(textureData, outTexture, sizeof(unsigned char) * 256 * 256 * 4);
	texture->PlatformData->Mips[0].BulkData.Unlock();
	texture->UpdateResource();

	return texture;
}

UTexture2D* Avolume_renderer::CUDA_Volume_Rendering_Tick(USceneComponent* cameraTransform, FVector2D tableRange, FVector aColor, FVector bColor)
{
	FVector feye = cameraTransform->GetRelativeLocation();
	FVector fdir = FVector(128.0, 128.0, 128.0) - feye;
	FVector fup = cameraTransform->GetUpVector();
	FVector fcross;

	fdir.Normalize();
	fcross = FVector::CrossProduct(fup, fdir);
	fup = FVector::CrossProduct(fcross, fdir);

	float eye[3] = { feye.X, feye.Y, feye.Z };
	float dir[3] = { fdir.X, fdir.Y, fdir.Z };
	float up[3] = { fup.X, fup.Y, fup.Z };
	float cross[3] = { fcross.X, fcross.Y, fcross.Z };

	int table_range[2] = { tableRange.X, tableRange.Y };
	float a_Color[3] = { aColor.X, aColor.Y, aColor.Z };
	float b_Color[3] = { bColor.X, bColor.Y, bColor.Z };

	tick(outTexture, table_range, a_Color, b_Color, eye, dir, cross, up, &error_massage);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, strT.c_str());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, error_massage.c_str());

	auto texture = UTexture2D::CreateTransient(256, 256, PF_R8G8B8A8);

	void* textureData = texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

	FMemory::Memcpy(textureData, outTexture, sizeof(unsigned char) * 256 * 256 * 4);
	texture->PlatformData->Mips[0].BulkData.Unlock();
	texture->UpdateResource();

	return texture;
}