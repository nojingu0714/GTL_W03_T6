#pragma once

//#include "Components/PrimitiveComponent.h"
#include "Core/Resource/Types.h"
#include "CoreUObject/GameFrameWork/Actor.h"
#include "Core/Engine/Engine.h"
#include "CoreUObject/World.h"
#include "Utils/Math/Geometry.h"
#include "Resource/ResourceManager.h"

enum class EGizmoModeIndex
{
	GMI_GizmoTranslate,
	GMI_GizmoRotate,
	GMI_GizmoScale,
};

class AGizmoBase : public AActor
{
	DECLARE_CLASS(AGizmoBase, AActor)
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
	AGizmoBase();

	void Init();

	virtual void Tick(float TickTime);
	virtual void Destroy() override;

	virtual void OnClick(int mx, int my) {}
	virtual void OnDragTick(FVector2 PointNDC, FVector2 DeltaNDC) {}
	virtual void OnRelease(int mx, int my) {}


	virtual FBoundingBox GetAABB() const;

	virtual bool Intersects(FRay ray, float& hitDistance) { return false; };
	EGizmoViewType GetGizmoViewType() const { return GizmoViewType; }
	AActor* GetTargetActor() { return Target; }
	EGizmoModeIndex GizmoMode;
};