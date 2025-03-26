#include "pch.h"
#include "Utils/Math/Geometry.h"
#include "GizmoStaticMeshComponent.h"
#include "Asset/ObjManager.h"


AGizmoTranslateMesh::AGizmoTranslateMesh()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
}

bool AGizmoTranslateMesh::Intersects(const FRay ray, float& distance) const
{
	FVector RayOrigin, RayDirection;
	RayOrigin = ray.Origin;
	RayDirection = ray.Direction;

	// Ray to local space
	FMatrix WorldToLocal = GetWorldMatrix().Inverse();
	RayOrigin = WorldToLocal.TransformPositionVector(RayOrigin);
	RayDirection = WorldToLocal.TransformDirectionVector(RayDirection);

	FRay LocalRay = FRay(RayOrigin, RayDirection);

	if (Geometry::IsRayIntersectAABB(LocalBox, ray, distance))
		return true;
	return false;
}

AGizmoRotateMesh::AGizmoRotateMesh()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotateNormalized.obj")));
}

bool AGizmoRotateMesh::Intersects(const FRay ray, float &distance) const
{
	FVector Normal = GetWorldMatrix().TransformDirectionVector(FVector(0, 0, 1));
	FVector PlanePoint = GetWorldMatrix().TransformPositionVector(FVector(0, 0, 0));

	// 가장 적은 빈도로 적중하는 순서로 체크.
	if (Geometry::IsRayIntersectWithSphere(ray, PlanePoint, OuterRadius, distance))
		if (!Geometry::IsRayIntersectWithSphere(ray, PlanePoint, InnerRadius, distance))
			// 거리는 여기서 계산.
			if (Geometry::IsRayIntersectWithPlane(ray, Normal, PlanePoint, distance))
				return true;

	return false;
}

AGizmoScaleMesh::AGizmoScaleMesh()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
}

bool AGizmoScaleMesh::Intersects(const FRay ray, float& distance) const
{
	FVector RayOrigin, RayDirection;
	RayOrigin = ray.Origin;
	RayDirection = ray.Direction;

	// Ray to local space
	FMatrix WorldToLocal = GetWorldMatrix().Inverse();
	RayOrigin = WorldToLocal.TransformPositionVector(RayOrigin);
	RayDirection = WorldToLocal.TransformDirectionVector(RayDirection);

	FRay LocalRay = FRay(RayOrigin, RayDirection);

	if (Geometry::IsRayIntersectAABB(LocalBox, ray, distance))
		return true;
	return false;
}
