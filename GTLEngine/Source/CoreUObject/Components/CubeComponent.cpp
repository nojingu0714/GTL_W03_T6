#include "pch.h"
#include "CubeComponent.h"

#include "Resource/Types.h"
#include "Utils/Math/Geometry.h"


#include "World.h"
#include "Core/Engine/Engine.h"
#include "CoreUObject/GameFrameWork/Actor.h"
#include "CoreUObject/Components/LineComponent.h"

UCubeComponent::UCubeComponent()
	: UPrimitiveComponent()
{
	PrimitiveType = EPrimitiveType::Cube;
}

void UCubeComponent::TickComponent(float TickTime)
{

}

void UCubeComponent::Destroy()
{
}

bool UCubeComponent::IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const {
	// OBB (Transformed ray with AABB)
	FMatrix transform = GetWorldMatrix().Inverse();
	FRay transformedRay = FRay(transform.TransformPositionVector(ray.Origin), transform.TransformDirectionVector(ray.Direction).GetSafeNormal());
	FBoundingBox AABBorigin = FBoundingBox(FVector(-0.5, -0.5, -0.5), FVector(0.5, 0.5, 0.5));

	bool result = Geometry::IsRayIntersectAABB(AABBorigin, transformedRay, hitDistance);
	if (result) {
		hitPoint = GetComponentLocation();
	}
	
	return result;
}
