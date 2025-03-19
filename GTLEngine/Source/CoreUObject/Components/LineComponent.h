#pragma once
#include "CoreUObject/Components/PrimitiveComponent.h"

class ULineComponent: public UPrimitiveComponent 
{
	DECLARE_CLASS(ULineComponent, UPrimitiveComponent)
private:
	FMatrix rotation;

public:
	ULineComponent();
	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;
	virtual FBoundingBox GetAABB() const override;
	virtual bool IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const override;
	//inline FVector GetPosition() const { return RelativeLocation; };
	//inline FVector GetDirection() const { return (end - start).GetSafeNormal(); }
	//inline FVector GetVector() const { return (end - start); }
	//inline FRay GetRay() const { return FRay(start, GetDirection()); }

	inline void SetDirection(FMatrix m) { rotation = m; }
	FMatrix GetWorldMatrix() const;
	FVector GetStartPoint() const;
	FVector GetEndPoint() const;
};

