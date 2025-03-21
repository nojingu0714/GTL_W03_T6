#pragma once

#include "PrimitiveComponent.h"

class UConeComponent : public UPrimitiveComponent
{
	DECLARE_CLASS(UConeComponent, UPrimitiveComponent)
public:
	UConeComponent();
	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;
protected:
};