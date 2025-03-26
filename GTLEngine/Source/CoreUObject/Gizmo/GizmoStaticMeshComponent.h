#pragma once

#include "Components/StaticMeshComponent.h"

// GizmoTranslate.obj : x: 0 - 1.724, y:0 - 0.7, z: 0 - 0.7
class AGizmoTranslateMesh : public UStaticMeshComponent
{
	DECLARE_CLASS(AGizmoTranslateMesh, UStaticMeshComponent)

public:
	AGizmoTranslateMesh();

	bool Intersects(const FRay ray, float& distance) const;
	
private:
	const FBoundingBox LocalBox = FBoundingBox(FVector(1.724, 0.7, 0.7), FVector(0, -0.7, -0.7));
};


// +x, +y 방향으로 0.88f, 1.0f
class AGizmoRotateMesh : public UStaticMeshComponent
{
	DECLARE_CLASS(AGizmoRotateMesh, UStaticMeshComponent)

public:
	AGizmoRotateMesh();

	bool Intersects(const FRay ray, float& distance) const;

private:
	const float InnerRadius = 0.88f;
	const float OuterRadius = 1.0f;
};

class AGizmoScaleMesh : public UStaticMeshComponent
{
	DECLARE_CLASS(AGizmoScaleMesh, UStaticMeshComponent)

public:
	AGizmoScaleMesh();

	bool Intersects(const FRay ray, float& distance) const;

	const FBoundingBox LocalBox = FBoundingBox(FVector(1.724, 0.7, 0.7), FVector(0, -0.7, -0.7));

};

