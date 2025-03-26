#pragma once

#include "GizmoBase.h"

class AGizmoTranslate : public AGizmoBase
{
	DECLARE_CLASS(AGizmoTranslate, AGizmoBase)
public:
	AGizmoTranslate();

	void Init();

	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

	void OnDragTick(FVector2 PointNDC, FVector2 DeltaNDC) override;

	virtual bool Intersects(FRay ray, float& hitDistance) override;
};