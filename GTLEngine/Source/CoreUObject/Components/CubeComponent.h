#pragma once

#include "PrimitiveComponent.h"

class UCubeComponent : public UPrimitiveComponent
{
	DECLARE_CLASS(UCubeComponent, UPrimitiveComponent)
public:
	UCubeComponent();

	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;
	virtual bool IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const override;
};