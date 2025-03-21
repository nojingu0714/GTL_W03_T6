#pragma once

#include "PrimitiveComponent.h"

class UTriangleComponent : public UPrimitiveComponent
{
	DECLARE_CLASS(UTriangleComponent, UPrimitiveComponent)
public:
	UTriangleComponent();
	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;
};