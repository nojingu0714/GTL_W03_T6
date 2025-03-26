#pragma once

#include "GizmoBase.h"

class UGizmoTranslate : public UGizmoBase
{
	DECLARE_CLASS(UGizmoTranslate, UGizmoBase)
public:
	UGizmoTranslate();

	void Init(EAxis axis, AActor* Target);

	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

	void OnClick(int mx, int my) override;
	void OnDragTick(int mx, int my, int dmx, int dmy) override;
	void OnRelease(int mx, int my) override;

	virtual bool IsClicked(FRay ray, float maxDistance, FVector& hitpoint) override;
};