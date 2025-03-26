#include "pch.h"
#include "Geometry.h"
#include "Core/Engine/Engine.h"
#include "CoreUObject/World.h"
#include <algorithm>

using std::max;
using std::min;

bool Geometry::IsRayIntersectAABB(FBoundingBox aabb, FRay ray, float& hitDistance) {

	// reference: https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
    FVector rayDir = ray.Direction.GetSafeNormal();
	FVector dirfrac(1 / rayDir.X, 1 / rayDir.Y, 1 / rayDir.Z);

	float t1 = (aabb.min.X - ray.Origin.X) * dirfrac.X;
	float t2 = (aabb.max.X - ray.Origin.X) * dirfrac.X;
	float t3 = (aabb.min.Y - ray.Origin.Y) * dirfrac.Y;
	float t4 = (aabb.max.Y - ray.Origin.Y) * dirfrac.Y;
	float t5 = (aabb.min.Z - ray.Origin.Z) * dirfrac.Z;
	float t6 = (aabb.max.Z - ray.Origin.Z) * dirfrac.Z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if ( tmax < 0 ) {
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if ( tmin > tmax ) {
		return false;
	}

	hitDistance = tmin;

	return true;
}

bool Geometry::IsRayIntersectWithTriangle(const FRay& ray, const FVector& v0, const FVector& v1, const FVector& v2, float& hitDistance) {
    const float epsilon = 1e-6f;
    FVector edge1 = v1 - v0;
    FVector edge2 = v2 - v0;
    FVector ray_cross_e2 = ray.GetNormalizedDirection().Cross(edge2);

    float det = edge1.Dot(ray_cross_e2);

    if ( det > -epsilon && det < epsilon )
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0f / det;
    FVector s = ray.Origin - v0;
    float u = inv_det * s.Dot(ray_cross_e2);

    if ( (u < 0 && abs(u) > epsilon) || (u > 1 && abs(u - 1) > epsilon) )
        return false;

    FVector s_cross_e1 = s.Cross(edge1);
    float v = inv_det * ray.GetNormalizedDirection().Dot(s_cross_e1);

    if ( (v < 0 && abs(v) > epsilon) || (u + v > 1 && abs(u + v - 1) > epsilon) )
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    hitDistance = inv_det * edge2.Dot(s_cross_e1);

    if ( hitDistance > epsilon ) // ray intersection
    {
        return true;
    } else { // This means that there is a line intersection but not a ray intersection.
        return false;
    }
}

bool Geometry::IsRayIntersectWithPlane(const FRay& ray, const FVector& planeNormal, const FVector& planePoint, float& hitDistance)
{
	float d = -planeNormal.Dot(planePoint);
	hitDistance = -(planeNormal.Dot(ray.Origin) + d) / planeNormal.Dot(ray.Direction);
	if (hitDistance < 0)
		return false;
	return true;
}

bool Geometry::IsRayIntersectWithSphere(const FRay& ray, const FVector& sphereCenter, float sphereRadius, float& hitDistance)
{
	FVector L = sphereCenter - ray.Origin;
	float tca = L.Dot(ray.Direction);
	if (tca < 0)
		return false;
	float d2 = L.Dot(L) - tca * tca;
	if (d2 > sphereRadius * sphereRadius)
		return false;
	float thc = sqrt(sphereRadius * sphereRadius - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;
	if (t0 > t1)
		std::swap(t0, t1);
	if (t0 < 0)
	{
		t0 = t1;
		if (t0 < 0)
			return false;
	}
	hitDistance = t0;
	return true;
}

bool Geometry::IsRayIntersectWithQuad(const FRay& ray, const FVector& v0, const FVector& v1, const FVector& v2, const FVector& v3, float& hitDistance)
{
	float hitDistance1, hitDistance2;
	FVector hitpoint1, hitpoint2;

	if (!IsRayIntersectWithTriangle(ray, v0, v1, v2, hitDistance1))
		return false;
	if (!IsRayIntersectWithTriangle(ray, v0, v2, v3, hitDistance2))
		return false;

	hitDistance = min(hitDistance1, hitDistance2);
	
	return true;
}

bool Geometry::IsRayIntersectWithBox(const FRay& ray, const FVector& boxCenter, const FVector& boxExtent, const FMatrix& boxRotation, float& hitDistance)
{
	FVector localRayOrigin = boxRotation.Inverse().TransformPositionVector(ray.Origin - boxCenter);
	FVector localRayDirection = boxRotation.Inverse().TransformDirectionVector(ray.Direction);

	return IsRayIntersectAABB(FBoundingBox(-boxExtent, boxExtent), FRay(localRayOrigin, localRayDirection), hitDistance);


	// 안될듯....
	FVector min = -boxExtent;
	FVector max = boxExtent;

	float tmin = (min.X - localRayOrigin.X) / localRayDirection.X;
	float tmax = (max.X - localRayOrigin.X) / localRayDirection.X;

	if (tmin > tmax)
		std::swap(tmin, tmax);

	float tymin = (min.Y - localRayOrigin.Y) / localRayDirection.Y;
	float tymax = (max.Y - localRayOrigin.Y) / localRayDirection.Y;

	if (tymin > tymax)
		std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.Z - localRayOrigin.Z) / localRayDirection.Z;
	float tzmax = (max.Z - localRayOrigin.Z) / localRayDirection.Z;

	if (tzmin > tzmax)
		std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	hitDistance = tmin;

	return true;
}


FRay Geometry::CreateRayWithMouse(float NDCMouseX, float NDCMouseY) {
	FWindowInfo WInfo = UEngine::GetEngine().GetWindowInfo();

	//float ViewX = (2.0f * MouseX) / WInfo.Width - 1.0f;
	//float ViewY = (-2.0f * MouseY) / WInfo.Height + 1.0f;
	float NDCX = NDCMouseX;
	float NDCY = NDCMouseY;

	// Projection 공간으로 변환
	FVector4 RayOrigin = FVector4(NDCX, NDCY, 0.0f, 1.0f);
	FVector4 RayEnd = FVector4(NDCX, NDCY, 1.0f, 1.0f);

	//// View 공간으로 변환
	//FMatrix InvProjMat = UEngine::GetEngine().GetWorld()->GetProjectionMatrix().Inverse();

	//RayOrigin = InvProjMat.TransformVector4(RayOrigin);
	//RayEnd = InvProjMat.TransformVector4(RayEnd);

	//FMatrix InvViewMat = UEngine::GetEngine().GetWorld()->GetViewMatrix().Inverse();
	//RayOrigin = InvViewMat.TransformVector4(RayOrigin);
	//RayEnd = InvViewMat.TransformVector4(RayEnd);
	//RayOrigin /= RayOrigin.W;
	//RayEnd /= RayEnd.W;

	FVector RayDir = (RayEnd - RayOrigin);

	return FRay(RayOrigin, RayDir);
}
