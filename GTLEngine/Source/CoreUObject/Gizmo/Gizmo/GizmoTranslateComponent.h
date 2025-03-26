#pragma once

#include "Components/StaticMeshComponent.h"

// GizmoTranslate.obj : x: 0 - 1.724, y:0 - 0.7, z: 0 - 0.7
class UGizmoTranslateMesh : public UStaticMeshComponent
{
	DECLARE_CLASS(UGizmoTranslateMesh, UStaticMeshComponent)

public:
	UGizmoTranslateMesh();

	bool Intersects(const FRay ray) const;


private:
	const FBoundingBox LocalBox = FBoundingBox(FVector(1.724, 0.7, 0.7), FVector(0, -0.7, -0.7));
};