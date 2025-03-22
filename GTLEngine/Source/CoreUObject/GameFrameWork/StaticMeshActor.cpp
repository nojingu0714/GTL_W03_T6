#include "pch.h"
#include "StaticMeshActor.h"

#include "Components/StaticMeshComponent.h"

AStaticMeshActor::AStaticMeshActor()
{
	StaticMeshComponent = AddComponent<UStaticMeshComponent>(this);
}

void AStaticMeshActor::Tick(float TickTime)
{
	Super::Tick(TickTime);
}

void AStaticMeshActor::Destroy()
{
	Super::Destroy();
}
