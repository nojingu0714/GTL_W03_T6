#include "pch.h"
#include "SphereComponent.h"
#include "CoreUObject/GameFrameWork/Actor.h"

USphereComponent::USphereComponent()
	: UPrimitiveComponent()
{
	PrimitiveType = EPrimitiveType::Sphere;
}

void USphereComponent::TickComponent(float TickTime)
{
}

void USphereComponent::Destroy()
{
}

FBoundingBox USphereComponent::GetAABB() const {
	FVector min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	FVector max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	FVector vecs[8] = {
		FVector(-1.f, -1.f, -1.f),
		FVector(1.f, -1.f, -1.f),
		FVector(-1.f, 1.f, -1.f),
		FVector(1.f, 1.f, -1.f),
		FVector(-1.f, -1.f, 1.f),
		FVector(1.f, -1.f, 1.f),
		FVector(-1.f, 1.f, 1.f),
		FVector(1.f, 1.f, 1.f),
	};

	FMatrix transform = GetWorldMatrix();
	for ( int i = 0; i < 8; i++ ) {
		vecs[i] = transform.TransformPositionVector(vecs[i]);
		if ( vecs[i].X < min.X ) min.X = vecs[i].X;
		if ( vecs[i].Y < min.Y ) min.Y = vecs[i].Y;
		if ( vecs[i].Z < min.Z ) min.Z = vecs[i].Z;
		if ( vecs[i].X > max.X ) max.X = vecs[i].X;
		if ( vecs[i].Y > max.Y ) max.Y = vecs[i].Y;
		if ( vecs[i].Z > max.Z ) max.Z = vecs[i].Z;
	}

	return FBoundingBox(min, max);
}
