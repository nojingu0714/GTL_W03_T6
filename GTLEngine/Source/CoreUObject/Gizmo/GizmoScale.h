#pragma once

#include "GizmoBase.h"

class AGizmoScale : public AGizmoBase
{
	DECLARE_CLASS(AGizmoScale, AGizmoBase)
public:
	AGizmoScale();

	void Init();

	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

	void OnClick(int mx, int my) override;
	void OnDragTick(FVector2 PointNDC, FVector2 DeltaNDC) override;
	void OnRelease(int mx, int my) override;

	virtual bool Intersects(FRay ray, float& hitDistance) override;
};