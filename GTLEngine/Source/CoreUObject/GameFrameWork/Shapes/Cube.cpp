#include "pch.h"
#include "Cube.h"

#include "Components/StaticMeshComponent.h"
#include "Asset/ObjManager.h"


ACube::ACube()
	: Super()
{
	StaticMeshComponent->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Contents/cube.obj")));
}

void ACube::Tick(float TickTime)
{
	AActor::Tick(TickTime);

	FRotator Rotation = GetActorRotation();
	SetActorLocation(GetActorLocation() + FVector(0.0f, 0.01f, 0.0f));
	SetActorRotation(Rotation + FRotator(1.0f, 0.0f, 0.0f));
	SetActorLocation(GetActorLocation()  + FVector(0.0f, 0.01f, 0.0f));
	

}

void ACube::Destroy()
{
	AActor::Destroy();
}
