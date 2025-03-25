#pragma once

#include "SceneComponent.h"
#include "Core/Resource/Types.h"

class UPrimitiveComponent : public USceneComponent
{
	DECLARE_CLASS(UPrimitiveComponent, USceneComponent)
public:
	UPrimitiveComponent();

	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;

public:

	virtual FBoundingBox GetAABB(const FMatrix& LocalToWorld) const;
	virtual bool IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const override;

protected:
	EPrimitiveType PrimitiveType;
	FVector OriginalAABB[8] = {
		FVector(-0.5f, -0.5f, -0.5f),
		FVector(0.5f, -0.5f, -0.5f),
		FVector(-0.5f, 0.5f, -0.5f),
		FVector(0.5f, 0.5f, -0.5f),
		FVector(-0.5f, -0.5f, 0.5f),
		FVector(0.5f, -0.5f, 0.5f),
		FVector(-0.5f, 0.5f, 0.5f),
		FVector(0.5f, 0.5f, 0.5f),
	};



};