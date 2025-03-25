#pragma once

#include "Core/Resource/Types.h"
#include "CoreUObject/GameFrameWork/Actor.h"
#include "Core/Engine/Engine.h"
#include "CoreUObject/World.h"
#include "Utils/Math/Geometry.h"
#include "Resource/ResourceManager.h"

class UGizmoBase : public UObject
{
	DECLARE_CLASS(UGizmoBase, UObject)
public:
	enum class EAxis {
		X,
		Y,
		Z
	};
	static bool IsAbsoluteCoord;
protected:	
	FVector OriginalAABB[8];
	EGizmoViewType GizmoViewType;
	EAxis axis = EAxis::X;
	AActor* Target;
	int startMouseX, startMouseY;
	FMatrix mat = FMatrix::Identity();
public:
	UGizmoBase();

	void Init(EAxis axis, AActor* Target);

	virtual void Tick(float TickTime);
	virtual void Destroy() override;

	virtual FBoundingBox GetAABB() const;

	virtual bool IsClicked(FRay ray, float maxDistance, FVector& hitpoint);
	EGizmoViewType GetGizmoViewType() const { return GizmoViewType; }
	AActor* GetTargetActor() { return Target; }
	EGizmoModeIndex GizmoMode;
};