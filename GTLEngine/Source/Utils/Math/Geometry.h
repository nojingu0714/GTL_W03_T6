#pragma once
#include "Core/Resource/Types.h"

class Geometry {
public:
	static bool IsRayIntersectAABB(FBoundingBox aabb, FRay ray, float& hitDistance);
	static bool IsRayIntersectWithTriangle(const FRay& ray, const FVector& v0, const FVector& v1, const FVector& v2, float& hitDistance);
	static bool IsRayIntersectWithPlane(const FRay& ray, const FVector& planeNormal, const FVector& planePoint, float& hitDistance);
	static bool IsRayIntersectWithSphere(const FRay& ray, const FVector& sphereCenter, float sphereRadius, float& hitDistance);
	static bool IsRayIntersectWithQuad(const FRay& ray, const FVector& v0, const FVector& v1, const FVector& v2, const FVector& v3, float& hitDistance);
	static bool IsRayIntersectWithBox(const FRay& ray, const FVector& boxCenter, const FVector& boxExtent, const FMatrix& boxRotation, float& hitDistance);
	static FRay CreateRayWithMouse(float NDCMouseX, float NDCMouseY);
};

