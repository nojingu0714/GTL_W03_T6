#pragma once

#include "PrimitiveComponent.h"

class UMeshComponent: public UPrimitiveComponent
{
	DECLARE_CLASS(UMeshComponent, UPrimitiveComponent)
public:
	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;
};