#include "pch.h"
#include "Cone.h"

#include "Components/StaticMeshComponent.h"
#include "Asset/ObjManager.h"

ACone::ACone()
	: Super()
{
	StaticMeshComponent->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Contents/Triangle.obj")));

}

void ACone::Tick(float TickTime)
{
	AActor::Tick(TickTime);
}

void ACone::Destroy()
{
	AActor::Destroy();
}