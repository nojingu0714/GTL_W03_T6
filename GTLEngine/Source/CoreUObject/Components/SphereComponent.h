#pragma once

#include "PrimitiveComponent.h"

class USphereComponent : public UPrimitiveComponent
{
	DECLARE_CLASS(USphereComponent, UPrimitiveComponent)

public:
	USphereComponent();

	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;
	virtual FBoundingBox GetAABB() const override;
};