#pragma once

#include "GameFrameWork/Actor.h"

class UGizmoTranslateComponent;
class UGizmoRotateComponent;
class UGizmoScaleComponent;

class AGizmoActor : public AActor
{
	DECLARE_CLASS(AGizmoActor, AActor)

public:
	AGizmoActor();

	void Init();

	void ProcessInput(FRay Ray);

	UGizmoTranslateComponent* GetGizmoTranslateComponent() const { return GizmoTranslateComponent; }
	UGizmoRotateComponent* GetGizmoRotateComponent() const { return GizmoRotateComponent; }
	UGizmoScaleComponent* GetGizmoScaleComponent() const { return GizmoScaleComponent; }

	//virtual void Tick(float TickTime);

	//GizmoState state;

private:
	USceneComponent* Pivot; // 사실상 rootcomponent.
	UGizmoTranslateComponent* GizmoTranslateComponent;
	UGizmoRotateComponent* GizmoRotateComponent;
	UGizmoScaleComponent* GizmoScaleComponent;

};


