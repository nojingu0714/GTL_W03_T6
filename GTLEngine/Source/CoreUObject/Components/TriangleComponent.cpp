#include "pch.h"
#include "TriangleComponent.h"

UTriangleComponent::UTriangleComponent()
	: UPrimitiveComponent()
{
	PrimitiveType = EPrimitiveType::Triangle;
	OriginalAABB[0] = FVector(0.f, - 0.5f, -0.5f);
	OriginalAABB[1] = FVector(0.f, 0.5f, -0.5f);
	OriginalAABB[2] = FVector(0.f, - 0.5f, 0.5f);
	OriginalAABB[3] = FVector(0.f, 0.5f, 0.5f);
	OriginalAABB[4] = FVector(0.f, - 0.5f, -0.5f);
	OriginalAABB[5] = FVector(0.f, 0.5f, -0.5f);
	OriginalAABB[6] = FVector(0.f, - 0.5f, 0.5f);
	OriginalAABB[7] = FVector(0.f, 0.5f, 0.5f);
}

void UTriangleComponent::Tick(float TickTime)
{
}

void UTriangleComponent::Destroy()
{
}
