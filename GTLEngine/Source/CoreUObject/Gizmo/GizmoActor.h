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

	UGizmoTranslateComponent* GetGizmoTranslateComponent() const { return GizmoTranslateComponent; }
	UGizmoRotateComponent* GetGizmoRotateComponent() const { return GizmoRotateComponent; }
	UGizmoScaleComponent* GetGizmoScaleComponent() const { return GizmoScaleComponent; }

private:
	UGizmoTranslateComponent* GizmoTranslateComponent;
	UGizmoRotateComponent* GizmoRotateComponent;
	UGizmoScaleComponent* GizmoScaleComponent;

};