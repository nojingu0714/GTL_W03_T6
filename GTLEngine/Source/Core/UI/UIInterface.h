#pragma once
#include "EnginePch.h"
#include "Core/Resource/Types.h"

class IClickable {
public:
	virtual void OnRelease(int mx, int my) abstract;
	virtual void OnClick(int mx, int my) abstract;
	virtual bool IsClicked(FRay ray, float maxDistance, FVector& hitpoint) abstract;
};

class IDragable: public IClickable {
public:
	virtual void OnDragTick(int dmx, int dmy) abstract;
};