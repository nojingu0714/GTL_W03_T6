#pragma once

#include "Object.h"
#include "Core/Resource/Types.h"

class AActor;

class UActorComponent : public UObject
{
	DECLARE_CLASS(UActorComponent, UObject)
public:
	UActorComponent()
		: UObject()
		, OwnerPrivate(nullptr)
	{
	}

	virtual void TickComponent(float TickTime);
	virtual void Destroy() override;
	virtual FBoundingBox GetAABB() const;
	virtual bool IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const;
public:
	inline AActor* GetOwner() const { return OwnerPrivate; }
	inline void SetOwner(AActor* InOwner) { OwnerPrivate = InOwner; }

private:
	AActor* OwnerPrivate;
};