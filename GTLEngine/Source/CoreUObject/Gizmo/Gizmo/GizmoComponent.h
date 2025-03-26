#pragma once
#include "Utils/Math/Geometry.h"
#include "CoreUObject/Components/SceneComponent.h"

class USceneComponent;
class UStaticMeshComponent;

class UGizmoBaseComponent : public USceneComponent
{
	DECLARE_CLASS(UGizmoBaseComponent, USceneComponent)
public:
	UGizmoBaseComponent();

	virtual void Init();
	virtual void TickComponent(float TickTime);
	virtual void Destroy();

public:
	virtual FVector GetHoveredHandleAxis(FRay Ray) const;

private:
	bool bIsSelected = false;
	bool bIsVisible = false;
};

class UGizmoTranslateComponent : public UGizmoBaseComponent
{
	DECLARE_CLASS(UGizmoTranslateComponent, UGizmoBaseComponent)
public:
	UGizmoTranslateComponent();

	virtual void Init() override;
	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;

public:
	virtual FVector GetHoveredHandleAxis(FRay Ray) const override;

private:
	const FVector HandleAxis[3] = { FVector(1, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1) };

	UStaticMeshComponent* XAxis;
	UStaticMeshComponent* YAxis;
	UStaticMeshComponent* ZAxis;
};

class UGizmoRotateComponent : public UGizmoBaseComponent
{
	DECLARE_CLASS(UGizmoRotateComponent, UGizmoBaseComponent)
public:
	UGizmoRotateComponent();

	virtual void Init() override;
	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;

public:
	virtual FVector GetHoveredHandleAxis(FRay Ray) const override;

private:
	const FVector HandleAxis[3] = { FVector(1, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1) };

	UStaticMeshComponent* XAxis;
	UStaticMeshComponent* YAxis;
	UStaticMeshComponent* ZAxis;
};

class UGizmoScaleComponent : public UGizmoBaseComponent
{
	DECLARE_CLASS(UGizmoScaleComponent, UGizmoBaseComponent)
public:
	UGizmoScaleComponent();

	virtual void Init() override;
	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;
	
public:
	virtual FVector GetHoveredHandleAxis(FRay Ray) const override;

private:
	const FVector HandleAxis[3] = { FVector(1, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1) };

	UStaticMeshComponent* XAxis;
	UStaticMeshComponent* YAxis;
	UStaticMeshComponent* ZAxis;
};
