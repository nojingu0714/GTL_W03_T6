#include "pch.h"
#include "StaticMeshActor.h"

#include "Components/StaticMeshComponent.h"

AStaticMeshActor::AStaticMeshActor()
{
	StaticMeshComponent = AddComponent<UStaticMeshComponent>(this);
}

void AStaticMeshActor::Tick(float TickTime)
{
	FVector pos = GetActorLocation();
	FRotator rot = GetActorRotation();

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

void AStaticMeshActor::SetStaticMesh(UStaticMesh* InStaticMesh)
{
	StaticMeshComponent->SetStaticMesh(InStaticMesh);
}

void AStaticMeshActor::SetStaticMesh(UStaticMeshComponent* InStaticMeshComponent)
{
	StaticMeshComponent = InStaticMeshComponent;
}
