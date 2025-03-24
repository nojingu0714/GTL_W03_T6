#include "pch.h"
#include "Cylinder.h"

#include "Components/StaticMeshComponent.h"
#include "Asset/ObjManager.h"

ACylinder::ACylinder()
	: Super()
{
	StaticMeshComponent->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Contents/Cylinder.obj")));
}

void ACylinder::Tick(float TickTime)
{
	AActor::Tick(TickTime);
}

void ACylinder::Destroy()
{
	AActor::Destroy();
}