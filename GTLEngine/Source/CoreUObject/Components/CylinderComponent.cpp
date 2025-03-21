#include "pch.h"
#include "CylinderComponent.h"

UCylinderComponent::UCylinderComponent()
	: UPrimitiveComponent()
{
	PrimitiveType = EPrimitiveType::Cylinder;
	OriginalAABB[0] = FVector(-0.5f, -0.5f, 0.f);
	OriginalAABB[1] = FVector(0.5f, -0.5f, 0.f);
	OriginalAABB[2] = FVector(-0.5f, 0.5f, 0.f);
	OriginalAABB[3] = FVector(0.5f, 0.5f, 0.f);
	OriginalAABB[4] = FVector(-0.5f, -0.5f, 1.f);
	OriginalAABB[5] = FVector(0.5f, -0.5f, 1.f);
	OriginalAABB[6] = FVector(-0.5f, 0.5f, 1.f);
	OriginalAABB[7] = FVector(0.5f, 0.5f, 1.f);
}

void UCylinderComponent::TickComponent(float TickTime)
{
}

void UCylinderComponent::Destroy()
{
}
