#include "pch.h"
#include "Sphere.h"

#include "Components/StaticMeshComponent.h"
#include "Asset/ObjManager.h"

ASphere::ASphere()
	: Super()
{
	StaticMeshComponent->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Contents/Sphere.obj")));

}

void ASphere::Tick(float TickTime)
{
	AActor::Tick(TickTime);

	SetActorRotation(GetActorRotation().Add(0.f, 0.f, 0.1f));
}

void ASphere::Destroy()
{
	AActor::Destroy();
}
