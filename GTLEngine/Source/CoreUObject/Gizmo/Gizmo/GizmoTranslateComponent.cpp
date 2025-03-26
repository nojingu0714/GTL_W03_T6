#include "pch.h"
#include "GizmoTranslateComponent.h"
#include "Utils/Math/Geometry.h"

#include "Asset/ObjManager.h"

UGizmoTranslateMesh::UGizmoTranslateMesh()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
}

bool UGizmoTranslateMesh::Intersects(const FRay ray) const
{
	FVector RayOrigin, RayDirection;
	RayOrigin = ray.Origin;
	RayDirection = ray.Direction;

	// Ray to local space
	FMatrix WorldToLocal = GetWorldMatrix().Inverse();
	RayOrigin = WorldToLocal.TransformPositionVector(RayOrigin);
	RayDirection = WorldToLocal.TransformDirectionVector(RayDirection);

	FRay LocalRay = FRay(RayOrigin, RayDirection);

	if (Geometry::IsRayIntersectAABB(LocalBox, ray, 1000.f))
		return true;
	return false;
}
