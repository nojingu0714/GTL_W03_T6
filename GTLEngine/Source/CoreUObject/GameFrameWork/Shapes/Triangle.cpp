#include "pch.h"
#include "Triangle.h"

#include "Components/StaticMeshComponent.h"
#include "Asset/ObjManager.h"

ATriangle::ATriangle()
	: Super()
{
	StaticMeshComponent->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Contents/Triangle.obj")));
}

void ATriangle::Tick(float TickTime)
{
	AActor::Tick(TickTime);
}

void ATriangle::Destroy()
{
	AActor::Destroy();
}
