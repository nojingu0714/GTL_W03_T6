#include "pch.h"
#include "Geometry.h"
#include "Core/Engine/Engine.h"
#include "CoreUObject/World.h"
#include <algorithm>

using std::max;
using std::min;

bool Geometry::IsRayIntersectAABB(FBoundingBox aabb, FRay ray, float maxDistance = 100.f) {

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

	return true;
}

bool Geometry::IsRayIntersectWithTriangle(const FRay& ray, const FVector& v0, const FVector& v1, const FVector& v2, float hitDistance, FVector& hitpoint) {

    hitpoint = FVector::Zero();

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
    float t = inv_det * edge2.Dot(s_cross_e1);

    if ( t > epsilon ) // ray intersection
    {
        hitpoint = ray.Origin + ray.GetNormalizedDirection() * t;
        return true;
    } else { // This means that there is a line intersection but not a ray intersection.
        return false;
    }
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

	// View 공간으로 변환
	FMatrix InvProjMat = UEngine::GetEngine().GetWorld()->GetProjectionMatrix().Inverse();

	RayOrigin = InvProjMat.TransformVector4(RayOrigin);
	RayEnd = InvProjMat.TransformVector4(RayEnd);

	FMatrix InvViewMat = UEngine::GetEngine().GetWorld()->GetViewMatrix().Inverse();
	RayOrigin = InvViewMat.TransformVector4(RayOrigin);
	RayEnd = InvViewMat.TransformVector4(RayEnd);
	RayOrigin /= RayOrigin.W;
	RayEnd /= RayEnd.W;

	FVector RayDir = (RayEnd - RayOrigin);

	return FRay(RayOrigin, RayDir);
}
