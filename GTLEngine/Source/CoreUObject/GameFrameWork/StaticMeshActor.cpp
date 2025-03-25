#include "pch.h"
#include "StaticMeshActor.h"

#include "Components/StaticMeshComponent.h"

AStaticMeshActor::AStaticMeshActor()
{
	StaticMeshComponent = AddComponent<UStaticMeshComponent>(this);
}

void AStaticMeshActor::Tick(float TickTime)
{
	static FVector pos(0, 0, 0);
	static FRotator rot(0, 0, 0);

	pos.X += 0.01;
	rot.Yaw += 0.1;

	Super::Tick(TickTime);
	SetActorLocation(pos);
	this->SetActorRotation(rot);
}

void AStaticMeshActor::Destroy()
{
	Super::Destroy();
}
